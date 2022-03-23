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
#include "../custom_threads/thread_functions.h"
#include "../custom_threads/custom_threads_init.h"

#define ONE_SECOND 1000

void generate_value(void * parameters);

static struct rt_timer print_timer;
int value = 0;

// test timer with thread activation
int custom_timers_init(void)
{

    // print_string timer initialization
    //rt_timer_init(&print_timer, "print_timer", launch_print_string_thread, RT_NULL, ONE_SECOND, RT_TIMER_FLAG_PERIODIC);
    rt_kprintf("Test pointer:: %d\n", &value);
    rt_timer_init(&print_timer, "print_timer", generate_value, &value, ONE_SECOND, RT_TIMER_FLAG_PERIODIC);


    // print_string timer startup
    rt_timer_start(&print_timer);

    return 0;
}

void generate_value(void * parameters){
    int *val_p = (int *)parameters;

    rt_kprintf("Generated value:: %d\n", *val_p);
    *val_p += 1;

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
