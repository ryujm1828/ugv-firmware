#include "app_rtos.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// temporary structs
typedef struct {
    uint8_t type;
    uint16_t seq;
    int16_t left;
    int16_t right;
} command_event_t;

typedef struct {
    uint8_t data[16];
    uint8_t len;
} tx_event_t;

#define CMD_QUEUE_LEN 8
#define TX_QUEUE_LEN 8

// ==========================================
// 1. ControlTask (Priority High, Stack 1024B)
// ==========================================
#define CTRL_STACK_BYTES 1024
static StaticTask_t s_ctrl_tcb;
static uint32_t s_ctrl_stack[CTRL_STACK_BYTES / 4U] __attribute__((aligned(8)));
static const osThreadAttr_t k_ctrl_attr = {
    .name = "ControlTask",
    .cb_mem = &s_ctrl_tcb,
    .cb_size = sizeof(s_ctrl_tcb),
    .stack_mem = s_ctrl_stack,
    .stack_size = sizeof(s_ctrl_stack),
    .priority = (osPriority_t)osPriorityHigh
};

// ==========================================
// 2. CommunicationTask (Priority AboveNormal, Stack 2048B)
// ==========================================
#define COMM_STACK_BYTES 2048
static StaticTask_t s_comm_tcb;
static uint32_t s_comm_stack[COMM_STACK_BYTES / 4U] __attribute__((aligned(8)));
static const osThreadAttr_t k_comm_attr = {
    .name = "CommunicationTask",
    .cb_mem = &s_comm_tcb,
    .cb_size = sizeof(s_comm_tcb),
    .stack_mem = s_comm_stack,
    .stack_size = sizeof(s_comm_stack),
    .priority = (osPriority_t)osPriorityAboveNormal
};

// ==========================================
// 3. TxTask (Priority Normal, Stack 1024B)
// ==========================================
#define TX_STACK_BYTES 1024
static StaticTask_t s_tx_tcb;
static uint32_t s_tx_stack[TX_STACK_BYTES / 4U] __attribute__((aligned(8)));
static const osThreadAttr_t k_tx_attr = {
    .name = "TxTask",
    .cb_mem = &s_tx_tcb,
    .cb_size = sizeof(s_tx_tcb),
    .stack_mem = s_tx_stack,
    .stack_size = sizeof(s_tx_stack),
    .priority = (osPriority_t)osPriorityNormal
};

// ==========================================
// 4. CommandQueue & TxQueue
// ==========================================
static StaticQueue_t s_cmdq_cb;
static command_event_t s_cmdq_buf[CMD_QUEUE_LEN];
static const osMessageQueueAttr_t k_cmdq_attr = {
    .name = "CommandQueue",
    .cb_mem = &s_cmdq_cb,
    .cb_size = sizeof(s_cmdq_cb),
    .mq_mem = s_cmdq_buf,
    .mq_size = sizeof(s_cmdq_buf)
};
osMessageQueueId_t cmd_queue_id;

static StaticQueue_t s_txq_cb;
static tx_event_t s_txq_buf[TX_QUEUE_LEN];
static const osMessageQueueAttr_t k_txq_attr = {
    .name = "TxQueue",
    .cb_mem = &s_txq_cb,
    .cb_size = sizeof(s_txq_cb),
    .mq_mem = s_txq_buf,
    .mq_size = sizeof(s_txq_buf)
};
osMessageQueueId_t tx_queue_id;

// ==========================================
// Task Functions
// ==========================================
static void ControlTask(void *arg) {
    for(;;) { osDelay(100); }
}

static void CommunicationTask(void *arg) {
    for(;;) { osDelay(100); }
}

static void TxTask(void *arg) {
    for(;;) { osDelay(100); }
}

// ==========================================
// Initialization System
// ==========================================
void app_rtos_init(void) {
    // 1. Make Queue (8 elements each)
    cmd_queue_id = osMessageQueueNew(CMD_QUEUE_LEN, sizeof(command_event_t), &k_cmdq_attr);
    tx_queue_id = osMessageQueueNew(TX_QUEUE_LEN, sizeof(tx_event_t), &k_txq_attr);

    // 2. Make Tasks
    osThreadNew(ControlTask, NULL, &k_ctrl_attr);
    osThreadNew(CommunicationTask, NULL, &k_comm_attr);
    osThreadNew(TxTask, NULL, &k_tx_attr);
}