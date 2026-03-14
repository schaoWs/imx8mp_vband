/*
 * Milestone 2: RPMsg Sensor Bridge (Fake Data)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* SDK and FreeRTOS Headers */
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"
#include "FreeRTOS.h"
#include "task.h"

/* Board and Application Specific Headers */
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_TASK_STACK_SIZE (256)
#ifndef LOCAL_EPT_ADDR
#define LOCAL_EPT_ADDR (30)
#endif

/* Data Structure for Milestone 2 */
typedef struct __attribute__((packed)) {
    int32_t temp_c_x10;    /* 555 = 55.5C */
    uint16_t adc_ch0;      /* 0999 */
    uint16_t adc_ch1;      /* 1111 */
    uint16_t adc_ch2;      /* 2222 */
    uint16_t adc_ch3;      /* 3333 */
    uint32_t uptime_ms;    /* M7 Uptime */
} sensor_data_t;

/*******************************************************************************
 * Code
 ******************************************************************************/
static TaskHandle_t app_task_handle = NULL;
static struct rpmsg_lite_instance *volatile my_rpmsg = NULL;
static struct rpmsg_lite_endpoint *volatile my_ept = NULL;
static volatile rpmsg_queue_handle my_queue        = NULL;

void app_task(void *param)
{
    volatile uint32_t remote_addr = 0U;
    void *rx_buf;
    uint32_t len;
    int32_t result;
    void *tx_buf;
    uint32_t size;

    sensor_data_t sensor_payload = {
        .temp_c_x10 = 555,
        .adc_ch0 = 999,
        .adc_ch1 = 1111,
        .adc_ch2 = 2222,
        .adc_ch3 = 3333
    };

    PRINTF("\r\nRPMSG Sensor Bridge Demo (Fake Data)...\r\n");

    /* Initialize RPMsg */
    my_rpmsg = rpmsg_lite_remote_init((void *)RPMSG_LITE_SHMEM_BASE, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
    rpmsg_lite_wait_for_link_up(my_rpmsg, RL_BLOCK);

    my_queue = rpmsg_queue_create(my_rpmsg);
    my_ept   = rpmsg_lite_create_ept(my_rpmsg, LOCAL_EPT_ADDR, rpmsg_queue_rx_cb, my_queue);

    SDK_DelayAtLeastUs(1000000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    (void)rpmsg_ns_announce(my_rpmsg, my_ept, "rpmsg-virtual-tty-channel-1", RL_NS_CREATE);

    PRINTF("Ready for data requests on /dev/ttyRPMSG30...\r\n");

    for (;;)
    {
        result = rpmsg_queue_recv_nocopy(my_rpmsg, my_queue, (uint32_t *)&remote_addr, (char **)&rx_buf, &len, RL_BLOCK);
        if (result != 0) assert(false);

        /* Update uptime */
        sensor_payload.uptime_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;

        /* Allocate and Send binary payload */
        tx_buf = rpmsg_lite_alloc_tx_buffer(my_rpmsg, &size, RL_BLOCK);
        assert(tx_buf);
        memcpy(tx_buf, &sensor_payload, sizeof(sensor_data_t));
        result = rpmsg_lite_send_nocopy(my_rpmsg, my_ept, remote_addr, tx_buf, sizeof(sensor_data_t));
        if (result != 0) assert(false);

        result = rpmsg_queue_nocopy_free(my_rpmsg, rx_buf);
        if (result != 0) assert(false);
    }
}

void app_create_task(void)
{
    if (xTaskCreate(app_task, "APP_TASK", APP_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &app_task_handle) != pdPASS)
    {
        PRINTF("\r\nFailed to create application task\r\n");
        for (;;);
    }
}

int main(void)
{
    /* Initialize Hardware */
    BOARD_InitHardware();
    
    app_create_task();
    vTaskStartScheduler();
    for (;;);
}
