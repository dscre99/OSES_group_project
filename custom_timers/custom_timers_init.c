/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     dscre       the first version
 */

#include <rtthread.h>
#include <rthw.h>
#include <board.h>
#include "../custom_threads/thread_functions.h"
#include "../custom_threads/custom_threads_init.h"
#include "../custom_mailbox/custom_mailbox_init.h"

#define ONE_SECOND 1000
#define _25_MSECOND 25

#define LED0_PIN    GET_PIN(A, 5)
#define BUT0_PIN    GET_PIN(C, 13)

void generate_value(void * parameters);
void brake_detection(void * parameters);

static struct rt_timer print_timer;
static struct rt_timer brake_timer;
int value = 0;

// test timer with thread activation
int custom_timers_init(void)
{

    // print_string timer initialization
    //rt_timer_init(&print_timer, "print_timer", launch_print_string_thread, RT_NULL, ONE_SECOND, RT_TIMER_FLAG_PERIODIC);
    //rt_kprintf("Test pointer:: %d\n", &value);
    rt_timer_init(&brake_timer, "brake_timer", brake_detection, &value, _25_MSECOND, RT_TIMER_FLAG_PERIODIC);


    // print_string timer startup
    //rt_timer_start(&print_timer);
    rt_timer_start(&brake_timer);

    return 0;
}

void generate_value(void * parameters){
    int *val_p = (int *)parameters;

    rt_kprintf("Generated value:: %d\n", *val_p);
    rt_mb_send(&mb, (rt_uint32_t)*val_p);

    *val_p += 1;

    return;
}

void brake_detection(void * parameters){
    int *val_p = (int *)parameters;
    if( rt_pin_read(BUT0_PIN)!=1){
        rt_pin_write(LED0_PIN, PIN_HIGH);
        *val_p += 1;
    } else {
        rt_pin_write(LED0_PIN, PIN_LOW);
        *val_p =0;
    }
    rt_mb_send(&mb, (rt_uint32_t)*val_p);

    return;
}

// test timer with function
/*int custom_timers_init(void)
{
    // print_string timer initialization
    rt_timer_init(&print_timer, "print_timer", print_string, RT_NULL, ONE_SECOND, RT_TIMER_FLAG_PERIODIC);

    // print_string timer startup
    rt_timer_start(&print_timer);

    return 0;
}*/
