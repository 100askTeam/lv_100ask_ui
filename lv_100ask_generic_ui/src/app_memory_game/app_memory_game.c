/**
 ******************************************************************************
 * @file    app_memory_game.c
 * @author  百问科技
 * @version V1.0
 * @date    2024-8-15
 * @brief	app_memory_game
 ******************************************************************************
 * Change Logs:
 * Date           Author          Notes
 * 2024-8-15     zhouyuebiao     First version
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

#if (LV_100ASK_GENERIC_UI_NO_BUILTIN_APP != 0) && (LV_100ASK_GENERIC_UI_USE_APP_MEMORY_GAME != 0)

#include "app_memory_game.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static int16_t app_memory_game_open(void);
static int16_t app_memory_game_close(void);

static void slider_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_100ask_app_data_t lv_100ask_app_memory_game_t = {
    .cont  = NULL,
    .name  = "Hw Test",
    .desc  = "Hardware Test",
    .open  = app_memory_game_open,
    .close = app_memory_game_close
};

static lv_obj_t * level_label;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


/**********************
 *  STATIC FUNCTIONS
 **********************/

static int16_t app_memory_game_open(void)
{
    lv_obj_t * cont = lv_obj_create(lv_screen_active());
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));

    lv_obj_t * memory_game = lv_100ask_memory_game_create(cont);
    lv_obj_set_size(memory_game, lv_pct(100), lv_pct(100));
    //lv_100ask_memory_set_map(memory_game, 2, 2);
    lv_obj_set_style_text_font(memory_game, &lv_font_montserrat_30, 0);
    lv_obj_center(memory_game);

    lv_obj_t * slider = lv_slider_create(cont);
    lv_obj_align_to(slider, memory_game, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_slider_set_range(slider, 2, 10);
    lv_slider_set_value(slider, lv_100ask_memory_game_get_row(memory_game), LV_ANIM_ON);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, memory_game);

    level_label = lv_label_create(cont);
    lv_label_set_text_fmt(level_label, "%d*%d", lv_100ask_memory_game_get_row(memory_game), lv_100ask_memory_game_get_row(memory_game));

    lv_obj_align_to(level_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    lv_100ask_app_memory_game_t.cont = cont;
    return 0;
}


static int16_t app_memory_game_close(void)
{
    lv_obj_delete(lv_100ask_app_memory_game_t.cont);
    return 0;
}


static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    lv_obj_t * memory_game = lv_event_get_user_data(e);

    lv_100ask_memory_game_set_map(memory_game, lv_slider_get_value(slider), lv_slider_get_value(slider));
    lv_label_set_text_fmt(level_label, "%d*%d", lv_100ask_memory_game_get_row(memory_game), lv_100ask_memory_game_get_row(memory_game));
}


#else

lv_100ask_app_data_t lv_100ask_app_memory_game_t = {
    .cont  = NULL,
    .name  = NULL,
    .desc  = NULL,
    .open  = NULL,
    .close = NULL
};

#endif
