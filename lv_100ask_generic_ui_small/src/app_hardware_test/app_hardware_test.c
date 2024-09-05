/**
 ******************************************************************************
 * @file    app_hardware_test.c
 * @author  百问科技
 * @version V1.0
 * @date    2024-8-11
 * @brief	app_hardware_test
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
#include "../../lv_100ask_generic_ui_small.h"

#include <string.h>

#include "app_hardware_test.h"

#if LV_100ASK_DESKTOP_USE_DshanMCUH7R_NoRTOS
#include "driver_passive_buzzer.h"
#include "driver_ws28xx.h"
#include "driver_ir_receiver.h"
#include "driver_ec11.h"
#include "driver_aht20.h"
#include "driver_w800.h"

#elif LV_100ASK_DESKTOP_USE_SIMULATOR

#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_timer_t * timer_update_humiture;
    lv_timer_t * timer_update_ir_rec;
    lv_timer_t * timer_update_w800_at;
    lv_timer_t * timer_update_key;
    lv_timer_t * timer_update_ec11;

    lv_obj_t * chart_temp;
    lv_obj_t * chart_humid;
    lv_obj_t * red_slider;
    lv_obj_t * green_slider;
    lv_obj_t * blue_slider;
    lv_color_t lamp_color
}lv_100ask_app_hw_test_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void style_init(void);

static int16_t app_hardware_test_open(void);
static int16_t app_hardware_test_close(void);

static void chart_draw_event_cb(lv_event_t * e);
static void chart_add_faded_area(lv_event_t * e);

static void update_humiture_timer(lv_timer_t * timer);
static lv_obj_t * create_slider(lv_obj_t * parent, lv_color_t color);
static void slider_event_cb(lv_event_t * e);
static void btn_lamp_event_handler(lv_event_t * e);
static void btn_buzzer_event_handler(lv_event_t * e);
static void update_w800_at_timer(lv_timer_t * timer);
static void update_ir_rec_timer(lv_timer_t * timer);
static void update_key_timer(lv_timer_t * timer);
static void update_ec11_timer(lv_timer_t * timer);


/**********************
 *  STATIC VARIABLES
 **********************/
lv_100ask_app_data_t lv_100ask_app_hardware_test_t = {
    .cont  = NULL,
    .name  = "Hw Test",
    .desc  = "Hardware Test",
    .open  = app_hardware_test_open,
    .close = app_hardware_test_close
};


static lv_100ask_app_hw_test_data_t g_lv_100ask_app_hw_test_data;
static lv_style_t style_cont;
static lv_style_t style_cont_sub;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


/**********************
 *  STATIC FUNCTIONS
 **********************/

 /// @brief
 /// @param
 /// @return
