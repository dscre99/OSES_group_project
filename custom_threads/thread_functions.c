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
#include <rthw.h>
#include <board.h>
#include <main.h>
#include <cpu_usage.h>
#include <stdint.h>
#include <stdio.h>
#include "../custom_mailbox/custom_mailbox_init.h"
#include <../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_adc.h>

ADC_HandleTypeDef hadc1;

#define LED0_PIN    GET_PIN(A, 5)
#define BUT0_PIN    GET_PIN(C, 13)
#define SPEED_LIMIT 25
#define MAX_THROTTLE 100
#define MAX_SPEED 99
#define MAX_ADC 63
#define ALL_OFF 0
#define ALL_ON 1
#define LEFT 2
#define RIGHT 3
#define BRAKE 4
#define AMBIENT_TEMP 25

void brake_detection(void * parameters){
    int brake_status = 0;

    rt_kprintf("-- brake_detection thread started --\n");

    while(1){
        if (rt_pin_read(BUT_BRAKE) != 1)
        {
            // if pin is low it means user engaged the brakes (button is pressed)
            brake_status = 1;
        }
        else
        {
            brake_status = 0;
        }

        // sends mail to throttle_detection
        rt_mb_send(&mb_brake_throttle, (rt_uint32_t)brake_status);
        // sends mail to speed_detection
        rt_mb_send(&mb_brake_speed, (rt_uint32_t)brake_status);
        // sends mail to auxiliary_light_management
        rt_mb_send(&mb_brake_alman, (rt_uint32_t)brake_status);

        rt_thread_mdelay(25);
    }
    return;
}

