/**
 ******************************************************************************
 * @file    page_drop_down.c
 * @author  百问科技
 * @version V1.0
 * @date    2024-8-11
 * @brief	page_drop_down
 ******************************************************************************
 * Change Logs:
 * Date           Author          Notes
 * 2024-8-11     zhouyuebiao     First version
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


#include "page_drop_down.h"

#if LV_100ASK_GENERIC_UI_DshanMCUH7R_NoRTOS
#include "driver_lcd_backlight.h"
#include "driver_passive_buzzer.h"
#include "driver_ws28xx.h"
#include "driver_ir_receiver.h"
#include "driver_ec11.h"
#include "driver_aht20.h"

#elif LV_100ASK_GENERIC_UI_USE_SIMULATOR

#endif
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void style_init(void);
static void slider_set_brightness_event_handler(lv_event_t * e);
static void slider_hit_test_event_handler(lv_event_t * e);
static void btn_event_handler(lv_event_t * e);
static void cont_sub_ext_draw_event_handler(lv_event_t * e);

static void opt_passive_buzzer(uint16_t val);
static void opt_ws28xx(uint16_t val);
static void opt_backlight(uint16_t val);


/**********************
 *  STATIC VARIABLES
 **********************/
static lv_style_t style_slider_part_main;
static lv_style_t style_slider_part_knob;
static lv_style_t style_slider_part_knob_pr;
static lv_style_t style_slider_part_indicator;
static lv_style_t style_slider_label;

static lv_style_t style_btn_default;
static lv_style_t style_btn_check;

typedef void(*hw_opt_fun)(uint16_t);

static hw_opt_fun lv_hw_opt_fun[] = {
    opt_passive_buzzer,
    opt_ws28xx,
    opt_backlight
};


/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_obj_t * page_drop_down_init(void)
{
    style_init();

    //////////////////////////////////////////////////////////
    lv_obj_t * cont = lv_obj_create(lv_layer_top());
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xe1e1e1), 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_90, 0);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_top(cont, 0, 0);
    lv_obj_set_style_pad_left(cont, 20, 0);
    lv_obj_set_style_pad_right(cont, 20, 0);
    lv_obj_set_style_pad_row(cont, 20, 0);
    lv_obj_set_style_clip_corner(cont, true, 0);
    //lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_START);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