static int16_t app_hardware_test_open(void)
{
    uint32_t i;
    lv_obj_t * tmp_cont;
    lv_obj_t * label;
    lv_obj_t * btn;
    lv_timer_t * timer;
    lv_obj_t * cont_sub;

    style_init();

    lv_obj_t * cont = lv_obj_create(lv_screen_active());
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    //lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_style(cont, &style_cont, 0);

    ////////////////////////////////////////////////////////// 1
    cont_sub = lv_obj_create(cont);
    lv_obj_add_style(cont_sub, &style_cont_sub, 0);
    lv_obj_set_size(cont_sub, lv_pct(100), lv_pct(58));
    lv_obj_set_flex_flow(cont_sub, LV_FLEX_FLOW_COLUMN);
    lv_obj_clear_flag(cont_sub, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * chart_temp;
    lv_obj_t * chart_humid;
    lv_obj_t * scale_left;
    lv_chart_series_t * ser_temp;
    lv_chart_series_t * ser_humid;

    static const char * scale_tick[] = {"0", "20", "40", "60", "80", "100", NULL};
    /* Create a chart Temperature */
    label = lv_label_create(cont_sub);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_10, 0);
    ////lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text(label, "Temperature");

    tmp_cont = lv_obj_create(cont_sub);
    lv_obj_remove_style_all(tmp_cont);
    lv_obj_set_size(tmp_cont, lv_pct(100), lv_pct(40));
    lv_obj_set_flex_flow(tmp_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_ver(tmp_cont, 3, 0);

    scale_left = lv_scale_create(tmp_cont);
    lv_scale_set_mode(scale_left, LV_SCALE_MODE_VERTICAL_LEFT);
    lv_obj_set_size(scale_left, 20, lv_pct(100));
    lv_scale_set_total_tick_count(scale_left, 6);
    lv_scale_set_major_tick_every(scale_left, 1);
    lv_scale_set_text_src(scale_left, scale_tick);
    lv_obj_set_style_length(scale_left, 0, LV_PART_INDICATOR);
    lv_obj_set_style_text_font(scale_left, &lv_font_montserrat_10, LV_PART_INDICATOR);
    lv_obj_set_style_text_color(scale_left, lv_palette_main(LV_PALETTE_PINK), LV_PART_INDICATOR);
    chart_temp = lv_chart_create(tmp_cont);
    lv_obj_set_size(chart_temp, lv_pct(100), lv_pct(100));
    lv_obj_set_style_border_width(chart_temp, 1, 0);
    ////lv_obj_set_style_bg_color(chart_temp, lv_color_hex(0x222a35), 0);
    lv_obj_set_style_radius(chart_temp, 0, 0);
    lv_chart_set_range(chart_temp, LV_CHART_AXIS_PRIMARY_Y, 0 , 100);
    lv_chart_set_type(chart_temp, LV_CHART_TYPE_LINE);   /*Show lines and points too*/
    lv_obj_set_grid_cell(chart_temp, LV_GRID_ALIGN_STRETCH, 2, 2, LV_GRID_ALIGN_STRETCH, 0, 4);

    lv_chart_set_point_count(chart_temp, 10);
    lv_chart_set_div_line_count(chart_temp, 8, 10);
    lv_obj_refresh_ext_draw_size(chart_temp);

    lv_obj_add_event_cb(chart_temp, chart_draw_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    //lv_obj_add_event_cb(chart_temp, chart_show_value_of_the_pressed_points_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(chart_temp, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

    ser_temp = lv_chart_add_series(chart_temp, lv_palette_main(LV_PALETTE_PINK), LV_CHART_AXIS_PRIMARY_Y);

    /* Create a chart Humidity */
    label = lv_label_create(cont_sub);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_10, 0);
    ////lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text(label, "Humidity");

    tmp_cont = lv_obj_create(cont_sub);
    lv_obj_remove_style_all(tmp_cont);
    lv_obj_set_size(tmp_cont, lv_pct(100), lv_pct(40));
    lv_obj_set_flex_flow(tmp_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_ver(tmp_cont, 3, 0);

    scale_left = lv_scale_create(tmp_cont);
    lv_scale_set_mode(scale_left, LV_SCALE_MODE_VERTICAL_LEFT);
    lv_obj_set_size(scale_left, 20, lv_pct(100));
    lv_scale_set_total_tick_count(scale_left, 6);
    lv_scale_set_major_tick_every(scale_left, 1);
    lv_scale_set_text_src(scale_left, scale_tick);
    lv_obj_set_style_length(scale_left, 0, LV_PART_INDICATOR);
    lv_obj_set_style_text_font(scale_left, &lv_font_montserrat_10, LV_PART_INDICATOR);
    lv_obj_set_style_text_color(scale_left, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    chart_humid = lv_chart_create(tmp_cont);
    lv_obj_set_size(chart_humid, lv_pct(100), lv_pct(100));
    lv_obj_set_style_border_width(chart_humid, 1, 0);
    ////lv_obj_set_style_bg_color(chart_humid, lv_color_hex(0x222a35), 0);
    lv_obj_set_style_radius(chart_humid, 0, 0);
    lv_chart_set_range(chart_humid, LV_CHART_AXIS_PRIMARY_Y, 0 , 100);
    lv_chart_set_type(chart_humid, LV_CHART_TYPE_LINE);   /*Show lines and points too*/
    lv_obj_set_grid_cell(chart_humid, LV_GRID_ALIGN_STRETCH, 2, 2, LV_GRID_ALIGN_STRETCH, 0, 4);

    lv_chart_set_point_count(chart_humid, 10);
    lv_chart_set_div_line_count(chart_humid, 8, 10);
    lv_obj_refresh_ext_draw_size(chart_humid);

    lv_obj_add_event_cb(chart_humid, chart_draw_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    //lv_obj_add_event_cb(chart_humid, chart_show_value_of_the_pressed_points_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(chart_humid, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

    ser_humid = lv_chart_add_series(chart_humid, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);

    g_lv_100ask_app_hw_test_data.chart_temp = chart_temp;
    g_lv_100ask_app_hw_test_data.chart_humid = chart_humid;

    g_lv_100ask_app_hw_test_data.timer_update_humiture = lv_timer_create(update_humiture_timer, 1000, cont_sub);

    ////////////////////////////////////////////////////////// 2
    cont_sub = lv_obj_create(cont);
    lv_obj_add_style(cont_sub, &style_cont_sub, 0);
    lv_obj_set_style_pad_all(cont_sub, 12, 0);
    lv_obj_set_style_pad_row(cont_sub, 12, 0);
    lv_obj_set_size(cont_sub, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont_sub, LV_FLEX_FLOW_COLUMN);
    lv_obj_clear_flag(cont_sub, LV_OBJ_FLAG_SCROLLABLE);

    g_lv_100ask_app_hw_test_data.red_slider = create_slider(cont_sub, lv_palette_main(LV_PALETTE_RED));
    g_lv_100ask_app_hw_test_data.green_slider = create_slider(cont_sub, lv_palette_main(LV_PALETTE_GREEN));
    g_lv_100ask_app_hw_test_data.blue_slider = create_slider(cont_sub, lv_palette_main(LV_PALETTE_BLUE));

    tmp_cont = lv_obj_create(cont_sub);
    lv_obj_remove_style_all(tmp_cont);
    lv_obj_set_size(tmp_cont, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(tmp_cont, 2, 0);
    lv_obj_set_flex_flow(tmp_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(tmp_cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    btn = lv_button_create(tmp_cont);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label, "LED1");
    lv_obj_add_event_cb(btn, btn_lamp_event_handler, LV_EVENT_VALUE_CHANGED, label);
    btn = lv_button_create(tmp_cont);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label, "LED2");
    lv_obj_add_event_cb(btn, btn_lamp_event_handler, LV_EVENT_VALUE_CHANGED, label);
    btn = lv_button_create(tmp_cont);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label, "LED3");
    lv_obj_add_event_cb(btn, btn_lamp_event_handler, LV_EVENT_VALUE_CHANGED, label);
    btn = lv_button_create(tmp_cont);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label, "LED4");
    lv_obj_add_event_cb(btn, btn_lamp_event_handler, LV_EVENT_VALUE_CHANGED, label);

    ////////////////////////////////////////////////////////// 3
    cont_sub = lv_obj_create(cont);
    lv_obj_add_style(cont_sub, &style_cont_sub, 0);
    lv_obj_set_style_pad_all(cont_sub, 14, 0);
    lv_obj_set_size(cont_sub, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont_sub, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont_sub, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(cont_sub, LV_OBJ_FLAG_SCROLLABLE);

    btn = lv_button_create(cont_sub);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_state(btn, LV_STATE_DISABLED);
    label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label, "KEY1");
    btn = lv_button_create(cont_sub);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_state(btn, LV_STATE_DISABLED);
    label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label, "KEY2");
    btn = lv_button_create(cont_sub);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_state(btn, LV_STATE_DISABLED);
    label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label, "KEY3");
    btn = lv_button_create(cont_sub);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_state(btn, LV_STATE_DISABLED);
    label = lv_label_create(btn);
    lv_obj_center(label);
    lv_label_set_text(label, "KEY4");

    g_lv_100ask_app_hw_test_data.timer_update_key = lv_timer_create(update_key_timer, 30, cont_sub);


    ////////////////////////////////////////////////////////// 4
    cont_sub = lv_obj_create(cont);
    lv_obj_add_style(cont_sub, &style_cont_sub, 0);
    lv_obj_set_style_pad_all(cont_sub, 12, 0);
    lv_obj_set_style_pad_row(cont_sub, 12, 0);
    lv_obj_set_size(cont_sub, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont_sub, LV_FLEX_FLOW_COLUMN);
    lv_obj_clear_flag(cont_sub, LV_OBJ_FLAG_SCROLLABLE);

    label = lv_label_create(cont_sub);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_18, 0);
    ////lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text_fmt(label, "IR-Receiver code:    %s", "Error");

    g_lv_100ask_app_hw_test_data.timer_update_ir_rec = lv_timer_create(update_ir_rec_timer, 30, label);

    ////////////////////////////////////////////////////////// 5
    cont_sub = lv_obj_create(cont);
    lv_obj_add_style(cont_sub, &style_cont_sub, 0);
    lv_obj_set_style_pad_all(cont_sub, 12, 0);
    lv_obj_set_style_pad_row(cont_sub, 12, 0);
    lv_obj_set_size(cont_sub, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont_sub, LV_FLEX_FLOW_COLUMN);
    lv_obj_clear_flag(cont_sub, LV_OBJ_FLAG_SCROLLABLE);

    label = lv_label_create(cont_sub);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_18, 0);
    ////lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text_fmt(label, "W800 AT Test:    %s", "Error");

    g_lv_100ask_app_hw_test_data.timer_update_w800_at = lv_timer_create(update_w800_at_timer, 400, label);

    ////////////////////////////////////////////////////////// 6
    cont_sub = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_sub);
    lv_obj_set_size(cont_sub, 210, 120);
    lv_obj_set_style_bg_opa(cont_sub, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(cont_sub, 2, 0);
    lv_obj_clear_flag(cont_sub, LV_OBJ_FLAG_SCROLLABLE);

    btn = lv_button_create(cont_sub);
    lv_obj_set_size(btn, lv_pct(100), lv_pct(100));
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_event_cb(btn, btn_buzzer_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    label = lv_label_create(btn);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_22, 0);
    //lv_obj_set_style_text_color(label, lv_color_white(), 0);
    lv_label_set_text(label, "BUZZER");
    lv_obj_center(label);


    ////////////////////////////////////////////////////////// 7
    cont_sub = lv_obj_create(cont);
    //lv_obj_remove_style_all(cont_sub);
    lv_obj_set_size(cont_sub, 210, 120);
    lv_obj_add_style(cont_sub, &style_cont_sub, 0);
    lv_obj_set_style_pad_all(cont_sub, 2, 0);
    lv_obj_clear_flag(cont_sub, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * arc = lv_arc_create(cont_sub);
    lv_arc_set_mode(arc, LV_ARC_MODE_SYMMETRICAL);
    lv_arc_set_range(arc, -100, 100);
    lv_obj_set_size(arc, 120, 120);
    lv_arc_set_rotation(arc, 135);
    lv_arc_set_bg_angles(arc, 0, 270);
    lv_arc_set_value(arc, 0);
    lv_obj_align(arc, LV_ALIGN_CENTER, 0, 10);
    g_lv_100ask_app_hw_test_data.timer_update_ec11 = lv_timer_create(update_ec11_timer, 30, arc);


    ////////////////////////////////////////////////////////// 8
    cont_sub = lv_obj_create(cont);
    lv_obj_add_style(cont_sub, &style_cont_sub, 0);
    lv_obj_set_style_pad_all(cont_sub, 12, 0);
    lv_obj_set_style_pad_row(cont_sub, 12, 0);
    lv_obj_set_size(cont_sub, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont_sub, LV_FLEX_FLOW_COLUMN);
    lv_obj_clear_flag(cont_sub, LV_OBJ_FLAG_SCROLLABLE);

    label = lv_label_create(cont_sub);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, 0);
    ////lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(label, "Docs:  https://lvgl.100ask.net\nForum:  https://forums.100ask.net");
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);


    //////////////////////////////////////////////////////////
    lv_100ask_app_hardware_test_t.cont = cont;
    return 0;
}