void throttle_detection(void * parameters){
    int brake_detected = 0;
    int speed_value = 0;
    int read_value = 0;
    int throttle = 0;

    rt_kprintf("-- throttle_detection thread started --\n");

    while (1)
    {
        // receive messages from throttle_sensor
        while (rt_mb_recv(&mb_main_throttle, (rt_ubase_t *) (&read_value), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }
        throttle=(MAX_THROTTLE*read_value)/MAX_ADC;

        // receive messages from brake_detection
        while (rt_mb_recv(&mb_brake_throttle, (rt_ubase_t *) (&brake_detected), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }
        // receive messages from speed_detection
        while (rt_mb_recv(&mb_speed_throttle, (rt_ubase_t *) (&speed_value), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }

        // throttle management
        if (brake_detected)
        {
            // disable motor in case brake is applied
            throttle = 0;
        }
        else if (speed_value > SPEED_LIMIT)
        {
            // disable motor in case speed limit is reached
            throttle = 0;
        }

        // sends mail to speed_detection
        rt_mb_send(&mb_throttle_display, (rt_uint32_t) throttle);
        // sends mail to motor_temperature
        rt_mb_send(&mb_throttle_mottemp, (rt_uint32_t) throttle);
        // sends mail to battery_temperature
        rt_mb_send(&mb_throttle_battemp, (rt_uint32_t) throttle);

        rt_thread_mdelay(50);
    }

    //load = cpu_load_average();
    //rt_kprintf("cpu load average: %f\n", load);


    return;
}

void speed_detection(void * parameters){
    int read_value = 0;
    int speed = 0;
    rt_kprintf("-- speed_detection thread started --\n");

    while (1)
    {
        // receive messages from speed_sensor
        while (rt_mb_recv(&mb_main_speed, (rt_ubase_t *) (&read_value), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }

        // extracts meaningful speed value from sensor reading
        speed=(MAX_SPEED*read_value)/MAX_ADC;


        // sends speed value to throttle_detection
        rt_mb_send(&mb_speed_throttle, (rt_uint32_t) speed);
        // sends speed value to display_manager
        rt_mb_send(&mb_speed_display, (rt_uint32_t) speed);

        rt_thread_mdelay(50);
    }
    return;
}

void display_management(void * parameters){
    int speed_value = 0;
    int throttle_value = 0;
    int motor_temperature_warning = -1;
    int battery_temperature_warning = -1;
    int battery_level_value = -1;
    int light_blink = 0;

    rt_kprintf("-- display_management thread started --\n");

    while (1)
    {
        // receive messages from speed_detection
        while (rt_mb_recv(&mb_speed_display, (rt_ubase_t *) (&speed_value), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }
        // receive messages from speed_detection
        while (rt_mb_recv(&mb_throttle_display, (rt_ubase_t *) (&throttle_value), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }
        // receive messages from motor_temperature
        while (rt_mb_recv(&mb_mottemp_display, (rt_ubase_t *) (&motor_temperature_warning), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }
        // receive messages from battery_temperature
        while (rt_mb_recv(&mb_battemp_display, (rt_ubase_t *) (&battery_temperature_warning), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }
        // receive messages from battery_level
        while (rt_mb_recv(&mb_batlevel_display, (rt_ubase_t *) (&battery_level_value), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }
        // receive messages from auxiliary_light_managemenet
        while (rt_mb_recv(&mb_alman_display, (rt_ubase_t *) (&light_blink), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }

        // update display information
        rt_kprintf("\nDISPLAY:: | Speed: %2d | Throttle: %3d | Battery: %3d\% |", speed_value, throttle_value, battery_level_value);
        if (light_blink == ALL_ON){
            // auxiliary lights symbol on
            rt_kprintf(" <--> |");
        }else if (light_blink == LEFT){
            // auxiliary left light symbol on
            rt_kprintf(" <-   |");
        }else if (light_blink == RIGHT){
            // auxiliary right light symbol on
            rt_kprintf("   -> |");
        }else{
            // auxiliary lights symbol off
            rt_kprintf("      |");
        }
        if (light_blink == BRAKE) {
            // brake symbol on
            rt_kprintf(" BRAKE! |\n");
        } else {
            // brake symbol off
            rt_kprintf("        |\n");
        }

        // displays warnings if detected
        if (motor_temperature_warning)
        {
            rt_kprintf("WARNING! High MOTOR temperature!\n");
        }
        if (battery_temperature_warning)
        {
            rt_kprintf("WARNING! High BATTERY temperature!\n");
        }
        if (battery_level_value <= 15)
        {
            rt_kprintf("WARNING! Low BATTERY level!\n");
        }

        rt_thread_mdelay(250);
    }
    return;
}

void motor_temperature(void * parameters){
    int temperature_warning = AMBIENT_TEMP;
    rt_uint32_t received_throttle = 0;

    rt_kprintf("-- motor_temperature thread started --\n");

    while (1)
    {
        // receive messages from throttle_detection
        while (rt_mb_recv(&mb_throttle_mottemp, (rt_ubase_t *) (&received_throttle), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }

        // performs emulation of motor temperature based on throttle
        if(received_throttle >= 0.75*MAX_THROTTLE){
            if (temperature_warning < AMBIENT_TEMP+50) {
                temperature_warning += 1;
            }
        } else if (received_throttle <= 0.25*MAX_THROTTLE) {
            if (temperature_warning > AMBIENT_TEMP) {
                temperature_warning -= 1;
            }
        }

        // senses motor temperature warning
        if (temperature_warning >= 50)
        {
            // sends motor temperature warning to display_manager
            rt_mb_send(&mb_mottemp_display, (rt_uint32_t) 1);
        }
        else
        {
            rt_mb_send(&mb_mottemp_display, (rt_uint32_t) 0);
        }

        rt_thread_mdelay(1500);
    }

    return;
}

void battery_temperature(void * parameters){
    int temperature_warning = AMBIENT_TEMP;
    rt_uint32_t received_throttle = 0;

    rt_kprintf("-- battery_temperature thread started --\n");

    while (1)
    {
        // receive messages from throttle_detection
        while (rt_mb_recv(&mb_throttle_battemp, (rt_ubase_t *) (&received_throttle), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }

        // performs emulation of battery temperature based on throttle
        if(received_throttle >= 0.75*MAX_THROTTLE){
            if (temperature_warning < AMBIENT_TEMP+50) {
                temperature_warning += 2;
            }
        } else if (received_throttle <= 0.25*MAX_THROTTLE) {
            if (temperature_warning > AMBIENT_TEMP) {
                temperature_warning -= 2;
            }
        }

        // senses battery temperature warning
        if (temperature_warning >= 45)
        {
            // sends battery temperature warning to display_manager
            rt_mb_send(&mb_battemp_display, (rt_uint32_t) 1);
        }
        else
        {
            rt_mb_send(&mb_battemp_display, (rt_uint32_t) 0);
        }

        rt_thread_mdelay(1500);
    }

    return;
}

void battery_level(void * parameters){
    int battery_level = 101;

    rt_kprintf("-- battery_level thread started --\n");

    while (1)
    {
        /* NOTE: this thread relies purely on emulation */
        // senses battery level
        battery_level -= 1;

        // sends battery level to display_manager
        rt_mb_send(&mb_batlevel_display, (rt_uint32_t) battery_level);

        // DEBUG: restores battery
        if (battery_level == 0)
        {
            battery_level = 101;
        }

        rt_thread_mdelay(1000);
    }
    return;
}

void auxiliary_light_management(void * parameters){
    int display_blink = 0;
    int emergency_but = 0;
    int left_but = 0;
    int right_but = 0;
    int but_press = 0;
    int brake_detected = 0;

    rt_kprintf("-- auxiliary_light_management thread started --\n");

    while (1)
    {
        // receives messages from brake_detection
        while (rt_mb_recv(&mb_brake_alman, (rt_ubase_t *) (&brake_detected), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }

        //set EMERGENCY mode when BUT_EMERGENCY is pressed the first time, disable when pressed the second time
        if (rt_pin_read(BUT_EMERGENCY) != 1 && but_press == 0){
            emergency_but=!emergency_but;
            but_press=1;
        }else if(rt_pin_read(BUT_EMERGENCY) == 1){
            but_press=0;
        }

        //set LEFT LIGHT mode when BUT_LEFT is pressed the first time, disable when pressed the second time
        if (rt_pin_read(BUT_LEFT) != 1 && but_press == 0){
             left_but=!left_but;
             but_press=1;
        }else if(rt_pin_read(BUT_LEFT) == 1){
             but_press=0;
        }

        //set LEFT LIGHT mode when BUT_LEFT is pressed the first time, disable when pressed the second time
        if (rt_pin_read(BUT_RIGHT) != 1 && but_press == 0){
             right_but=!right_but;
             but_press=1;
         }else if(rt_pin_read(BUT_RIGHT) == 1){
             but_press=0;
         }

        //choose the blink mode
        if (emergency_but == 1){
            // EMERGENCY: invert all lights value
            rt_pin_write(LED_RIGHT, !rt_pin_read(LED_RIGHT));
            rt_pin_write(LED_LEFT, !rt_pin_read(LED_LEFT));
            display_blink = !display_blink;
        }else if (brake_detected == 1) {
            // BRAKE: turn on all lights
            rt_pin_write(LED_RIGHT, PIN_HIGH);
            rt_pin_write(LED_LEFT, PIN_HIGH);
            display_blink = BRAKE;
        }else if (left_but == 1) {
            // LEFT LIGHT
            rt_pin_write(LED_RIGHT, PIN_LOW);
            rt_pin_write(LED_LEFT, !rt_pin_read(LED_LEFT));
            display_blink = LEFT;
        }else if (right_but == 1) {
            // RIGHT LIGHT
            rt_pin_write(LED_RIGHT, !rt_pin_read(LED_RIGHT));
            rt_pin_write(LED_LEFT, PIN_LOW);
            display_blink = RIGHT;
        }else {
            // LIGHTS_OFF: turn off all lights
            rt_pin_write(LED_RIGHT, PIN_LOW);
            rt_pin_write(LED_LEFT, PIN_LOW);
            display_blink = ALL_OFF;
        }

        // sends blink status to display_management
        rt_mb_send(&mb_alman_display, (rt_uint32_t) display_blink);

        rt_thread_mdelay(300);
    }
    return;
}
