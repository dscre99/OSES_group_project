/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <main.h>
#include <stdint.h>
#include <stdio.h>
#include <cpu_usage.h>
#include <../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_adc.h>
#include "../custom_mailbox/custom_mailbox_init.h"

/* defined the PIN */
#define LED0_PIN    GET_PIN(A, 5)

ADC_HandleTypeDef hadc1;

int main(void)
{
    int count = 1;

    // set PIN mode
    rt_pin_mode(BUT_BRAKE, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(BUT_LEFT, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(BUT_RIGHT, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(LED_LEFT, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_RIGHT, PIN_MODE_OUTPUT);

    //ADC variables
    rt_uint32_t read_value_speed = 0;
    rt_uint32_t read_value_throttle = 0;
    //ADC initialization
    MX_ADC1_Init(&hadc1);
    float load;
    cpu_usage_init();

    while (count++)
    {

        load = cpu_load_average();
        printf("CPU load average: %2.2f%%\n",load);

        //ADC sampling
        ADC_Select_CH0(&hadc1);
        read_value_speed = get_adc_value(&hadc1);
        HAL_ADC_Stop(&hadc1);
        ADC_Select_CH1(&hadc1);
        read_value_throttle = get_adc_value(&hadc1);
        HAL_ADC_Stop(&hadc1);


        rt_mb_send(&mb_main_speed, (rt_uint32_t) read_value_speed);
        rt_mb_send(&mb_main_throttle, (rt_uint32_t) read_value_throttle);
        rt_thread_mdelay(250);
    }

    return RT_EOK;
}