/// @brief
/// @param
/// @return
static int16_t app_hardware_test_close(void)
{
    lv_style_reset(&style_cont);
    lv_style_reset(&style_cont_sub);

    lv_timer_delete(g_lv_100ask_app_hw_test_data.timer_update_humiture);
    lv_timer_delete(g_lv_100ask_app_hw_test_data.timer_update_ir_rec);
    lv_timer_delete(g_lv_100ask_app_hw_test_data.timer_update_key);
    lv_timer_delete(g_lv_100ask_app_hw_test_data.timer_update_ec11);

    lv_obj_delete(lv_100ask_app_hardware_test_t.cont);
    return 0;
}


/// @brief
/// @param
static void style_init(void)
{
    lv_style_init(&style_cont);
    ////lv_style_set_bg_color(&style_cont, lv_color_hex(0x000000));
    lv_style_set_radius(&style_cont, 0);
    lv_style_set_border_opa(&style_cont, LV_OPA_TRANSP);
    lv_style_set_flex_flow(&style_cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_style_set_flex_main_place(&style_cont, LV_FLEX_ALIGN_SPACE_AROUND);
    lv_style_set_layout(&style_cont, LV_LAYOUT_FLEX);

    lv_style_init(&style_cont_sub);
    lv_style_set_bg_opa(&style_cont_sub, LV_OPA_COVER);
    ////lv_style_set_bg_color(&style_cont_sub, lv_color_hex(0x222a35));
    ////lv_style_set_border_color(&style_cont_sub, lv_color_hex(0x222a35));
    lv_style_set_radius(&style_cont_sub, 6);
    lv_style_set_pad_all(&style_cont_sub, 4);
}


/// @brief
/// @param e
static void chart_draw_event_cb(lv_event_t * e)
{
    lv_draw_task_t * draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base_dsc = lv_draw_task_get_draw_dsc(draw_task);

    if(base_dsc->part == LV_PART_ITEMS && lv_draw_task_get_type(draw_task) == LV_DRAW_TASK_TYPE_LINE) {
        chart_add_faded_area(e);

    }
    /*Hook the division lines too*/
    //if(base_dsc->part == LV_PART_MAIN && lv_draw_task_get_type(draw_task) == LV_DRAW_TASK_TYPE_LINE) {
        //chart_hook_division_lines(e);
    //}
}


/// @brief
/// @param e
static void chart_add_faded_area(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);

    lv_draw_task_t * draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base_dsc = lv_draw_task_get_draw_dsc(draw_task);

    const lv_chart_series_t * ser = lv_chart_get_series_next(obj, NULL);
    lv_color_t ser_color = lv_chart_get_series_color(obj, ser);

    /*Draw a triangle below the line witch some opacity gradient*/
    lv_draw_line_dsc_t * draw_line_dsc = lv_draw_task_get_draw_dsc(draw_task);
    lv_draw_triangle_dsc_t tri_dsc;

    lv_draw_triangle_dsc_init(&tri_dsc);
    tri_dsc.p[0].x = draw_line_dsc->p1.x;
    tri_dsc.p[0].y = draw_line_dsc->p1.y;
    tri_dsc.p[1].x = draw_line_dsc->p2.x;
    tri_dsc.p[1].y = draw_line_dsc->p2.y;
    tri_dsc.p[2].x = draw_line_dsc->p1.y < draw_line_dsc->p2.y ? draw_line_dsc->p1.x : draw_line_dsc->p2.x;
    tri_dsc.p[2].y = LV_MAX(draw_line_dsc->p1.y, draw_line_dsc->p2.y);
    tri_dsc.bg_grad.dir = LV_GRAD_DIR_VER;

    int32_t full_h = lv_obj_get_height(obj);
    int32_t fract_uppter = (int32_t)(LV_MIN(draw_line_dsc->p1.y, draw_line_dsc->p2.y) - coords.y1) * 255 / full_h;
    int32_t fract_lower = (int32_t)(LV_MAX(draw_line_dsc->p1.y, draw_line_dsc->p2.y) - coords.y1) * 255 / full_h;
    tri_dsc.bg_grad.stops[0].color = ser_color;
    tri_dsc.bg_grad.stops[0].opa = 255 - fract_uppter;
    tri_dsc.bg_grad.stops[0].frac = 0;
    tri_dsc.bg_grad.stops[1].color = ser_color;
    tri_dsc.bg_grad.stops[1].opa = 255 - fract_lower;
    tri_dsc.bg_grad.stops[1].frac = 255;

    lv_draw_triangle(base_dsc->layer, &tri_dsc);

    /*Draw rectangle below the triangle*/
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_grad.dir = LV_GRAD_DIR_VER;
    rect_dsc.bg_grad.stops[0].color = ser_color;
    rect_dsc.bg_grad.stops[0].frac = 0;
    rect_dsc.bg_grad.stops[0].opa = 255 - fract_lower;
    rect_dsc.bg_grad.stops[1].color = ser_color;
    rect_dsc.bg_grad.stops[1].frac = 255;
    rect_dsc.bg_grad.stops[1].opa = 0;

    lv_area_t rect_area;
    rect_area.x1 = (int32_t)draw_line_dsc->p1.x;
    rect_area.x2 = (int32_t)draw_line_dsc->p2.x - 1;
    rect_area.y1 = (int32_t)LV_MAX(draw_line_dsc->p1.y, draw_line_dsc->p2.y) - 1;
    rect_area.y2 = (int32_t)coords.y2;
    lv_draw_rect(base_dsc->layer, &rect_dsc, &rect_area);
}


