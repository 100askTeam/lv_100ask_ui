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

#if LV_100ASK_GENERIC_UI_HAS_LOCK_SCREEN != 0

#include "page_lock_screen.h"

#if LV_100ASK_GENERIC_UI_DshanMCUH7R_NoRTOS
#include "driver_aht20.h"
#elif LV_100ASK_GENERIC_UI_USE_SIMULATOR
#endif

/*********************
 *      DEFINES
 *********************/
#if LV_100ASK_GENERIC_UI_LIMIT_LOCK_SCREEN_PIC
    #define MAX_LOCK_SCREEN_IMG     3
#else
    #define MAX_LOCK_SCREEN_IMG     6
#endif
/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_obj_t * cont;
    lv_obj_t * img_bg;
    lv_obj_t * label_time;
    lv_obj_t * label_date;
    lv_timer_t * timer_page_show;
    lv_timer_t * timer_update_humiture;
    lv_timer_t * timer_reset;
}lv_100ask_page_lock_screen_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void style_init(void);
static void page_hidden_event_handler(lv_event_t * e);

static void page_update_pdate_humiture_timer(lv_timer_t * timer);
static void page_show_timer(lv_timer_t * timer);
static void page_hidden_timer(lv_timer_t * timer);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_100ask_page_lock_screen_data_t g_page_lock_screen_data;

static lv_style_t style_btn_default;
static lv_style_t style_btn_check;

/**********************
 *      MACROS
 **********************/
LV_IMG_DECLARE(img_lock_screen_1);
LV_IMG_DECLARE(img_lock_screen_2);
LV_IMG_DECLARE(img_lock_screen_3);
LV_IMG_DECLARE(img_lock_screen_4);
LV_IMG_DECLARE(img_lock_screen_5);
LV_IMG_DECLARE(img_lock_screen_6);