#if LV_100ASK_GENERIC_UI_SCREEN_SIZE_320X480
lv_obj_set_y(cont, -480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_480X480
lv_obj_set_y(cont, -480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_1024X600
lv_obj_set_y(cont, -600);
#endif
   

    //////////////////////////////////////////////////////////
    lv_obj_t * cont_sub;
    lv_obj_t * btn;
    lv_obj_t * label;

    ////////////////////////////////////////////////////////// 0
    cont_sub = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_sub);
    lv_obj_set_size(cont_sub, 390, 10);
    lv_obj_set_style_bg_opa(cont_sub, LV_OPA_TRANSP, 0);

    ////////////////////////////////////////////////////////// 1
    cont_sub = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_sub);
    lv_obj_set_size(cont_sub, 180, 180);
    lv_obj_set_style_bg_color(cont_sub, lv_color_hex(0x686868), 0);
    lv_obj_set_style_bg_opa(cont_sub, LV_OPA_90, 0);
    lv_obj_set_style_radius(cont_sub, 16, 0);
    lv_obj_set_style_pad_all(cont_sub, 12, 0);
    lv_obj_set_style_pad_row(cont_sub, 18, 0);
    lv_obj_set_flex_flow(cont_sub, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont_sub, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    //
    btn = lv_button_create(cont_sub);
    lv_obj_add_style(btn, &style_btn_default, 0);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_size(btn, lv_pct(42), lv_pct(42));
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x898989), 0);
    lv_obj_set_style_bg_color(btn,lv_palette_main(LV_PALETTE_ORANGE), LV_STATE_CHECKED);
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_POWER);
    lv_obj_center(label);

    //
    btn = lv_button_create(cont_sub);
    lv_obj_add_style(btn, &style_btn_default, 0);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_size(btn, lv_pct(42), lv_pct(42));
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x898989), 0);
    lv_obj_set_style_bg_color(btn,lv_palette_main(LV_PALETTE_GREEN), LV_STATE_CHECKED);
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_GPS);
    lv_obj_center(label);

    //
    btn = lv_button_create(cont_sub);
    lv_obj_add_style(btn, &style_btn_default, 0);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_size(btn, lv_pct(42), lv_pct(42));
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x898989), 0);
    lv_obj_set_style_bg_color(btn,lv_palette_main(LV_PALETTE_BLUE), LV_STATE_CHECKED);
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_WIFI);
    lv_obj_center(label);

    //
    btn = lv_button_create(cont_sub);
    lv_obj_add_style(btn, &style_btn_default, 0);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_size(btn, lv_pct(42), lv_pct(42));
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x898989), 0);
    lv_obj_set_style_bg_color(btn,lv_palette_main(LV_PALETTE_BLUE), LV_STATE_CHECKED);
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_BLUETOOTH);
    lv_obj_center(label);

    ////////////////////////////////////////////////////////// 2
    cont_sub = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_sub);
    lv_obj_set_size(cont_sub, 180, 180);
    lv_obj_set_style_bg_color(cont_sub, lv_color_hex(0x686868), 0);
    lv_obj_set_style_bg_opa(cont_sub, LV_OPA_90, 0);
    lv_obj_set_style_radius(cont_sub, 16, 0);
    lv_obj_set_style_pad_all(cont_sub, 8, 0);
    //lv_obj_set_flex_flow(cont_sub, LV_FLEX_FLOW_ROW_WRAP);
    //lv_obj_set_flex_align(cont_sub, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    label = lv_label_create(cont_sub);
    lv_label_set_text(label, "Not Playing");
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -10);

    lv_obj_t * cont_sub_tmp = lv_obj_create(cont_sub);
    lv_obj_remove_style_all(cont_sub_tmp);
    lv_obj_set_size(cont_sub_tmp, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(cont_sub_tmp, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(cont_sub_tmp, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_sub_tmp, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_text_font(cont_sub_tmp, &lv_font_montserrat_20, 0);

    lv_obj_align(cont_sub_tmp, LV_ALIGN_BOTTOM_MID, 0, -20);

    btn = lv_button_create(cont_sub_tmp);
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    label = lv_label_create(btn);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_label_set_text(label, LV_SYMBOL_PREV);
    lv_obj_center(label);

    btn = lv_button_create(cont_sub_tmp);
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);
    label = lv_label_create(btn);
    lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text(label, LV_SYMBOL_PLAY);
    lv_obj_center(label);

    btn = lv_button_create(cont_sub_tmp);
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);
    label = lv_label_create(btn);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_label_set_text(label, LV_SYMBOL_NEXT);
    lv_obj_center(label);


    ////////////////////////////////////////////////////////// 3
    cont_sub = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_sub);
    lv_obj_set_size(cont_sub, 180, 180);
    lv_obj_set_style_bg_opa(cont_sub, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(cont_sub, 0, 0);
    lv_obj_set_style_pad_row(cont_sub, 18, 0);
    lv_obj_set_flex_flow(cont_sub, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont_sub, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    //lv_obj_add_event_cb(cont_sub, cont_sub_ext_draw_event_handler, LV_EVENT_REFR_EXT_DRAW_SIZE, NULL);
    //lv_obj_refresh_ext_draw_size(cont_sub);

    //
    btn = lv_button_create(cont_sub);
    lv_obj_add_style(btn, &style_btn_default, 0);
    lv_obj_add_style(btn, &style_btn_check, LV_STATE_CHECKED);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_size(btn, lv_pct(44), lv_pct(44));
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_VALUE_CHANGED, lv_hw_opt_fun[0]);

    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_BELL);
    lv_obj_center(label);

    //
    btn = lv_button_create(cont_sub);
    lv_obj_add_style(btn, &style_btn_default, 0);
    lv_obj_add_style(btn, &style_btn_check, LV_STATE_CHECKED);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    //lv_obj_add_flag(btn, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_set_size(btn, lv_pct(44), lv_pct(44));
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_VALUE_CHANGED, lv_hw_opt_fun[1]);

    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_CHARGE);
    lv_obj_center(label);

    //
    btn = lv_button_create(cont_sub);
    lv_obj_add_style(btn, &style_btn_default, 0);
    lv_obj_add_style(btn, &style_btn_check, LV_STATE_CHECKED);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_refresh_ext_draw_size(btn);
    lv_obj_set_size(btn, lv_pct(44), lv_pct(44));
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_HOME);
    lv_obj_center(label);

    //
    btn = lv_button_create(cont_sub);
    lv_obj_add_style(btn, &style_btn_default, 0);
    lv_obj_add_style(btn, &style_btn_check, LV_STATE_CHECKED);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
    lv_obj_refresh_ext_draw_size(btn);
    lv_obj_set_size(btn, lv_pct(44), lv_pct(44));
    lv_obj_center(btn);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_SETTINGS);
    lv_obj_center(label);



    ////////////////////////////////////////////////////////// 4
    cont_sub = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_sub);
    lv_obj_set_size(cont_sub, 180, 180);
    //lv_obj_set_style_bg_color(cont_sub, lv_color_hex(0x686868), 0);
    lv_obj_set_style_bg_opa(cont_sub, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont_sub, 16, 0);
    lv_obj_set_style_pad_row(cont_sub, 8, 0);
    lv_obj_set_flex_flow(cont_sub, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont_sub, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * slider[2];
    for(int i = 0; i < 2; i++)
    {
        slider[i] = lv_slider_create(cont_sub);
        lv_slider_set_range(slider[i], LV_100ASK_GENERIC_UI_LCD_MIN_BACKLIGHT, LV_100ASK_GENERIC_UI_LCD_MAX_BACKLIGHT);
        lv_slider_set_value(slider[i], 1000, LV_ANIM_OFF);
        lv_obj_set_size(slider[i], lv_pct(40), lv_pct(100));
        lv_obj_align(slider[i], LV_ALIGN_LEFT_MID, (70 * i) + 30, 0);
        lv_obj_add_event_cb(slider[i], slider_set_brightness_event_handler, LV_EVENT_VALUE_CHANGED, lv_hw_opt_fun[2]);
        lv_obj_send_event(slider[i], LV_EVENT_VALUE_CHANGED, NULL);
        //lv_obj_add_event_cb(slider[i], slider_hit_test_event_handler, LV_EVENT_HIT_TEST, NULL);

        label = lv_label_create(slider[i]);
        lv_obj_add_style(label, &style_slider_label, LV_PART_MAIN);
        if(i == 0)  lv_label_set_text(label, LV_SYMBOL_EYE_OPEN);
        else        lv_label_set_text(label, LV_SYMBOL_VOLUME_MAX);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 45);

        lv_obj_add_style(slider[i], &style_slider_part_main, LV_PART_MAIN);
        lv_obj_add_style(slider[i], &style_slider_part_indicator, LV_PART_INDICATOR);
        lv_obj_add_style(slider[i], &style_slider_part_knob, LV_PART_KNOB);
        lv_obj_add_style(slider[i], &style_slider_part_knob_pr, LV_PART_KNOB | LV_STATE_PRESSED);
    }

    ////////////////////////////////////////////////////////// 5