/// @brief
/// @param timer
static void update_humiture_timer(lv_timer_t * timer)
{
    int32_t temp;
    int32_t humi;
    /*Use the user_data*/
    //lv_obj_t * cont_sub = lv_timer_get_user_data(timer);
    lv_obj_t * chart_temp = g_lv_100ask_app_hw_test_data.chart_temp;
    lv_obj_t * chart_humid = g_lv_100ask_app_hw_test_data.chart_humid;
    lv_chart_series_t * ser_temp = lv_chart_get_series_next(chart_temp, NULL);
    lv_chart_series_t * ser_humid = lv_chart_get_series_next(chart_humid, NULL);

#if LV_100ASK_DESKTOP_USE_DshanMCUH7R_NoRTOS
    aht20_update();
    aht20_get_datas(&temp, &humi);
    temp = temp / 10;
    humi = humi / 10;
#elif LV_100ASK_DESKTOP_USE_SIMULATOR
    temp = lv_rand(20, 34);
    humi = lv_rand(60, 80);
#endif

    lv_chart_set_next_value(chart_temp, ser_temp, temp);
    lv_chart_set_next_value(chart_humid, ser_humid, humi);
}

/// @brief
/// @param parent
/// @param color
/// @return
static lv_obj_t * create_slider(lv_obj_t * parent, lv_color_t color)
{
    lv_obj_t * slider = lv_slider_create(parent);
    lv_slider_set_range(slider, 0, 255);
    lv_obj_set_size(slider, lv_pct(100), 10);
    lv_obj_set_style_bg_color(slider, color, LV_PART_KNOB);
    lv_obj_set_style_bg_color(slider, lv_color_darken(color, LV_OPA_40), LV_PART_INDICATOR);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, parent);

    return slider;
}

