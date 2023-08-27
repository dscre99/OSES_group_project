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
//#include <../custom_flash_func/custom_flash_func.h>
#include <../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_adc.h>
#include "../custom_mailbox/custom_mailbox_init.h"

/* defined the PIN */
#define LED0_PIN    GET_PIN(A, 5)
//#define BUT0_PIN    GET_PIN(C, 13)

ADC_HandleTypeDef hadc1;

int main(void)
{
    /*uint8_t myText[4]={0x01,0x09,0x09,0x08};
    uint8_t myRead[4];
    uint8_t i=0;
    while(1){
        set_sector(6,0x08040000); //SECTOR 6
        write_flash(0,myText,4);
        read_flash(0, myRead,4);

        for(i=0; i<4; i++){
            rt_kprintf("read[%d]=%x\n",i,myRead[i]);
        }
        rt_thread_mdelay(5000);
    }
     */
    int count = 1;
    //int mode = 0;

    // set PIN mode
    rt_pin_mode(BUT_BRAKE, PIN_MODE_INPUT_PULLUP);
    //rt_pin_mode(BUT_THROTTLE, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(BUT_LEFT, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(BUT_RIGHT, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(LED_LEFT, PIN_MODE_OUTPUT);
    rt_pin_mode(LED_RIGHT, PIN_MODE_OUTPUT);
    //rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    //rt_pin_mode(BUT0_PIN, PIN_MODE_INPUT);
    //rt_pin_write(LED0_PIN, PIN_LOW);

    //ADC variables
    rt_uint32_t read_value_speed = 0;
    rt_uint32_t read_value_throttle = 0;
    //ADC initialization
    MX_ADC1_Init(&hadc1);


    while (count++)
    {

        ADC_Select_CH0(&hadc1);
        read_value_speed = get_adc_value(&hadc1);
        HAL_ADC_Stop(&hadc1);
        ADC_Select_CH1(&hadc1);
        read_value_throttle = get_adc_value(&hadc1);
        HAL_ADC_Stop(&hadc1);
        /*
        if (read_value < 22)
        { //uphill road
            mode = 1;
        }
        else if (read_value < 42)
        { //normal road
            mode = 2;
        }
        else
        { //downhill road
            mode = 3;
        }*/

        rt_mb_send(&mb_main_speed, (rt_uint32_t) read_value_speed);
        rt_mb_send(&mb_main_throttle, (rt_uint32_t) read_value_throttle);
        //rt_thread_mdelay(3000);
    }

    return RT_EOK;
}
