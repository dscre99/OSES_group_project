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
#include "../custom_mailbox/custom_mailbox_init.h"
#include <../libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_adc.h>

ADC_HandleTypeDef hadc1;

#define LED0_PIN    GET_PIN(A, 5)
#define BUT0_PIN    GET_PIN(C, 13)
#define SPEED_LIMIT 25
#define MAX_THROTTLE 100
#define MAX_SPEED 99
#define MAX_ADC 63
//#define OFF 0
//#define BRAKE 1
//#define LEFT 2
//#define RIGHT 3
//#define EMERGENCY 4
#define AMBIENT_TEMP 25

//#define BUT_THROTTLE    GET_PIN(A, 10)
//#define BUT_BRAKE       GET_PIN(B, 5)
//#define LED_RIGHT      GET_PIN(A, 8)
//#define LED_LEFT       GET_PIN(A, 9)

void print_string(void *parameter)
{
    rt_kprintf("\nThread 'print_string' activated\n");

    rt_kprintf("\nTHIS IS A TEST STRING\n");

    rt_kprintf("\nThread 'print_string' terminating...\n");
}

void receive_message(void *parameter)
{
    int received_val = 0;

    rt_kprintf("\n---- receive_message_thread started ----\n\n");

    // to be completed
    while(1)
    {

        /* Receive messages from the message queue */
        if (rt_mb_recv(&mb, (rt_ubase_t *)(&received_val), RT_WAITING_FOREVER) == RT_EOK)
        {
            rt_kprintf("\rreceive_message: recv msg from msg queue, the content:%d ", received_val);
        }
        rt_thread_mdelay(5);
    }
}

void brake_detection(void * parameters){
    //rt_pin_mode(BUT_BRAKE, PIN_MODE_INPUT_PULLUP);

    while(1){
        if (rt_pin_read(BUT_BRAKE) != 1)
        {
            // if pin is low it means user engaged the brakes
            //      (button is pressed)
            // sends mail to throttle_detection
            rt_mb_send(&mb_brake_throttle, (rt_uint32_t) 1);
            // sends mail to speed_detection
            rt_mb_send(&mb_brake_speed, (rt_uint32_t) 1);
            // sends mail to auxiliary_light_management
            rt_mb_send(&mb_brake_alman, (rt_uint32_t) 1);
            //rt_kprintf("brake_detection: 1\n");
        }
        else
        {
            // sends mail to throttle_detection
            rt_mb_send(&mb_brake_throttle, (rt_uint32_t) 0);
            // sends mail to speed_detection
            rt_mb_send(&mb_brake_speed, (rt_uint32_t) 0);
            // sends mail to auxiliary_light_management
            rt_mb_send(&mb_brake_alman, (rt_uint32_t) 0);
            //rt_kprintf("brake_detection: 0\n");
        }

        rt_thread_mdelay(25);
    }
    return;
}

void throttle_detection(void * parameters){
    //rt_pin_mode(BUT_THROTTLE, PIN_MODE_INPUT_PULLUP);

    int brake_detected = 0;
    int speed_value = 0;
    int read_value = 0;
    int throttle = 0;

    while (1)
    {
        //if (rt_pin_read(BUT_THROTTLE) != 1)
        //{
            //rt_kprintf("throttle_detection\n");

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

            // TO BE COMPLETED
            if (brake_detected)
            {
                // sends mail to speed_detection
                rt_mb_send(&mb_throttle_display, (rt_uint32_t) 0);

            }
            else if (speed_value > SPEED_LIMIT)
            {
                // disable motor
                rt_mb_send(&mb_throttle_display, (rt_uint32_t) 0);
            }
            else
            {
                // sends mail to speed_detection
                rt_mb_send(&mb_throttle_display, (rt_uint32_t) throttle);
            }
        //}
        //else{
            // sends mail to speed_detection
            //rt_mb_send(&mb_throttle_speed, (rt_uint32_t) 0);
        //}

        rt_thread_mdelay(50);
    }
    return;
}

