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

static struct rt_mailbox mb;

static char mb_pool[128];

int custom_mailbox_init(void)
{
    rt_err_t err_control;
    err_control = rt_mb_init(&mb,
                        "mb",                      /* Name is mb*/
                        &mb_pool[0],                /* The memory pool used by the mailbox is mb_pool */
                        sizeof(mb_pool) / 4,        /* The number of messages in the mailbox because a message occupies 4 bytes */
                        RT_IPC_FLAG_FIFO);          /* Thread waiting in FIFO approach */
    if (err_control != RT_EOK)
    {
        rt_kprintf("init mailbox failed.\n");
        return -1;
    }
    return 0;
}