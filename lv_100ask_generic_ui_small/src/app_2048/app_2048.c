/**
 ******************************************************************************
 * @file    app_2048.c
 * @author  百问科技
 * @version V1.0
 * @date    2024-8-15
 * @brief	app_2048
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
#include "../../lv_100ask_generic_ui_small.h"

#if (LV_100ASK_GENERIC_UI_SMALLE_NO_BUILTIN_APP != 0) && (LV_100ASK_GENERIC_UI_SMALLE_USE_APP_2048 != 0)

#include "app_2048.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static int16_t app_2048_open(void);
static int16_t app_2048_close(void);

static void game_2048_event_cb(lv_event_t * e);
static void new_game_btn_event_handler(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_100ask_app_data_t lv_100ask_app_2048_t = {
    .cont  = NULL,
    .name  = "Hw Test",
    .desc  = "Hardware Test",
    .open  = app_2048_open,
    .close = app_2048_close
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**********************
 *  STATIC FUNCTIONS
 **********************/

 static int16_t app_2048_open(void)
{
    lv_obj_t * cont = lv_obj_create(lv_screen_active());
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));

        /*Create 2048 game*/
    lv_obj_t * obj_2048 = lv_100ask_2048_create(cont);
    lv_obj_set_style_text_font(obj_2048, &lv_font_montserrat_42, 0);
#if LV_100ASK_GENERIC_UI_SMALLE_SCREEN_SIZE_320X480
    lv_obj_set_size(obj_2048, 280, 300);
    lv_obj_align(obj_2048, LV_ALIGN_CENTER, 0, -10);
#elif LV_100ASK_GENERIC_UI_SMALLE_SCREEN_SIZE_480X480
    lv_obj_set_size(obj_2048, 380, 380);
    lv_obj_align(obj_2048, LV_ALIGN_BOTTOM_MID, 0, -30);
#endif

    /*Information*/
    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text_fmt(label, "SCORE:%d", lv_100ask_2048_get_score(obj_2048));
    lv_obj_align_to(label, obj_2048, LV_ALIGN_OUT_TOP_RIGHT, 0, 0);

    lv_obj_add_event_cb(obj_2048, game_2048_event_cb, LV_EVENT_ALL, label);

    /*New Game*/
    lv_obj_t * btn = lv_btn_create(cont);
    lv_obj_align_to(btn, obj_2048, LV_ALIGN_OUT_TOP_LEFT, 0, -20);
    lv_obj_add_event_cb(btn, new_game_btn_event_handler, LV_EVENT_CLICKED, obj_2048);

    label = lv_label_create(btn);
    lv_label_set_text(label, "New Game");
    lv_obj_center(label);

    lv_100ask_app_2048_t.cont = cont;
    return 0;
}


static int16_t app_2048_close(void)
{
    lv_obj_delete(lv_100ask_app_2048_t.cont);
    return 0;
}


static void game_2048_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj_2048 = lv_event_get_target(e);
    lv_obj_t * label = lv_event_get_user_data(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        if (lv_100ask_2048_get_best_tile(obj_2048) >= 2048)
            //lv_label_set_text(label, "#00b329 YOU WIN! #");
            lv_label_set_text(label, "YOU WIN!");
        else if(lv_100ask_2048_get_status(obj_2048))
            //lv_label_set_text(label, "#00b329 www.100ask.net: # #ff0000 GAME OVER! #");
            lv_label_set_text(label, "www.100ask.net: GAME OVER!");
        else
            //lv_label_set_text_fmt(label, "SCORE: #ff00ff %d #", lv_100ask_2048_get_score(obj_2048));
            lv_label_set_text_fmt(label, "SCORE: %d", lv_100ask_2048_get_score(obj_2048));
    }

}

static void new_game_btn_event_handler(lv_event_t * e)
{
    lv_obj_t * obj_2048 = lv_event_get_user_data(e);

    lv_100ask_2048_set_new_game(obj_2048);
}

#else

lv_100ask_app_data_t lv_100ask_app_2048_t = {
    .cont  = NULL,
    .name  = NULL,
    .desc  = NULL,
    .open  = NULL,
    .close = NULL
};

#endif