/// @brief
/// @param e
static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * parent = lv_event_get_user_data(e);
    lv_obj_t * red_slider = lv_obj_get_child(parent, 0);
    lv_obj_t * green_slider = lv_obj_get_child(parent, 1);
    lv_obj_t * blue_slider = lv_obj_get_child(parent, 2);

    /*Recolor the image based on the sliders' values*/
    lv_color_t color  = lv_color_make(lv_slider_get_value(red_slider), lv_slider_get_value(green_slider),
                                      lv_slider_get_value(blue_slider));

    lv_obj_set_style_bg_color(parent, color, 0);

    g_lv_100ask_app_hw_test_data.lamp_color = color;
}

/// @brief
/// @param e
static void btn_lamp_event_handler(lv_event_t * e)
{
    uint16_t pixel_index = 0;
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * label = lv_event_get_user_data(e);

    char * label_str = lv_label_get_text(label);
    if(lv_strcmp("LED1", label_str) == 0)        pixel_index = 0;
    else if(lv_strcmp("LED2", label_str) == 0)   pixel_index = 1;
    else if(lv_strcmp("LED3", label_str) == 0)   pixel_index = 2;
    else if(lv_strcmp("LED4", label_str) == 0)   pixel_index = 3;

    if(lv_obj_has_state(btn, LV_STATE_CHECKED))
    {
#if LV_100ASK_DESKTOP_USE_DshanMCUH7R_NoRTOS
    WS28XX_SetPixel_RGB_565(&WS28xx_Handle, pixel_index, lv_color_to_u16(g_lv_100ask_app_hw_test_data.lamp_color));
    WS28XX_Update(&WS28xx_Handle);
#elif LV_100ASK_DESKTOP_USE_SIMULATOR
    LV_LOG_USER("pixel_index:%d", pixel_index);
#endif
    }
    else{
#if LV_100ASK_DESKTOP_USE_DshanMCUH7R_NoRTOS
    WS28XX_SetPixel_RGB_565(&WS28xx_Handle, pixel_index, 0x000000);
    WS28XX_Update(&WS28xx_Handle);
#elif LV_100ASK_DESKTOP_USE_SIMULATOR
    LV_LOG_USER("pixel_index:%d", pixel_index);
#endif
    }

}

