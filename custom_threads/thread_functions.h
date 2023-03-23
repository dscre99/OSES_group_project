/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-16     dscre       the first version
 */
#ifndef CUSTOM_THREADS_THREAD_FUNCTIONS_H_
#define CUSTOM_THREADS_THREAD_FUNCTIONS_H_

// Thread functions prototypes
void print_string(void *parameter);
void receive_message(void *parameter);
void brake_detection(void * parameters);
void throttle_detection(void * parameters);
void speed_detection(void * parameters);
void display_management(void * parameters);
void motor_temperature(void * parameters);
void battery_temperature(void * parameters);
void battery_level(void * parameters);
void auxiliary_light_management(void * parameters);
void auxiliary_light_blink(void * parameters);


#endif /* CUSTOM_THREADS_THREAD_FUNCTIONS_H_ */
