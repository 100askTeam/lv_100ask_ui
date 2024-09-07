/**
 ******************************************************************************
 * @file    app_sketchpad.c
 * @author  百问科技
 * @version V1.0
 * @date    2024-8-15
 * @brief	app_sketchpad
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

#if (LV_100ASK_GENERIC_UI_NO_BUILTIN_APP != 0) && (LV_100ASK_GENERIC_UI_USE_APP_FILE_EXPLORER != 0)

#include "app_sketchpad.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static int16_t app_sketchpad_open(void);
static int16_t app_sketchpad_close(void);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_100ask_app_data_t lv_100ask_app_sketchpad_t = {
    .cont  = NULL,
    .name  = "Hw Test",
    .desc  = "Hardware Test",
    .open  = app_sketchpad_open,
    .close = app_sketchpad_close
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

static int16_t app_sketchpad_open(void)
{
    lv_obj_t * cont = lv_obj_create(lv_screen_active());
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    LV_DRAW_BUF_DEFINE(draw_buf, SKETCHPAD_DEFAULT_WIDTH, SKETCHPAD_DEFAULT_HEIGHT, LV_COLOR_FORMAT_RGB565);
	lv_obj_t * sketchpad = lv_100ask_sketchpad_create(cont);

	lv_canvas_set_draw_buf(sketchpad, &draw_buf);
    lv_obj_center(sketchpad);
    lv_canvas_fill_bg(sketchpad, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_OPA_COVER);

    lv_100ask_app_sketchpad_t.cont = cont;
    return 0;
}


static int16_t app_sketchpad_close(void)
{
    lv_obj_delete(lv_100ask_app_sketchpad_t.cont);
    return 0;
}


#else

lv_100ask_app_data_t lv_100ask_app_sketchpad_t = {
    .cont  = NULL,
    .name  = NULL,
    .desc  = NULL,
    .open  = NULL,
    .close = NULL
};

#endif