/// @brief
/// @param e
static void btn_buzzer_event_handler(lv_event_t * e)
{
    lv_obj_t * btn = lv_event_get_target(e);
    
#if LV_100ASK_DESKTOP_USE_DshanMCUH7R_NoRTOS
    PassiveBuzzer_Control(lv_obj_has_state(btn, LV_STATE_CHECKED));
#endif
}


/// @brief
/// @param timer
static void update_ir_rec_timer(lv_timer_t * timer)
{
    /*Use the user_data*/
    lv_obj_t * label = lv_timer_get_user_data(timer);

#if LV_100ASK_DESKTOP_USE_DshanMCUH7R_NoRTOS
    uint8_t dev, data;
    if (!IRReceiver_Read(&dev, &data))
        lv_label_set_text_fmt(label, "IR-Receiver code:   %s", IRReceiver_CodeToString(data));
    //else
    //    lv_label_set_text_fmt(label, "IR-Receiver code:   %s", "Error");
#elif LV_100ASK_DESKTOP_USE_SIMULATOR
    const char *names[21]= {"Power", "Menu", "Test", "+", "Return", "Left", "Play", "Right", \
                            "0", "-", "C", "1", "2", "3", "4", "5", \
                            "6", "7", "8", "9", "Repeat"};

    lv_label_set_text_fmt(label, "IR-Receiver code:   %s", names[lv_rand(0, 21)]);
    lv_timer_set_period(timer, 1000);
#endif
}


