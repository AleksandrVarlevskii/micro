#include "stdio-task/stdio-task.h"
#include "stdio.h"
#include "stdlib.h"
#include "pico/stdlib.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

int main(){
    stdio_init_all();
    stdio_task_init();

    while (1){

    stdio_task_handle();
    }






    return 0;
}
