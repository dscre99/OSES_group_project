/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-23     david       the first version
 */

#include <rtthread.h>
#include "../custom_mailbox/custom_mailbox_init.h"

struct rt_mailbox mb;
struct rt_mailbox mb_speed_display;
struct rt_mailbox mb_mottemp_display;
struct rt_mailbox mb_battemp_display;
struct rt_mailbox mb_batlevel_display;
struct rt_mailbox mb_brake_throttle;
struct rt_mailbox mb_speed_throttle;

static char mb_pool[128];
static char mb_speed_display_pool[128];
static char mb_mottemp_display_pool[16];
static char mb_battemp_display_pool[16];
static char mb_batlevel_display_pool[24];
static char mb_brake_throttle_pool[128];
static char mb_speed_throttle_pool[128];

int custom_mailbox_init(void)
{
    rt_err_t err_control;

    // initializes receive_message mailbox
    err_control = rt_mb_init(&mb,
                        "mb",                      /* Name is mb*/
                        &mb_pool[0],                /* The memory pool used by the mailbox is mb_pool */
                        sizeof(mb_pool) / 4,        /* The number of messages in the mailbox because a message occupies 4 bytes */
                        RT_IPC_FLAG_FIFO);          /* Thread waiting in FIFO approach */
    if (err_control != RT_EOK)
    {
        rt_kprintf("init receive message mailbox failed.\n");
        return -1;
    }

    // initializes speed_detection-display_management mailbox
    err_control = rt_mb_init(&mb_speed_display,
                        "mb_speed_display",                     /* Name is mb_speed_display*/
                        &mb_speed_display_pool[0],              /* The memory pool used by the mailbox is mb_pool */
                        sizeof(mb_speed_display_pool) / 4,      /* The number of messages in the mailbox because a message occupies 4 bytes */
                        RT_IPC_FLAG_FIFO);                      /* Thread waiting in FIFO approach */
    if (err_control != RT_EOK)
    {
        rt_kprintf("init speed_detection-display_manager mailbox failed.\n");
        return -1;
    }

    // initializes motor_temperature-display_management mailbox
    err_control = rt_mb_init(&mb_mottemp_display,
                        "mb_mottemp_display",                   /* Name is mb_mottemp_display*/
                        &mb_mottemp_display_pool,               /* The memory pool used by the mailbox is mb_pool */
                        sizeof(mb_mottemp_display_pool) / 4,    /* The number of messages in the mailbox because a message occupies 4 bytes */
                        RT_IPC_FLAG_FIFO);                      /* Thread waiting in FIFO approach */
    if (err_control != RT_EOK)
    {
        rt_kprintf("init motor_temperature-display_manager mailbox failed.\n");
        return -1;
    }

    // initializes battery_temperature-display_management mailbox
    err_control = rt_mb_init(&mb_battemp_display,
                        "mb_battemp_display",                   /* Name is mb_battemp_display*/
                        &mb_battemp_display_pool,               /* The memory pool used by the mailbox is mb_pool */
                        sizeof(mb_battemp_display_pool) / 4,    /* The number of messages in the mailbox because a message occupies 4 bytes */
                        RT_IPC_FLAG_FIFO);                      /* Thread waiting in FIFO approach */
    if (err_control != RT_EOK)
    {
        rt_kprintf("init battery_temperature-display_manager mailbox failed.\n");
        return -1;
    }

    // initializes battery_level-display_management mailbox
    err_control = rt_mb_init(&mb_batlevel_display,
                        "mb_batlevel_display",                  /* Name is mb_batlevel_display*/
                        &mb_batlevel_display_pool,              /* The memory pool used by the mailbox is mb_pool */
                        sizeof(mb_batlevel_display_pool) / 4,   /* The number of messages in the mailbox because a message occupies 4 bytes */
                        RT_IPC_FLAG_FIFO);                      /* Thread waiting in FIFO approach */
    if (err_control != RT_EOK)
    {
        rt_kprintf("init battery_level-display_manager mailbox failed.\n");
        return -1;
    }

    // initializes brake_detection-throttle_detection mailbox
    err_control = rt_mb_init(&mb_brake_throttle,
                        "mb_brake_throttle",                    /* Name is mb_brake_throttle*/
                        &mb_brake_throttle_pool,                /* The memory pool used by the mailbox is mb_pool */
                        sizeof(mb_brake_throttle_pool) / 4,     /* The number of messages in the mailbox because a message occupies 4 bytes */
                        RT_IPC_FLAG_FIFO);                      /* Thread waiting in FIFO approach */
    if (err_control != RT_EOK)
    {
        rt_kprintf("init brake_detection-throttle_detection mailbox failed.\n");
        return -1;
    }

    // initializes speed_detection-throttle_detection mailbox
    err_control = rt_mb_init(&mb_speed_throttle,
                        "mb_speed_throttle",                    /* Name is mb_speed_throttle*/
                        &mb_speed_throttle_pool,                /* The memory pool used by the mailbox is mb_pool */
                        sizeof(mb_speed_throttle_pool) / 4,     /* The number of messages in the mailbox because a message occupies 4 bytes */
                        RT_IPC_FLAG_FIFO);                      /* Thread waiting in FIFO approach */
    if (err_control != RT_EOK)
    {
        rt_kprintf("init speed_detection-throttle_detection mailbox failed.\n");
        return -1;
    }

    return 0;
}