void speed_detection(void * parameters){
    int read_value = 0;
    int speed = 0;
    //int throttle = 0;
    int brake = 0;
    //int mode = 0;
    while (1)
    {
        /*while (rt_mb_recv(&mb_throttle_speed, (rt_ubase_t *) (&throttle), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }*/
        while (rt_mb_recv(&mb_brake_speed, (rt_ubase_t *) (&brake), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }
        while (rt_mb_recv(&mb_main_speed, (rt_ubase_t *) (&read_value), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }
        speed=(MAX_SPEED*read_value)/MAX_ADC;
        /*
        if(brake == 1){
            speed = speed - 5;
        }else if(throttle == 1){
            speed = speed +3;
        }*/

        /*if (mode == 1)
        {   //uphill road
            speed = speed - 2;
            // brake light goes off
            //rt_pin_write(LED0_PIN, PIN_LOW);
        }
        else if (mode == 2)
        {   //normal road
            speed = speed - 1;
            //rt_pin_write(LED0_PIN, PIN_HIGH);
        }
        else if (mode == 3)
        {   //downhill road
            speed = speed + 2;
            // brake light goes off
            //rt_pin_write(LED0_PIN, PIN_LOW);
        }

        if (speed < 0)
        {
            speed = 0;
        }*/
        //mode = 0;
        // sends speed value to throttle_detection
        rt_mb_send(&mb_speed_throttle, (rt_uint32_t) speed);
        // sends speed value to display_manager
        rt_mb_send(&mb_speed_display, (rt_uint32_t) speed);
        rt_mb_send(&mb_speed_battemp, (rt_uint32_t) speed);

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

    while (1)
    {
        //rt_kprintf("display_management\n");

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
        // receive messages from auxiliary_light_blink
        //while (rt_mb_recv(&mb_alblink_display, (rt_ubase_t *) (&light_blink), RT_WAITING_NO) == RT_EOK)
        //{
            // do nothing, simply receive all messages
        //}
        // receive messages from auxiliary_light_managemenet
        while (rt_mb_recv(&mb_alman_display, (rt_ubase_t *) (&light_blink), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }

        // update display information
        rt_kprintf("\nDISPLAY:: | Speed: %2d | Throttle: %3d | Battery: %3d\% |", speed_value, throttle_value, battery_level_value);
        if (light_blink==1){
            // auxiliary lights symbol on
            rt_kprintf(" <--> |\n");
        }else if (light_blink==2){
            // auxiliary left light symbol on
            rt_kprintf(" <-   |\n");
        }else if (light_blink==3){
            // auxiliary right light symbol on
            rt_kprintf("   -> |\n");
        }else{
            // auxiliary lights symbol off
            rt_kprintf("      |\n");
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
    int temperature_warning = 0;

    while (1)
    {
        //rt_kprintf("motor_temperature\n");

        // senses motor temperature warning
        if (temperature_warning == 20) //temperature warning about every 30 sec
        {
            temperature_warning = 0;
            // sends motor temperature warning to display_manager
            rt_mb_send(&mb_mottemp_display, (rt_uint32_t) 1);
        }
        else
        {
            temperature_warning += 1;
            rt_mb_send(&mb_mottemp_display, (rt_uint32_t) 0);
        }

        rt_thread_mdelay(1500);
    }
    return;
}

void battery_temperature(void * parameters){
    int temperature_warning = AMBIENT_TEMP;
    rt_uint32_t received_throttle = 0;

    while (1)
    {
        //rt_kprintf("battery_temperature\n");
        while (rt_mb_recv(&mb_throttle_battemp, (rt_ubase_t *) (&received_throttle), RT_WAITING_NO) == RT_EOK)
        {
            // do nothing, simply receive all messages
        }

        // performs emulation of battery temperature based on throttle
        if(received_throttle >= 0.75*MAX_THROTTLE){
            temperature_warning += 2;
        } else if (received_throttle <= 0.25*MAX_THROTTLE) {
            if (temperature_warning > AMBIENT_TEMP) {
                temperature_warning -= 2;
            }
        }

        // senses battery temperature warning
        if (temperature_warning >= 45) //temperature warning about every 30 sec
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

    while (1)
    {
        //rt_kprintf("battery_level\n");

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

    while (1)
    {
        //rt_kprintf("auxiliary_light_management\n");
        // receive brake press from brake_detection
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
            rt_mb_send(&mb_alman_display, (rt_uint32_t) display_blink);
        }else if (brake_detected == 1) {
            // BRAKE: turn on all lights
            rt_pin_write(LED_RIGHT, PIN_HIGH);
            rt_pin_write(LED_LEFT, PIN_HIGH);
            display_blink = 1;
            rt_mb_send(&mb_alman_display, (rt_uint32_t) display_blink);
        }else if (left_but == 1) {
            // LEFT LIGHT
            rt_pin_write(LED_RIGHT, PIN_LOW);
            rt_pin_write(LED_LEFT, !rt_pin_read(LED_LEFT));
            display_blink = 2;
            rt_mb_send(&mb_alman_display, (rt_uint32_t) display_blink);
        }else if (right_but == 1) {
            // RIGHT LIGHT
            rt_pin_write(LED_RIGHT, !rt_pin_read(LED_RIGHT));
            rt_pin_write(LED_LEFT, PIN_LOW);
            display_blink = 3;
            rt_mb_send(&mb_alman_display, (rt_uint32_t) display_blink);
        }else {
            // LIGHTS_OFF: turn off all lights
            rt_pin_write(LED_RIGHT, PIN_LOW);
            rt_pin_write(LED_LEFT, PIN_LOW);
            display_blink = 0;
            rt_mb_send(&mb_alman_display, (rt_uint32_t) 0);
        }

        // sends light active to blinker
        //if (light_active)
        //{
        //    rt_mb_send(&mb_alman_alblink, (rt_uint32_t) 1);
        //}
        //else
        //{
        //    rt_mb_send(&mb_alman_alblink, (rt_uint32_t) 0);
        //}


        rt_thread_mdelay(300);
    }
    return;
}

/*void auxiliary_light_blink(void * parameters){
    int blink_mode = -1;
    int display_blink = 0;
    int emergency_but = 0;
    int but_press = 0;

    while (1)
    {
        //rt_kprintf("auxiliary_light_blink\n");

        // receive blink mode from auxiliary_light_management
        //while (rt_mb_recv(&mb_alman_alblink, (rt_ubase_t *) (&blink_mode), RT_WAITING_NO) == RT_EOK)
        //{
            // do nothing, simply receive all messages
        //}
        if (rt_pin_read(BUT_EMERGENCY) != 1 && but_press == 0){
            emergency_but=!emergency_but;
            but_press=1;
        }else if(rt_pin_read(BUT_EMERGENCY) == 1){
            but_press=0;
        }

        if (emergency_but == 1){
            blink_mode=EMERGENCY;
        }else if (rt_pin_read(BUT_BRAKE) != 1) {
            blink_mode=BRAKE;
        }else {
            blink_mode=OFF;
        }

        switch (blink_mode) {
            case OFF:
                // turn off all lights
                rt_pin_write(LED_RIGHT, PIN_LOW);
                rt_pin_write(LED_LEFT, PIN_LOW);
                display_blink = 0;
                break;

            case BRAKE:
                // turn on all lights
                rt_pin_write(LED_RIGHT, PIN_HIGH);
                rt_pin_write(LED_LEFT, PIN_HIGH);
                display_blink = 1;
                break;

            case LEFT:
                // invert left light status
                // other lights off
                rt_pin_write(LED_RIGHT, PIN_LOW);
                rt_pin_write(LED_LEFT, !rt_pin_read(LED_LEFT));
                display_blink = !display_blink;
                break;

            case RIGHT:
                // invert right light status
                // other lights off
                rt_pin_write(LED_RIGHT, !rt_pin_read(LED_RIGHT));
                rt_pin_write(LED_LEFT, PIN_LOW);
                display_blink = !display_blink;
                break;

            case EMERGENCY:
                // invert all lights value
                rt_pin_write(LED_RIGHT, !rt_pin_read(LED_RIGHT));
                rt_pin_write(LED_LEFT, !rt_pin_read(LED_LEFT));
                display_blink = !display_blink;
                break;

            default:
                // all lights off
                rt_pin_write(LED_RIGHT, PIN_LOW);
                rt_pin_write(LED_LEFT, PIN_LOW);
                display_blink = 0;
                break;
        }
        rt_mb_send(&mb_alblink_display, (rt_uint32_t) display_blink);


        rt_thread_mdelay(300);
    }
    return;
}*/

