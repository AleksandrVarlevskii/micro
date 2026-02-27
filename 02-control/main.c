#include "stdio-task/stdio-task.h"
#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

api_t device_api[];

void version_callback(const char* args);
void led_on_callback(const char* args);
void led_off_callback(const char* args);
void led_blink_callback(const char* args);
void led_blink_set_period_ms_callback(const char* args);
void help_callback(const char* args);











void version_callback(const char* args)
{
	printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}
void led_on_callback(const char* args){
    led_task_state_set(LED_STATE_ON);
}
void led_off_callback(const char* args){
    led_task_state_set(LED_STATE_OFF);
}
void led_blink_callback(const char* args){
    led_task_state_set(LED_STATE_BLINK);
}
void led_blink_set_period_ms_callback(const char* args){
    uint period_ms = 0;
    sscanf(args, "%u", &period_ms);
    if (period_ms == 0){
        printf("Error! Function led_blink_set_period_ms_callback received no argument ");
        return;
    }
    led_task_set_blink_period_ms(period_ms);
    /*Функция int sscanf(const char *str, const char *format, ...) String Scan Formatted (сканирование строки по формату).
    Первым аргументом мы передаем указатель на строку, которую будем сканировать,
    Вторым аргументом мы передаем сроку формата, которая определяет какие типы искать, их может быть указано несколько:
    "%u"     // искать беззнаковое целое число
    "%d"     // искать целое число со знаком
    "%s"     // искать строку (до пробела)
    "%f"     // искать число с плавающей точкой
    "%u %s"  // искать число и строку
    Оставшимися аргументами идут указатели на переменные, куда скалдывать найденый формат.
     Если поиск был по нескольким форматам, можно указать указатели на переменые в том же порядке.*/
}


void help_callback(const char* args){
    for (uint i = 0; device_api[i].command_help != NULL; i++) {
        printf("%s: %s\n",device_api[i].command_name, device_api[i].command_help);

    }
}
void mem_callback(const char* hex_str) {
    uint32_t addr = strtoul(hex_str, NULL, 16); 
    /* unsigned long strtoul(const char* str, char** endptr, int base); (PS: strtoul = string to unsigned long)
    Читает строку str и преобразует её в число типа unsigned long с учётом системы счисления base.
    str — строка для преобразования
    endptr — указатель, куда будет записан адрес первого непрочитанного символа (можно NULL, если не нужно)
    base — система счисления (2..36, или 0 для автоопределения)
    */
    uint32_t value = *(volatile uint32_t*)addr; // 
    printf("Value: 0x%08X (%u)\n", value, value); // 0x%08X формат 16 системы.
}

void wmem_callback(const char* hex_str) {
    uint32_t addr;
    uint32_t value;

    if (sscanf(hex_str, "%x:%x", &addr, &value) == 2) { // функция считывает строку hex_str и, с учетом формата "%x:%x", записывает полученные строки по указаным адресам
        // оба в шеснацеричной системе исчесления 
    }
    else {
        printf("Format error\n");
        return;
    }
    *(volatile uint32_t*)addr = value;

}


api_t device_api[] =
{
    {"version", version_callback, "get device name and firmware version"},
    {"led_off",led_off_callback , "The function turns off the LED."},
    {"led_on",led_on_callback, "The function turns on the LED."},
    {"led_blink",led_blink_callback, "The function makes the LED blink."},
    {"set_period",led_blink_set_period_ms_callback, "set LED blink period in milliseconds (e.g., 'set_period 500')"},
    {"help", help_callback, "show this help message or help for a specific command"},
    {"mem",mem_callback,  "This function returns the value stored at the address passed as an argument"},
    {"wmem", wmem_callback, "This function writes the specified value to the given address. Signature: wmem addres:value"},
    {NULL, NULL, NULL},
};





int main(){
    stdio_init_all();
    stdio_task_init();
    protocol_task_init(device_api);
    led_state_init();

    while (1){

    protocol_task_handle(stdio_task_handle());
    led_task_handler();
    }








    return 0;
}
