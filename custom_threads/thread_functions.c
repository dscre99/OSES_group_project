/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-09     dscre       the first version
 */

#include <rtthread.h>
#include <rthw.h>
#include <board.h>
#include "../custom_mailbox/custom_mailbox_init.h"

#define LED0_PIN    GET_PIN(A, 5)
#define BUT0_PIN    GET_PIN(C, 13)

void print_string(void *parameter)
{
    rt_kprintf("\nThread 'print_string' activated\n");

    rt_kprintf("\nTHIS IS A TEST STRING\n");

    rt_kprintf("\nThread 'print_string' terminating...\n");
}

void receive_message(void *parameter)
{
    int received_val = 0;

    rt_kprintf("\n---- receive_message_thread started ----\n\n");

    // to be completed
    while(1)
    {

        /* Receive messages from the message queue */
        if (rt_mb_recv(&mb, (rt_ubase_t *)(&received_val), RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("\rreceive_message: recv msg from msg queue, the content:%d ", received_val);
        }
        rt_thread_mdelay(5);
    }
}

void brake_detection(void * parameters){

    while(1){
        if (rt_pin_read(BUT0_PIN) != 1)
        {
            rt_pin_write(LED0_PIN, PIN_HIGH);
            //rt_mb_send(&mb, (rt_uint32_t) 1);
            rt_kprintf("brake_detection: 1\n");
        }
        else
        {
            rt_pin_write(LED0_PIN, PIN_LOW);
            //rt_mb_send(&mb, (rt_uint32_t) 0);
            rt_kprintf("brake_detection: 0\n");
        }

        rt_thread_mdelay(25);
    }
    return;
}

void throttle_detection(void * parameters){

    while (1)
    {
        rt_kprintf("throttle_detection\n");

        rt_thread_mdelay(50);
    }
    return;
}

void speed_detection(void * parameters){
    int speed = 0;

    while (1)
    {
        //rt_kprintf("speed_detection\n");
        speed += 1;
        speed = speed%100;
        rt_mb_send(&mb, (rt_uint32_t) speed);

        rt_thread_mdelay(50);
    }
    return;
}

void display_management(void * parameters){

    while (1)
    {
        //rt_kprintf("display_management\n");

        int speed_value = 0;

        /* Receive messages from the message queue */
        while (rt_mb_recv(&mb, (rt_ubase_t *) (&speed_value), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }

        // update display information
        rt_kprintf("\nDIPLAY:: |Speed: %d|\n", speed_value);

        rt_thread_mdelay(250);
    }
    return;
}

void motor_temperature(void * parameters){

    while (1)
    {
        rt_kprintf("motor_temperature\n");

        rt_thread_mdelay(1500);
    }
    return;
}

void battery_temperature(void * parameters){

    while (1)
    {
        rt_kprintf("battery_temperature\n");

        rt_thread_mdelay(1500);
    }
    return;
}

void battery_level(void * parameters){

    while (1)
    {
        rt_kprintf("battery_level\n");

        rt_thread_mdelay(1000);
    }
    return;
}

void auxiliary_light_management(void * parameters){

    while (1)
    {
        rt_kprintf("auxiliary_light_management\n");

        rt_thread_mdelay(300);
    }
    return;
}

