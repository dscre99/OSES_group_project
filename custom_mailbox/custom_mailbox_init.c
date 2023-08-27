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
struct rt_mailbox mb_main_speed; //!!
struct rt_mailbox mb_main_throttle; //!!
struct rt_mailbox mb_speed_display;
struct rt_mailbox mb_throttle_display; //!!
struct rt_mailbox mb_mottemp_display;
struct rt_mailbox mb_battemp_display;
struct rt_mailbox mb_batlevel_display;
struct rt_mailbox mb_brake_throttle;
struct rt_mailbox mb_brake_alman;    //!!
struct rt_mailbox mb_brake_speed;    //!!
struct rt_mailbox mb_throttle_speed; //!!
struct rt_mailbox mb_speed_throttle;
//struct rt_mailbox mb_alman_alblink;
struct rt_mailbox mb_alman_display;   //!!
//struct rt_mailbox mb_alblink_display;

static char mb_pool[128];
static char mb_main_speed_pool[128]; //!!
static char mb_main_throttle_pool[128]; //!!
static char mb_speed_display_pool[128];
static char mb_throttle_display_pool[128]; //!!
static char mb_mottemp_display_pool[16];
static char mb_battemp_display_pool[16];
static char mb_batlevel_display_pool[24];
static char mb_brake_throttle_pool[128];
static char mb_brake_alman_pool[128];   //!!
static char mb_brake_speed_pool[128];   //!!
static char mb_throttle_speed_pool[128];   //!!
static char mb_speed_throttle_pool[128];
//static char mb_alman_alblink_pool[32];
static char mb_alman_display_pool[32];  //!!
//static char mb_alblink_display_pool[32];

int custom_mailbox_init(void)
{
    rt_err_t err_control;

    // initializes main_speed mailbox  !!!!
        err_control = rt_mb_init(&mb_main_speed,
                            "mb_main_speed",                      /* Name is mb*/
                            &mb_main_speed_pool[0],                /* The memory pool used by the mailbox is mb_pool */
                            sizeof(mb_main_speed_pool) / 4,        /* The number of messages in the mailbox because a message occupies 4 bytes */
                            RT_IPC_FLAG_FIFO);          /* Thread waiting in FIFO approach */
        if (err_control != RT_EOK)
        {
            rt_kprintf("init receive message mailbox failed.\n");
            return -1;
        }

        // initializes main_throttle mailbox  !!!!
            err_control = rt_mb_init(&mb_main_throttle,
                                "mb_main_throttle",                      /* Name is mb*/
                                &mb_main_throttle_pool[0],                /* The memory pool used by the mailbox is mb_pool */
                                sizeof(mb_main_throttle_pool) / 4,        /* The number of messages in the mailbox because a message occupies 4 bytes */
                                RT_IPC_FLAG_FIFO);          /* Thread waiting in FIFO approach */
            if (err_control != RT_EOK)
            {
                rt_kprintf("init receive message mailbox failed.\n");
                return -1;
            }

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

    // initializes throttle_detection-display_management mailbox
        err_control = rt_mb_init(&mb_throttle_display,
                            "mb_throttle_display",                     /* Name is mb_speed_display*/
                            &mb_throttle_display_pool[0],              /* The memory pool used by the mailbox is mb_pool */
                            sizeof(mb_throttle_display_pool) / 4,      /* The number of messages in the mailbox because a message occupies 4 bytes */
                            RT_IPC_FLAG_FIFO);                      /* Thread waiting in FIFO approach */
        if (err_control != RT_EOK)
        {
            rt_kprintf("init throttle_detection-display_manager mailbox failed.\n");
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

    // !! initializes brake_detection-throttle_detection mailbox!!
        err_control = rt_mb_init(&mb_brake_alman,
                            "mb_brake_alman",                    /* Name is mb_brake_throttle*/
                            &mb_brake_alman_pool,                /* The memory pool used by the mailbox is mb_pool */
                            sizeof(mb_brake_alman_pool) / 4,     /* The number of messages in the mailbox because a message occupies 4 bytes */
                            RT_IPC_FLAG_FIFO);                      /* Thread waiting in FIFO approach */
        if (err_control != RT_EOK)
        {
            rt_kprintf("init brake_detection-auxiliary_light_management mailbox failed.\n");
            return -1;
        }

    // !! initializes brake_detection-speed_detection mailbox !!
        err_control = rt_mb_init(&mb_brake_speed,
                            "mb_brake_speed",                    /* Name is mb_brake_throttle*/
                            &mb_brake_speed_pool,                /* The memory pool used by the mailbox is mb_pool */
                            sizeof(mb_brake_speed_pool) / 4,     /* The number of messages in the mailbox because a message occupies 4 bytes */
                            RT_IPC_FLAG_FIFO);                      /* Thread waiting in FIFO approach */
        if (err_control != RT_EOK)
        {
            rt_kprintf("init brake_detection-speed_detection mailbox failed.\n");
            return -1;
        }

     // !! initializes throttle_detection-speed_detection mailbox !!
        err_control = rt_mb_init(&mb_throttle_speed,
                            "mb_throttle_speed",                    /* Name is mb_speed_throttle*/
                            &mb_throttle_speed_pool,                /* The memory pool used by the mailbox is mb_pool */
                            sizeof(mb_throttle_speed_pool) / 4,     /* The number of messages in the mailbox because a message occupies 4 bytes */
                            RT_IPC_FLAG_FIFO);                      /* Thread waiting in FIFO approach */
        if (err_control != RT_EOK)
        {
            rt_kprintf("init throttle_detection-speed_detection mailbox failed.\n");
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

    // initializes auxiliary_light_management-auxiliary_light_blink mailbox
    /*err_control = rt_mb_init(&mb_alman_alblink,
                        "mb_alman_alblink",                     // Name is mb_alman_alblink
                        &mb_alman_alblink_pool,                 // The memory pool used by the mailbox is mb_pool
                        sizeof(mb_alman_alblink_pool) / 4,      // The number of messages in the mailbox because a message occupies 4 bytes
                        RT_IPC_FLAG_FIFO);                      // Thread waiting in FIFO approach
    if (err_control != RT_EOK)
    {
        rt_kprintf("init auxiliary_light_management-auxiliary_light_blink mailbox failed.\n");
        return -1;
    }*/

    // !! initializes auxiliary_light_management-display_management mailbox !!
       err_control = rt_mb_init(&mb_alman_display,
                           "mb_alman_display",                  /* Name is mb_alblink_display*/
                           &mb_alman_display_pool,              /* The memory pool used by the mailbox is mb_pool */
                           sizeof(mb_alman_display_pool) / 4,   /* The number of messages in the mailbox because a message occupies 4 bytes */
                           RT_IPC_FLAG_FIFO);                     /* Thread waiting in FIFO approach */
       if (err_control != RT_EOK)
       {
           rt_kprintf("init auxiliary_light_management-display_management mailbox failed.\n");
           return -1;
       }

    // initializes auxiliary_light_blink-display_management mailbox
    /*err_control = rt_mb_init(&mb_alblink_display,
                        "mb_alblink_display",                  // Name is mb_alblink_display
                        &mb_alblink_display_pool,              // The memory pool used by the mailbox is mb_pool
                        sizeof(mb_alblink_display_pool) / 4,   // The number of messages in the mailbox because a message occupies 4 bytes
                        RT_IPC_FLAG_FIFO);                     // Thread waiting in FIFO approach
    if (err_control != RT_EOK)
    {
        rt_kprintf("init auxiliary_light_blink-display_management mailbox failed.\n");
        return -1;
    }*/

    return 0;
}
