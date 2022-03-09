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

void print_string(void *parameter)
{
    rt_kprintf("\nThread 'print_string' activated\n");

    rt_kprintf("\nTHIS IS A TEST STRING\n");

    rt_kprintf("\nThread 'print_string' terminating...\n");
}
