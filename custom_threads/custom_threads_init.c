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
#include "../custom_threads/thread_functions.h"

/* Thread Functions Prototypes */
//void print_string(void *parameter);
//void receive_message(void *parameter);


struct rt_thread print_string_thread;
static rt_uint8_t print_string_thread_stack[512];

struct rt_thread receive_message_thread;
static rt_uint8_t receive_message_thread_stack[512];

struct rt_thread brake_detection_thread;
static rt_uint8_t brake_detection_thread_stack[512];

struct rt_thread throttle_detection_thread;
static rt_uint8_t throttle_detection_thread_stack[512];

struct rt_thread speed_detection_thread;
static rt_uint8_t speed_detection_thread_stack[512];

struct rt_thread display_management_thread;
static rt_uint8_t display_management_thread_stack[512];

struct rt_thread motor_temperature_thread;
static rt_uint8_t motor_temperature_thread_stack[512];

struct rt_thread battery_temperature_thread;
static rt_uint8_t battery_temperature_thread_stack[512];

struct rt_thread battery_level_thread;
static rt_uint8_t battery_level_thread_stack[512];

struct rt_thread auxiliary_light_management_thread;
static rt_uint8_t auxiliary_light_management_thread_stack[512];

struct rt_thread auxiliary_light_blink_thread;
static rt_uint8_t auxiliary_light_blink_thread_stack[512];


void launch_print_string_thread(void *parameter)
{
    /* Initialize print string thread */
        /* The thread entry is print_string and the parameter is RT_NULL
         * Thread stack is print_string_thread_stack
         * Priority is 5 and time slice is 20 OS Tick
         */
    rt_thread_init(&print_string_thread, "print_string_thread", print_string,
                    RT_NULL, print_string_thread_stack, sizeof(print_string_thread_stack),
                    5, 20);

    rt_thread_startup(&print_string_thread);

    return;
}

void custom_threads_init(void)
{
    /* Initialize print string thread */
        /* The thread entry is print_string and the parameter is RT_NULL
         * Thread stack is print_string_thread_stack
         * Priority is 5 and time slice is 20 OS Tick
         */
    rt_thread_init(&receive_message_thread, "receive_message_thread", receive_message,
                    RT_NULL, receive_message_thread_stack, sizeof(receive_message_thread_stack),
                    10, 20);
    rt_thread_init(&brake_detection_thread, "brake_detection_thread", brake_detection,
                    RT_NULL, brake_detection_thread_stack, sizeof(brake_detection_thread_stack),
                    1, 20);
    rt_thread_init(&throttle_detection_thread, "throttle_detection_thread", throttle_detection,
                    RT_NULL, throttle_detection_thread_stack, sizeof(throttle_detection_thread_stack),
                    2, 20);
    rt_thread_init(&speed_detection_thread, "speed_detection_thread", speed_detection,
                    RT_NULL, speed_detection_thread_stack, sizeof(speed_detection_thread_stack),
                    3, 20);
    rt_thread_init(&display_management_thread, "display_management_thread", display_management,
                    RT_NULL, display_management_thread_stack, sizeof(display_management_thread_stack),
                    4, 20);
    rt_thread_init(&motor_temperature_thread, "motor_temperature_thread", motor_temperature,
                    RT_NULL, motor_temperature_thread_stack, sizeof(motor_temperature_thread_stack),
                    5, 20);
    rt_thread_init(&battery_temperature_thread, "battery_temperature_thread", battery_temperature,
                    RT_NULL, battery_temperature_thread_stack, sizeof(battery_temperature_thread_stack),
                    5, 20);
    rt_thread_init(&battery_level_thread, "battery_level_thread", battery_level,
                    RT_NULL, battery_level_thread_stack, sizeof(battery_level_thread_stack),
                    6, 20);
    rt_thread_init(&auxiliary_light_management_thread, "auxiliary_light_management_thread", auxiliary_light_management,
                    RT_NULL, auxiliary_light_management_thread_stack, sizeof(auxiliary_light_management_thread_stack),
                    6, 20);
    rt_thread_init(&auxiliary_light_blink_thread, "auxiliary_light_blink_thread", auxiliary_light_blink,
                    RT_NULL, auxiliary_light_blink_thread_stack, sizeof(auxiliary_light_blink_thread_stack),
                    6, 20);

    //rt_thread_startup(&receive_message_thread);
    //rt_thread_startup(&brake_detection_thread);
    //rt_thread_startup(&throttle_detection_thread);
    rt_thread_startup(&speed_detection_thread);
    rt_thread_startup(&display_management_thread);
    rt_thread_startup(&motor_temperature_thread);
    rt_thread_startup(&battery_temperature_thread);
    rt_thread_startup(&battery_level_thread);
    rt_thread_startup(&auxiliary_light_management_thread);
    rt_thread_startup(&auxiliary_light_blink_thread);


    return;
}