#if LV_100ASK_GENERIC_UI_LIMIT_LOCK_SCREEN_PIC
static const lv_image_dsc_t *g_img_lock_screen[MAX_LOCK_SCREEN_IMG] = {
    &img_lock_screen_1,
    &img_lock_screen_4,
    &img_lock_screen_6
};
#else
static const lv_image_dsc_t *g_img_lock_screen[MAX_LOCK_SCREEN_IMG] = {
    &img_lock_screen_1,
    &img_lock_screen_2,
    &img_lock_screen_3,
    &img_lock_screen_4,
    &img_lock_screen_5,
    &img_lock_screen_6
};
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
lv_obj_t * page_lock_screen_init(void)
{
    style_init();

    g_page_lock_screen_data.cont = lv_obj_create(lv_layer_sys());
    lv_obj_remove_style_all(g_page_lock_screen_data.cont);
    lv_obj_clear_flag(g_page_lock_screen_data.cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_size(g_page_lock_screen_data.cont, lv_pct(100), lv_pct(100));
    //lv_obj_set_style_bg_color(g_page_lock_screen_data.cont, lv_color_hex(0xe1e1e1), 0);

    g_page_lock_screen_data.img_bg = lv_image_create(g_page_lock_screen_data.cont);
    lv_image_set_src(g_page_lock_screen_data.img_bg, g_img_lock_screen[lv_rand(0, (MAX_LOCK_SCREEN_IMG-1))]);
    lv_obj_center(g_page_lock_screen_data.img_bg);
    lv_obj_set_user_data(g_page_lock_screen_data.cont, g_page_lock_screen_data.img_bg);


    lv_obj_t * label_time = lv_label_create(g_page_lock_screen_data.cont);
    lv_obj_set_style_text_font(label_time, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(label_time, lv_color_hex(0xf5f5f7), 0);
    lv_label_set_text(label_time, "2 1 : 0 0");
    lv_obj_align(label_time, LV_ALIGN_CENTER, 0, -100);

    lv_obj_t * label_date = lv_label_create(g_page_lock_screen_data.cont);
    lv_obj_set_style_text_font(label_date, &lv_font_montserrat_26, 0);
    lv_obj_set_style_text_color(label_date, lv_color_hex(0xf5f5f7), 0);
    lv_label_set_text(label_date, "Tuesday, August 13");
    lv_obj_align_to(label_date, label_time, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    //////////////////////////////////////
    lv_obj_t * label_tip = lv_label_create(g_page_lock_screen_data.cont);
    lv_obj_set_style_text_font(label_tip, &lv_font_montserrat_26, 0);
    lv_obj_set_style_text_color(label_tip, lv_color_hex(0xf5f5f7), 0);
    lv_label_set_text(label_tip, "Tuesday, August 13");
    lv_obj_align_to(label_tip, label_time, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    //////////////////////////////////////
    lv_obj_t * label_humiture = lv_label_create(g_page_lock_screen_data.cont);
#if LV_100ASK_GENERIC_UI_SCREEN_SIZE_320X480
    lv_obj_set_style_text_font(label_humiture, &lv_font_montserrat_14, 0);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_480X480
    lv_obj_set_style_text_font(label_humiture, &lv_font_montserrat_20, 0);
#endif    
    lv_obj_set_style_text_color(label_humiture, lv_color_hex(0xf5f5f7), 0);
    lv_label_set_text_fmt(label_humiture, LV_SYMBOL_HOME"Temperature: %dC      Humidity: %d%%", 24, 60);
    lv_obj_align(label_humiture, LV_ALIGN_BOTTOM_MID, 0, -40);

    g_page_lock_screen_data.timer_update_humiture = lv_timer_create(page_update_pdate_humiture_timer, 1000, label_humiture);

    //////////////////////////////////////
    //lv_obj_fade_out(g_page_lock_screen_data.cont, 0, 0);
    lv_obj_add_flag(g_page_lock_screen_data.cont, LV_OBJ_FLAG_HIDDEN);
    g_page_lock_screen_data.timer_page_show = lv_timer_create(page_show_timer, 5000, g_page_lock_screen_data.cont);
    lv_obj_add_event_cb(g_page_lock_screen_data.cont, page_hidden_event_handler, LV_EVENT_CLICKED, g_page_lock_screen_data.timer_page_show);

    return g_page_lock_screen_data.cont;
}


/// @brief 
/// @param period 
void set_page_lock_screen_time(uint32_t period)
{
    if(period == 0)
    {
        lv_timer_pause(g_page_lock_screen_data.timer_page_show);
        lv_timer_pause(g_page_lock_screen_data.timer_update_humiture);
    }
    else
    {
        lv_timer_set_period(g_page_lock_screen_data.timer_page_show, period);
        lv_timer_resume(g_page_lock_screen_data.timer_page_show);
        lv_timer_reset(g_page_lock_screen_data.timer_page_show);
    }
}


void set_page_lock_screen_reset(void)
{
    lv_timer_reset(g_page_lock_screen_data.timer_page_show);
    lv_timer_reset(g_page_lock_screen_data.timer_update_humiture);
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
}


static void page_hidden_event_handler(lv_event_t * e)
{
    lv_obj_t * cont = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_timer_t * timer = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED) {
        lv_timer_reset(timer);
        lv_timer_resume(timer);
        lv_obj_fade_out(cont, 400, 0);
        lv_timer_t * timer = lv_timer_create(page_hidden_timer, 400, cont);
        lv_timer_set_repeat_count(timer, 1);
    }
}

static void page_hidden_timer(lv_timer_t * timer)
{
    /*Use the user_data*/
    lv_obj_t * cont = lv_timer_get_user_data(timer);
    lv_obj_add_flag(cont, LV_OBJ_FLAG_HIDDEN);
}


static void page_show_timer(lv_timer_t * timer)
{
    /*Use the user_data*/
    lv_obj_t * cont = lv_timer_get_user_data(timer);
    lv_obj_t * img = lv_obj_get_user_data(cont);
    lv_image_set_src(img, g_img_lock_screen[lv_rand(0, (MAX_LOCK_SCREEN_IMG-1))]);

    lv_obj_clear_flag(cont, LV_OBJ_FLAG_HIDDEN);
    lv_obj_fade_in(cont, 500, 0);
    lv_timer_pause(timer);
}


static void page_update_pdate_humiture_timer(lv_timer_t * timer)
{
    int32_t temp;
    int32_t humi;

    /*Use the user_data*/
    lv_obj_t * label_humiture = lv_timer_get_user_data(timer);

#if LV_100ASK_GENERIC_UI_DshanMCUH7R_NoRTOS
    aht20_update();
    aht20_get_datas(&temp, &humi);
    temp = temp / 10;
    humi = humi / 10;
#elif LV_100ASK_GENERIC_UI_USE_SIMULATOR
    temp = lv_rand(20, 34);
    humi = lv_rand(60, 80);
#endif

    lv_label_set_text_fmt(label_humiture, LV_SYMBOL_HOME"Temperature: %dC      Humidity: %d%%", temp, humi);
}


#endif /* LV_100ASK_GENERIC_UI_HAS_LOCK_SCREEN */
