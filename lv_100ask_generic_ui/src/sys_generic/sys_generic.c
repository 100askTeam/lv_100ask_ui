/**
 ******************************************************************************
 * @file    sys_generic.c
 * @author  百问科技
 * @version V1.0
 * @date    2024-4-25
 * @brief	sys_generic
 ******************************************************************************
 * Change Logs:
 * Date           Author          Notes
 * 2024-4-25     zhouyuebiao     First version
 ******************************************************************************
 * @attention
 *
 * Copyright (C) 2008-2024 深圳百问网科技有限公司<https://www.100ask.net/>
 * All rights reserved
 *
 * 代码配套的视频教程：
 *      B站：   https://www.bilibili.com/video/BV1WE421K75k
 *      百问网：https://fnwcn.xetslk.com/s/39njGj
 *      淘宝：  https://detail.tmall.com/item.htm?id=779667445604
 *
 * 本程序遵循MIT协议, 请遵循协议！
 * 免责声明: 百问网编写的文档, 仅供学员学习使用, 可以转发或引用(请保留作者信息),禁止用于商业用途！
 * 免责声明: 百问网编写的程序, 仅供学习参考，假如被用于商业用途, 但百问网不承担任何后果！
 *
 * 百问网学习平台   : https://www.100ask.net
 * 百问网交流社区   : https://forums.100ask.net
 * 百问网官方B站    : https://space.bilibili.com/275908810
 * 百问网官方淘宝   : https://100ask.taobao.com
 * 百问网微信公众号 ：百问科技 或 baiwenkeji
 * 联系我们(E-mail):  support@100ask.net 或 fae_100ask@163.com
 *
 *                             版权所有，盗版必究。
 ******************************************************************************
 */


/*********************
 *      INCLUDES
 *********************/
#include "../../lv_100ask_generic_ui.h"

#include "sys_generic.h"

#include "../page_lock_screen/page_lock_screen.h"


/*********************
 *      DEFINES
 *********************/
LV_IMG_DECLARE(img_lv_100ask_demo_logo);

typedef struct {
    int16_t sound;  // 0: close
}lv_100ask_sys_generic_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void boot_animation_clear_timer(lv_timer_t * timer);
static void click_check_timer(lv_timer_t * timer);


/**********************
 *  STATIC VARIABLES
 **********************/
static lv_100ask_sys_generic_data_t g_lv_100ask_sys_generic_data;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_100ask_boot_animation(uint32_t delay)
{
    lv_obj_t * logo_cont = lv_image_create(lv_layer_sys());
    lv_obj_remove_style_all(logo_cont);
    lv_obj_set_style_bg_color(logo_cont, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(logo_cont, LV_OPA_COVER, 0);
    lv_obj_set_size(logo_cont, lv_pct(100), lv_pct(100));

    lv_obj_t * logo = lv_image_create(logo_cont);
    lv_img_set_src(logo, &img_lv_100ask_demo_logo);
    //lv_image_set_scale(logo, 512);
    lv_obj_align(logo, LV_ALIGN_CENTER, 0, -50);

    /*Animate in the content after the intro time*/
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_path_cb(&a, lv_anim_path_bounce);
    lv_anim_set_path_cb(&a, lv_anim_path_overshoot);
    lv_anim_set_var(&a, logo);
    lv_anim_set_time(&a, delay);
    lv_anim_set_delay(&a, 0);
    lv_anim_set_values(&a, 0, 128);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) lv_image_set_scale);
	lv_anim_set_completed_cb(&a, lv_obj_delete_anim_completed_cb);
    lv_anim_start(&a);

    /* Create an intro from a label */
    lv_obj_t * title = lv_label_create(logo_cont);
    //lv_label_set_text(title, "100ASK LVGL DEMO\nhttps://www.100ask.net\nhttp:/lvgl.100ask.net");
	lv_label_set_text(title, "100ASK LVGL DEMO");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(title, 8, LV_STATE_DEFAULT);
    lv_obj_align_to(title, logo, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    lv_obj_fade_out(title, 0, delay);
    lv_obj_fade_out(logo, 0, delay);
    lv_obj_fade_out(logo_cont, 0, delay);

    lv_timer_t * timer = lv_timer_create(boot_animation_clear_timer, delay+100, logo_cont);
    lv_timer_set_repeat_count(timer, 1);
}

void sys_generic_click_check_init(void)
{
    g_lv_100ask_sys_generic_data.sound = 1;
    lv_timer_t * timer = lv_timer_create(click_check_timer, LV_DEF_REFR_PERIOD, NULL);
}

void sys_generic_set_sound(int16_t sound)
{
    g_lv_100ask_sys_generic_data.sound = sound;
}


/**********************
 *  STATIC FUNCTIONS
 **********************/

static void click_check_timer(lv_timer_t * timer)
{
    static uint8_t buzzer_state = 0;

    lv_indev_t * indev = lv_indev_get_next(NULL);
    if(indev == NULL)  return;

    lv_indev_state_t indev_state = lv_indev_get_state(indev);

    if(indev_state == LV_INDEV_STATE_PRESSED)
    {
#if LV_100ASK_GENERIC_UI_HAS_LOCK_SCREEN
        set_page_lock_screen_reset();
#endif
    }
}

static void boot_animation_clear_timer(lv_timer_t * timer)
{
    lv_obj_t * logo_cont = lv_timer_get_user_data(timer);
    lv_obj_delete(logo_cont);
}