/// @brief
/// @param timer
static void update_w800_at_timer(lv_timer_t * timer)
{
    char * test_res = NULL;
    /*Use the user_data*/
    lv_obj_t * label = lv_timer_get_user_data(timer);

#if LV_100ASK_DESKTOP_USE_DshanMCUH7R_NoRTOS
    uint8_t str_tmp[8] = "AT+\r";

    HAL_UART_Transmit(&huart7, str_tmp, 4, 10);
    memset(str_tmp, 0, 8);
    HAL_UART_Receive(&huart7, str_tmp, 7, 10);
    lv_label_set_text_fmt(label, "W800 AT Test11:    %s", str_tmp);
    
    test_res = strstr(str_tmp, "+OK");
#elif LV_100ASK_DESKTOP_USE_SIMULATOR
    char *str_tmp[2]= {"+OK\r\n", "ERROR"};

    test_res = strstr(str_tmp[lv_rand(0, 1)], "+OK");
#endif

    if(test_res)
    {
        lv_label_set_text_fmt(label, "W800 AT Test:    %s", "OK");
        lv_timer_delete(timer);
    }
}



/// @brief
/// @param timer
static void update_key_timer(lv_timer_t * timer)
{
    lv_obj_t * btn;
    int32_t key_index = -1;

    /*Use the user_data*/
    lv_obj_t * cont_sub = lv_timer_get_user_data(timer);

#if LV_100ASK_DESKTOP_USE_DshanMCUH7R_NoRTOS
    if      (GPIO_PIN_RESET == HAL_GPIO_ReadPin(USER_KEY1_GPIO_Port, USER_KEY1_Pin))    key_index = 0;
    else if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(USER_KEY2_GPIO_Port, USER_KEY2_Pin))    key_index = 1;
    else if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(USER_KEY3_GPIO_Port, USER_KEY3_Pin))    key_index = 2;
    else if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(USER_KEY4_GPIO_Port, USER_KEY4_Pin))    key_index = 3;

    if(key_index != -1)
    {
        btn = lv_obj_get_child(cont_sub, key_index);
        lv_obj_add_state(btn, LV_STATE_CHECKED);
    }
