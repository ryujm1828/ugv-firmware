#include "app_rtos.h"
#include "cmsis_os2.h"
#include "main.h" // HAL_GPIO_TogglePin, GPIOA, GPIO_PIN_5
#include "FreeRTOS.h"
#include "task.h"

// Static allocation TCB and stack for the BlinkTask
#define BLINK_STACK_BYTES 512
static StaticTask_t s_blink_tcb;
static uint32_t s_blink_stack[BLINK_STACK_BYTES/4U] __attribute__((aligned(8)));

static const osThreadAttr_t blink_attr = {
    .name = "BlinkTask",
    .cb_mem = &s_blink_tcb,
    .cb_size = sizeof(s_blink_tcb),
    .stack_mem = s_blink_stack,
    .stack_size = sizeof(s_blink_stack),
    .priority = (osPriority_t)osPriorityNormal
};

// Test function to toggle an LED every 500ms
static void BlinkTask(void *arg) {
    for(;;) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        osDelay(500);
    }
}

void app_rtos_init(void) {
    osThreadNew(BlinkTask, NULL, &blink_attr);
}