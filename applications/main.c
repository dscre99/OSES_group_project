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

/* defined the PIN */
#define LED0_PIN    GET_PIN(A, 5)
#define BUT0_PIN    GET_PIN(C, 13)

int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(BUT0_PIN, PIN_MODE_INPUT);
    rt_pin_write(LED0_PIN, PIN_LOW);

    while (count++)
    {
       // rt_pin_write(LED0_PIN, PIN_HIGH);
        //rt_thread_mdelay(500);
        rt_thread_mdelay(50);

        if( rt_pin_read(BUT0_PIN)!=1){
                    rt_pin_write(LED0_PIN, PIN_HIGH);
        } else {
            rt_pin_write(LED0_PIN, PIN_LOW);
        }

    }

    return RT_EOK;
}