#if 1
    cont_sub = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_sub);
#if LV_100ASK_GENERIC_UI_SCREEN_SIZE_320X480
    lv_obj_set_size(cont_sub, 280, LV_SIZE_CONTENT);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_480X480
    lv_obj_set_size(cont_sub, 390, LV_SIZE_CONTENT);
#endif
    lv_obj_set_style_bg_color(cont_sub, lv_color_hex(0x686868), 0);
    lv_obj_set_style_bg_opa(cont_sub, LV_OPA_90, 0);
    lv_obj_set_style_radius(cont_sub, 16, 0);
    lv_obj_set_style_pad_all(cont_sub, 12, 0);

    label = lv_label_create(cont_sub);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, "Docs:  https://lvgl.100ask.net\nForum:  https://forums.100ask.net");
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);
#endif

    ///////
    lv_obj_send_event(cont, LV_EVENT_SCROLL, NULL);
    /*Be sure the fist button is in the middle*/
    lv_obj_scroll_to_view(lv_obj_get_child(cont, 0), LV_ANIM_OFF);
    return cont;
}


/**********************
 *  STATIC FUNCTIONS
 **********************/

static void style_init(void)
{
    ///////////////////////////////
    lv_style_init(&style_btn_default);
    lv_style_set_radius(&style_btn_default, 20);
    lv_style_set_shadow_width(&style_btn_default, 0);
    lv_style_set_shadow_offset_y(&style_btn_default, 0);
    lv_style_set_bg_color(&style_btn_default, lv_color_hex(0x686868));
    lv_style_set_text_font(&style_btn_default, &lv_font_montserrat_28);

    lv_style_init(&style_btn_check);
    lv_style_set_text_color(&style_btn_check, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_color(&style_btn_check, lv_color_hex(0xffffff));

    ///////////////////////////////
    lv_style_init(&style_slider_part_main);
    lv_style_set_radius(&style_slider_part_main, 16);
    lv_style_set_bg_color(&style_slider_part_main, lv_color_hex(0x686868));
    lv_style_set_bg_opa(&style_slider_part_main, LV_OPA_COVER);

    lv_style_init(&style_slider_part_indicator);
    lv_style_set_radius(&style_slider_part_indicator, 16);
    lv_style_set_bg_color(&style_slider_part_indicator, lv_color_hex(0xffffff));

    lv_style_init(&style_slider_part_knob);
    //lv_style_set_opa(&style_slider_part_knob, LV_OPA_TRANSP);
    //lv_style_set_opa(&style_slider_part_knob, LV_OPA_TRANSP);
    lv_style_set_pad_all(&style_slider_part_knob, 0);
    lv_style_set_pad_top(&style_slider_part_knob, -34);
    lv_style_set_pad_bottom(&style_slider_part_knob, -23);
    lv_style_set_radius(&style_slider_part_knob, 0);
    lv_style_set_bg_color(&style_slider_part_knob, lv_color_hex(0xffffff));

    lv_style_init(&style_slider_part_knob_pr);
    lv_style_set_transform_width(&style_slider_part_knob_pr, 0);
    lv_style_set_transform_height(&style_slider_part_knob_pr, 0);

    lv_style_init(&style_slider_label);
    lv_style_set_text_font(&style_slider_label, &lv_font_montserrat_30);
}

static void slider_set_brightness_event_handler(lv_event_t * e)
{
    int32_t slider_value = 0;
    lv_obj_t *slider = lv_event_get_current_target(e);
    hw_opt_fun opt_fun = lv_event_get_user_data(e);

    slider_value = lv_slider_get_value(slider);
    LV_LOG_USER("LV_EVENT_VALUE_CHANGED: %d%%", slider_value);

    //if((slider_value >= 95) || (slider_value <= 11))
    if((slider_value <= (LV_100ASK_GENERIC_UI_LCD_MIN_BACKLIGHT + 150)) || (slider_value >= (LV_100ASK_GENERIC_UI_LCD_MAX_BACKLIGHT - 50)))
    {
        lv_obj_set_style_opa(slider, LV_OPA_TRANSP, LV_PART_KNOB);
    }
    else
    {
        lv_obj_set_style_opa(slider, LV_OPA_COVER, LV_PART_KNOB);
    }

    opt_fun(slider_value);
}


static void slider_hit_test_event_handler(lv_event_t * e)
{
    lv_obj_t *slider = lv_event_get_current_target(e);

    lv_indev_t *indev = lv_indev_get_act();
    if (indev == NULL)
        return;

    int16_t sensitive = 3;  // // TODO: You can adjust the sensitivity
    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);
    //LV_LOG_USER("vect.x: %d, vect.y: %d\n", vect.x, vect.y);
    if (vect.y < -sensitive)
        lv_slider_set_value(slider, (lv_slider_get_value(slider) + 5), LV_ANIM_ON);
    else if (vect.y > sensitive)
        lv_slider_set_value(slider, (lv_slider_get_value(slider) - 5), LV_ANIM_ON);
}


