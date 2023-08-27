/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-23     david       the first version
 */
#ifndef CUSTOM_MAILBOX_CUSTOM_MAILBOX_INIT_H_
#define CUSTOM_MAILBOX_CUSTOM_MAILBOX_INIT_H_

extern struct rt_mailbox mb;
struct rt_mailbox mb_main_speed; //!!
struct rt_mailbox mb_main_throttle; //!!
extern struct rt_mailbox mb_speed_display;
extern struct rt_mailbox mb_throttle_display; //!!
extern struct rt_mailbox mb_mottemp_display;
extern struct rt_mailbox mb_battemp_display;
extern struct rt_mailbox mb_batlevel_display;
extern struct rt_mailbox mb_brake_throttle;
extern struct rt_mailbox mb_brake_alman;    //!!
extern struct rt_mailbox mb_brake_speed;    //!!
extern struct rt_mailbox mb_throttle_speed; //!!
extern struct rt_mailbox mb_speed_throttle;
//extern struct rt_mailbox mb_alman_alblink;
extern struct rt_mailbox mb_alman_display; //!!
//extern struct rt_mailbox mb_alblink_display;

int custom_mailbox_init(void);

#endif /* CUSTOM_MAILBOX_CUSTOM_MAILBOX_INIT_H_ */
