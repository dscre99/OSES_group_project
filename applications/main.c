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
#include <stdint.h>
#include <../custom_flash_func/custom_flash_func.h>

/* defined the PIN */
#define LED0_PIN    GET_PIN(A, 5)
#define BUT0_PIN    GET_PIN(C, 13)

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
    // set LED0 pin mode to output
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