#elif LV_100ASK_DESKTOP_USE_SIMULATOR
    key_index = lv_rand(0, 3);

    btn = lv_obj_get_child(cont_sub, key_index);
    lv_obj_add_state(btn, LV_STATE_CHECKED);

    lv_timer_set_period(timer, 1000);
#endif
}

/// @brief
/// @param timer
static void update_ec11_timer(lv_timer_t * timer)
{
    /*Use the user_data*/
    lv_obj_t * arc = lv_timer_get_user_data(timer);

#if LV_100ASK_DESKTOP_USE_DshanMCUH7R_NoRTOS
    int8_t encoder_diff = ec11_scan();
    int32_t last_arc_value = lv_arc_get_value(arc);
    if (encoder_diff == 1)
        lv_arc_set_value(arc, last_arc_value+10);
    else if (encoder_diff == -1)
        lv_arc_set_value(arc, last_arc_value-10);

    if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(EC11_D_GPIO_Port, EC11_D_Pin))
    {
        lv_arc_set_value(arc, 0);
    }
#elif LV_100ASK_DESKTOP_USE_SIMULATOR
    int8_t encoder_diff = lv_rand(0, 2);
    if (encoder_diff == 1)
        lv_arc_set_value(arc, lv_arc_get_value(arc)+10);
    else if (encoder_diff == 2)
        lv_arc_set_value(arc, lv_arc_get_value(arc)-10);
    else if (encoder_diff == 0)
        lv_arc_set_value(arc, 0);

    lv_timer_set_period(timer, 1000);
#endif
}