static void btn_event_handler(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    hw_opt_fun opt_fun = lv_event_get_user_data(e);

    if(opt_fun == NULL) return;

    if(code == LV_EVENT_VALUE_CHANGED) { 
        //lv_obj_refresh_ext_draw_size(lv_obj_get_parent(btn));
        opt_fun(lv_obj_has_state(btn, LV_STATE_CHECKED));
    }
}


static void cont_sub_ext_draw_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_REFR_EXT_DRAW_SIZE) {
        lv_event_set_ext_draw_size(e, 20);
    }
}


/**********setting functions************* */

static void opt_passive_buzzer(uint16_t val)
{
    LV_LOG_USER("[opt_passive_buzzer] val: %d", val);
#if LV_100ASK_GENERIC_UI_DshanMCUH7R_NoRTOS
    PassiveBuzzer_Control(val);
#endif
}

static void opt_ws28xx(uint16_t val)
{
    LV_LOG_USER("[opt_ws28xx] val: %d", val);
#if LV_100ASK_GENERIC_UI_DshanMCUH7R_NoRTOS
    ws28xx_all_opt(val);
#endif
}
static void opt_backlight(uint16_t val)
{
    LV_LOG_USER("[opt_backlight] val: %d", val);

#if LV_100ASK_GENERIC_UI_DshanMCUH7R_NoRTOS
    lcd_backlight_set_value(val);
#endif
}
