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

/* Thread Functions Prototypes */
void print_string(void *parameter);

struct rt_thread print_string_thread;
static rt_uint8_t print_string_thread_stack[512];

void custom_threads_init(void)
{
    /* Initialize print string thread */
        /* The thread entry is print_string and the parameter is RT_NULL
         * Thread stack is print_string_thread_stack
         * Priority is 5 and time slice is 20 OS Tick
         */
    rt_thread_init(&print_string_thread, "print_string_thread", print_string,
                    RT_NULL, print_string_thread_stack, sizeof(print_string_thread_stack),
                    5, 20);

}
