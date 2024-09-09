/**
 ******************************************************************************
 * @file    lesson_2_3_1.c
 * @author  百问科技
 * @version V1.0
 * @date    2024-4-25
 * @brief	Lesson 2-3-1 demo
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


#include "lv_100ask_generic_ui_main.h"

#include "../page_lock_screen/page_lock_screen.h"
#include "../page_drop_down/page_drop_down.h"
#include "../app_hardware_test/app_hardware_test.h"
#include "../app_settings/app_settings.h"
#include "../app_2048/app_2048.h"
#include "../app_calc/app_calc.h"
#include "../app_memory_game/app_memory_game.h"
#include "../app_sketchpad/app_sketchpad.h"
#include "../app_file_explorer/app_file_explorer.h"
#include "../app_snake/app_snake.h"

/*********************
 *      DEFINES
 *********************/
#if LV_100ASK_GENERIC_UI_SCREEN_SIZE_320X480
#define APP_A_ROW_OF_TOTAL                          2
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_480X480
#define APP_A_ROW_OF_TOTAL                          3
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_1024X600
#define APP_A_ROW_OF_TOTAL                          7
#endif

#if LV_100ASK_GENERIC_UI_HAS_DESKTOP_BG
#if LV_100ASK_GENERIC_UI_SCREEN_SIZE_320X480
LV_IMG_DECLARE(img_desktop_bg_480x480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_480X480
LV_IMG_DECLARE(img_desktop_bg_480x480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_1024X600
LV_IMG_DECLARE(img_desktop_bg_480x480);
#endif
#endif

LV_IMG_DECLARE(img_app_icon_about_us);
LV_IMG_DECLARE(img_app_2048);
LV_IMG_DECLARE(img_app_calc);
LV_IMG_DECLARE(img_app_file_explorer);
LV_IMG_DECLARE(img_app_hardware_test);
LV_IMG_DECLARE(img_app_sketchpad);
LV_IMG_DECLARE(img_app_snake);
LV_IMG_DECLARE(img_app_memory_game);

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    LV_SYSTEM_STATE_SHOW_DESKTOP = 0,
    LV_SYSTEM_STATE_SHOW_DROP_DOWN_PAGE,
}lv_100ask_system_state_t;

typedef struct {
    lv_obj_t * cont_desktop;
    lv_obj_t * cont_drop_down_page;
    lv_100ask_system_state_t system_state;
    lv_100ask_app_data_t * cur_app;
}lv_100ask_desktop_data_t;


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void scroll_recent_app_item_event_cb(lv_event_t * e);
static void scroll_all_app_event_cb(lv_event_t * e);
static void drop_down_apge_event_handler(lv_event_t * e);
static void top_drag_event_handler(lv_event_t * e);
static void buttom_drag_event_handler(lv_event_t * e);
static void desktop_scroll_event(lv_event_t * e);
static void app_icon_event_cb(lv_event_t * e);


/**********************
 *  STATIC VARIABLES
 **********************/
static lv_100ask_desktop_data_t g_lv_100ask_desktop_data;

static lv_100ask_app_data_t * all_app_list[] = {
    &lv_100ask_app_settings_t,          &lv_100ask_app_hardware_test_t,     &lv_100ask_app_2048_t, \
    &lv_100ask_app_calc_t,              &lv_100ask_app_memory_game_t,       &lv_100ask_app_snake_t, \
    &lv_100ask_app_file_explorer_t,     &lv_100ask_app_sketchpad_t
    };

static lv_image_dsc_t * all_app_icon_list[] = {
    &img_app_icon_about_us,     &img_app_hardware_test,     &img_app_2048, \
    &img_app_calc,              &img_app_memory_game,       &img_app_snake, \
    &img_app_file_explorer,     &img_app_sketchpad
};

static lv_100ask_app_data_t * favorites_app_list[] = {
    &lv_100ask_app_settings_t,          &lv_100ask_app_hardware_test_t,     &lv_100ask_app_2048_t, \
    &lv_100ask_app_calc_t,              &lv_100ask_app_memory_game_t,       &lv_100ask_app_snake_t, \
    &lv_100ask_app_file_explorer_t,     &lv_100ask_app_sketchpad_t
    };

static lv_image_dsc_t * favorites_app_icon_list[] = {
    &img_app_icon_about_us,     &img_app_hardware_test,     &img_app_2048, \
    &img_app_calc,              &img_app_memory_game,       &img_app_snake, \
    &img_app_file_explorer,     &img_app_sketchpad
};


/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lv_100ask_generic_ui(void)
{
    //////////////////////////////////////////////////
    lv_100ask_boot_animation(1000);

    g_lv_100ask_desktop_data.system_state = LV_SYSTEM_STATE_SHOW_DESKTOP;

    //////////////////////////////////////////////////
    lv_obj_t * top_gesture_obj = lv_obj_create(lv_layer_sys());
    lv_obj_set_align(top_gesture_obj, LV_ALIGN_TOP_MID);
    lv_obj_set_size(top_gesture_obj, lv_pct(100), 10);
    lv_obj_set_style_opa(top_gesture_obj, LV_OPA_TRANSP, 0);

    lv_obj_t * buttom_gesture_obj = lv_obj_create(lv_layer_sys());
    lv_obj_set_align(buttom_gesture_obj, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_size(buttom_gesture_obj, lv_pct(100), 20);
    lv_obj_set_style_opa(buttom_gesture_obj, LV_OPA_TRANSP, 0);

    ///////////////////////////////////
#if LV_100ASK_GENERIC_UI_HAS_LOCK_SCREEN
    page_lock_screen_init();
    sys_generic_click_check_init();
#endif

    ////////////////////////////////////////////////// page drop down
    g_lv_100ask_desktop_data.cont_drop_down_page = page_drop_down_init();
    lv_obj_add_event_cb(g_lv_100ask_desktop_data.cont_drop_down_page, drop_down_apge_event_handler, LV_EVENT_CLICKED, NULL);

    //////////////////////////////////////////////////
    g_lv_100ask_desktop_data.cont_desktop = lv_obj_create(lv_screen_active());
    lv_obj_remove_style_all(g_lv_100ask_desktop_data.cont_desktop);
    lv_obj_set_size(g_lv_100ask_desktop_data.cont_desktop, LV_PCT(100), LV_PCT(100));
    lv_obj_center(g_lv_100ask_desktop_data.cont_desktop);

    //////////////////////////////////////////////////
#if LV_100ASK_GENERIC_UI_HAS_DESKTOP_BG
#if LV_100ASK_GENERIC_UIE_LIMIT_DESKTOP_BG_PIC
    static lv_color_t grad_colors[2];

    grad_colors[0] = lv_color_make(lv_rand(0, 255), lv_rand(0, 255), lv_rand(0, 255));
    grad_colors[1] = lv_color_make(lv_rand(0, 255), lv_rand(0, 255), lv_rand(0, 255));
    //grad_colors[0] = lv_color_make(rand() % 255, rand() % 255, rand() % 255);
    //grad_colors[1] = lv_color_make(rand() % 255, rand() % 255, rand() % 255);

    int32_t width = lv_display_get_horizontal_resolution(NULL);
    int32_t height = lv_display_get_vertical_resolution(NULL);

    static lv_style_t style_grad_bg;
    lv_style_init(&style_grad_bg);
    lv_style_set_bg_opa(&style_grad_bg, LV_OPA_COVER);

    /*First define a color gradient. In this example we use a purple to black color map.*/
    static lv_grad_dsc_t grad_bg;

    lv_gradient_init_stops(&grad_bg, grad_colors, NULL, NULL, sizeof(grad_colors) / sizeof(lv_color_t));

    /*Make a radial gradient with the center in the middle of the object, extending to the farthest corner*/
    lv_grad_radial_init(&grad_bg, LV_GRAD_CENTER, LV_GRAD_CENTER, LV_GRAD_RIGHT, LV_GRAD_LEFT, LV_GRAD_EXTEND_PAD);

    /*Set gradient as background*/
    lv_style_set_bg_grad(&style_grad_bg, &grad_bg);

    // img_bg
    lv_obj_t * img_bg = lv_obj_create(g_lv_100ask_desktop_data.cont_desktop);
    lv_obj_remove_style_all(img_bg);
    lv_obj_set_size(img_bg, lv_pct(100), lv_pct(100));
    lv_obj_add_style(img_bg, &style_grad_bg, 0);
#else
    lv_obj_t * img_bg = lv_image_create(g_lv_100ask_desktop_data.cont_desktop);
    lv_obj_set_size(img_bg, lv_pct(100), lv_pct(100));
    lv_obj_set_style_opa(img_bg, LV_OPA_70, 0);
    lv_image_set_src(img_bg, &img_desktop_bg_480x480);
#endif

#endif

    //////////////////////////////////////////////////
    lv_obj_t * tv_desktop = lv_tileview_create(g_lv_100ask_desktop_data.cont_desktop);
    lv_obj_set_style_bg_opa(tv_desktop, LV_OPA_TRANSP, 0);
    //lv_obj_add_flag(tv_desktop, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_set_scrollbar_mode(tv_desktop, LV_SCROLLBAR_MODE_OFF);
    //lv_obj_add_event_cb(tv_desktop, desktop_scroll_event, LV_EVENT_GESTURE, tv_desktop);

    lv_obj_add_event_cb(top_gesture_obj, top_drag_event_handler, LV_EVENT_PRESSING, NULL);
    lv_obj_add_event_cb(top_gesture_obj, top_drag_event_handler, LV_EVENT_RELEASED, NULL);
    lv_obj_add_event_cb(buttom_gesture_obj, buttom_drag_event_handler, LV_EVENT_PRESSING, NULL);
    lv_obj_add_event_cb(buttom_gesture_obj, buttom_drag_event_handler, LV_EVENT_RELEASED, NULL);

    //lv_obj_t * img_bg = lv_image_create(tv_desktop);
    //lv_image_set_src(img_bg, &img_desktop_bg_480x480);

    //////////////////////////////////////////////////
    lv_obj_t * cont_primary_desktop = lv_tileview_add_tile(tv_desktop, 0, 0, LV_DIR_BOTTOM);
    lv_obj_set_size(cont_primary_desktop, lv_pct(100), lv_pct(100));
    lv_obj_clear_flag(cont_primary_desktop, LV_OBJ_FLAG_SCROLLABLE);
    //lv_obj_add_flag(cont_primary_desktop, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_tileview_set_tile(tv_desktop, cont_primary_desktop, LV_ANIM_OFF);

    ////////////////////////////////////////////////// 1-1
    lv_obj_t * cont_status_bar = lv_obj_create(cont_primary_desktop);
    lv_obj_remove_style_all(cont_status_bar);
    lv_obj_set_size(cont_status_bar, lv_pct(100), lv_pct(8));
    lv_obj_set_style_text_font(cont_status_bar, &lv_font_montserrat_18, 0);

    lv_obj_t * label_time = lv_label_create(cont_status_bar);
    lv_label_set_text(label_time, "21:00");
    lv_obj_align(label_time, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * label_battery = lv_label_create(cont_status_bar);
    lv_label_set_text(label_battery, LV_SYMBOL_BATTERY_3);
    lv_obj_align(label_battery, LV_ALIGN_RIGHT_MID, -10, 0);

    ////////////////////////////////////////////////// 1-2
    lv_obj_t * cont_recent_app_item = lv_obj_create(cont_primary_desktop);
    lv_obj_remove_style_all(cont_recent_app_item);
    lv_obj_set_size(cont_recent_app_item, lv_pct(100), lv_pct(56));
    lv_obj_align_to(cont_recent_app_item, cont_status_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(cont_recent_app_item, LV_FLEX_FLOW_ROW);
    lv_obj_add_event_cb(cont_recent_app_item, scroll_recent_app_item_event_cb, LV_EVENT_SCROLL, NULL);
    //lv_obj_set_style_clip_corner(cont_recent_app_item, true, 0);
    lv_obj_set_scroll_dir(cont_recent_app_item, LV_DIR_HOR);
    lv_obj_set_scroll_snap_x(cont_recent_app_item, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(cont_recent_app_item, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_column(cont_recent_app_item, 46, 0);
    lv_obj_set_style_pad_ver(cont_recent_app_item, 30, 0);

    uint32_t i;
    lv_obj_t * image;

    uint16_t favorites_app_total = sizeof(favorites_app_list)/sizeof(lv_100ask_app_data_t*);
    for(i = 0; i < favorites_app_total; i++) {
        if((favorites_app_list[i]->open == NULL) || !favorites_app_icon_list[i]) continue;

        image = lv_image_create(cont_recent_app_item);
        //lv_obj_set_size(image, 96, 96);
        lv_image_set_src(image, favorites_app_icon_list[i]);
        lv_obj_set_user_data(image, favorites_app_list[i]);
        lv_obj_add_flag(image, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(image, app_icon_event_cb, LV_EVENT_CLICKED, g_lv_100ask_desktop_data.cont_desktop);
    }

    /*Update the buttons position manually for first*/
    lv_obj_send_event(cont_recent_app_item, LV_EVENT_SCROLL, NULL);

    /*Be sure the fist button is in the middle*/
    if(lv_obj_get_child(cont_recent_app_item, 0))
        lv_obj_scroll_to_view(lv_obj_get_child(cont_recent_app_item, 0), LV_ANIM_OFF);

    ////////////////////////////////////////////////// 1-3
    lv_obj_t * cont_recent_app_info = lv_obj_create(cont_primary_desktop);
    //lv_obj_remove_style_all(cont_recent_app_info);
#if LV_100ASK_GENERIC_UI_SCREEN_SIZE_320X480
    lv_obj_set_size(cont_recent_app_info, 800, 800);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_480X480
    lv_obj_set_size(cont_recent_app_info, 1200, 1200);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_1024X600
    lv_obj_set_size(cont_recent_app_info, 2000, 2000);
#endif
    
    lv_obj_set_style_pad_all(cont_recent_app_info, 0, 0);
    lv_obj_set_style_opa(cont_recent_app_info, LV_OPA_90, 0);
    lv_obj_align_to(cont_recent_app_info, cont_recent_app_item, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    lv_obj_set_style_radius(cont_recent_app_info, LV_RADIUS_CIRCLE, 0);
    //lv_obj_set_style_bg_color(cont_recent_app_info, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_bg_opa(cont_recent_app_info, LV_OPA_COVER, 0);

    lv_obj_t * label_app_title = lv_label_create(cont_recent_app_info);
    lv_obj_set_style_text_font(label_app_title, &lv_font_montserrat_24, 0);
    lv_label_set_text(label_app_title, "100ASK APP");
    lv_obj_align(label_app_title, LV_ALIGN_TOP_MID, 0, 30);

    lv_obj_t * label_app_info = lv_label_create(cont_recent_app_info);
    lv_obj_set_style_text_font(label_app_info, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_align(label_app_info, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(label_app_info, "This is a testing application\nwww.100ask.net");
    lv_obj_align_to(label_app_info, label_app_title, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    lv_obj_t * label_all_app = lv_label_create(cont_recent_app_info);
    lv_obj_set_style_text_font(label_all_app, &lv_font_montserrat_16, 0);
    lv_label_set_text(label_all_app, LV_SYMBOL_UP);
    lv_obj_align_to(label_all_app, label_app_info, LV_ALIGN_OUT_BOTTOM_MID, 0, 26);

    ////////////////////////////////////////////////// 2-1
    lv_obj_t * cont_secondary_desktop = lv_tileview_add_tile(tv_desktop, 0, 1, LV_DIR_NONE);
    //lv_obj_add_flag(cont_secondary_desktop, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_set_flex_flow(cont_secondary_desktop, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont_secondary_desktop, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_event_cb(cont_secondary_desktop, scroll_all_app_event_cb, LV_EVENT_SCROLL, NULL);
    //lv_obj_set_style_clip_corner(cont_secondary_desktop, true, 0);
    lv_obj_set_scroll_dir(cont_secondary_desktop, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(cont_secondary_desktop, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(cont_secondary_desktop, LV_SCROLLBAR_MODE_OFF);
    //lv_obj_set_style_pad_left(cont, 40, 0);
    lv_obj_set_style_pad_all(cont_secondary_desktop, 30, 0);
    lv_obj_set_style_pad_row(cont_secondary_desktop, 46, 0);
    lv_obj_set_style_pad_column(cont_secondary_desktop, 30, 0);
    //lv_obj_set_style_base_dir(cont, LV_BASE_DIR_LTR, 0);
    lv_obj_set_style_bg_opa(cont_secondary_desktop, LV_OPA_COVER, 0);

    uint16_t all_app_valid = sizeof(all_app_list)/sizeof(lv_100ask_app_data_t*);
    uint16_t all_app_invalid = APP_A_ROW_OF_TOTAL - (all_app_valid % APP_A_ROW_OF_TOTAL);
    for(i = 0; i < (all_app_valid + all_app_invalid); i++) {
        if(i >= all_app_valid){
            image = lv_obj_create(cont_secondary_desktop);
            lv_obj_set_size(image, 96, 96);
            lv_obj_clear_flag(image, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_opa(image, LV_OPA_TRANSP, 0);
        }
        else
        {
            if((all_app_list[i]->open == NULL) || !all_app_icon_list[i]) continue;

            image = lv_image_create(cont_secondary_desktop);
            lv_image_set_src(image, all_app_icon_list[i]);
            lv_obj_set_user_data(image, all_app_list[i]);
            lv_obj_add_flag(image, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_event_cb(image, app_icon_event_cb, LV_EVENT_CLICKED, g_lv_100ask_desktop_data.cont_desktop);
        }
    }

    /*Update the buttons position manually for first*/
    lv_obj_send_event(cont_secondary_desktop, LV_EVENT_SCROLL, NULL);

    /*Be sure the fist button is in the middle*/
    if(lv_obj_get_child(cont_secondary_desktop, 0))
        lv_obj_scroll_to_view(lv_obj_get_child(cont_secondary_desktop, 0), LV_ANIM_OFF);

    lv_obj_set_style_bg_opa(cont_primary_desktop, LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_opa(cont_secondary_desktop, LV_OPA_TRANSP, 0);

}

#if 0
void lv_100ask_generic_ui(void)
{
    lv_obj_t * panel = lv_obj_create(lv_screen_active());
    lv_obj_set_size(panel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_scroll_snap_x(panel, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_align(panel, LV_ALIGN_CENTER, 0, 20);

    uint32_t i;
    for(i = 0; i < 10; i++) {
        lv_obj_t * btn = lv_button_create(panel);
        lv_obj_set_size(btn, 64, 64);

        lv_obj_t * label = lv_label_create(btn);

        lv_label_set_text_fmt(label, "Panel %"LV_PRIu32, i);

        lv_obj_center(label);
    }

    lv_obj_update_snap(panel, LV_ANIM_ON);
}
#endif


/**********************
 *  STATIC FUNCTIONS
 **********************/

static void scroll_all_app_event_cb(lv_event_t * e)
{
    lv_obj_t * cont = lv_event_get_target(e);

    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    int32_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;

    int32_t r = lv_obj_get_height(cont) * 60 / 10;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_count(cont);
    for(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        int32_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        int32_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y);

        /*Get the x of diff_y on a circle.*/
        int32_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if(diff_y >= r) {
            x = r;
        }
        else {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000);   /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        //lv_obj_set_style_translate_x(child, x, 0);

        /*Use some opacity with larger translations*/
        lv_opa_t opa = lv_map(x, 0, r, 512, 1024);
        //lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
        //lv_obj_set_style_transform_scale(child, LV_OPA_COVER - opa, 0);

        lv_obj_set_style_transform_width(child, -opa, 0);
        lv_obj_set_style_transform_height(child, -opa, 0);
    }

    //lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);
}


static void desktop_scroll_event(lv_event_t * e)
{
  lv_obj_t * title;
  lv_obj_t * tv = lv_event_get_target(e);
  lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());

  switch(dir) {
    case LV_DIR_LEFT:
      title = lv_obj_get_child(tv, 0);
      lv_tileview_set_tile(tv, title, LV_ANIM_ON);
      LV_LOG_USER("LV_DIR_LEFT");
      break;
    case LV_DIR_RIGHT:
      title = lv_obj_get_child(tv, 0);
      lv_tileview_set_tile(tv, title, LV_ANIM_ON);
      LV_LOG_USER("LV_DIR_RIGHT");
      break;
    case LV_DIR_TOP:
      title = lv_obj_get_child(tv, 1);
      lv_tileview_set_tile(tv, title, LV_ANIM_ON);
      LV_LOG_USER("LV_DIR_TOP");
      break;
    case LV_DIR_BOTTOM:
      title = lv_obj_get_child(tv, 0);
      lv_tileview_set_tile(tv, title, LV_ANIM_ON);
      LV_LOG_USER("LV_DIR_BOTTOM");
      break;
  }
}

static void drop_down_apge_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);

#if LV_100ASK_GENERIC_UI_SCREEN_SIZE_320X480
    lv_obj_set_y(obj, -480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_480X480
    lv_obj_set_y(obj, -480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_1024X600
    lv_obj_set_y(obj, -600);
#endif
    g_lv_100ask_desktop_data.system_state = LV_SYSTEM_STATE_SHOW_DESKTOP;
}

static void top_drag_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj_page_dorp_down = g_lv_100ask_desktop_data.cont_drop_down_page;

    if(code == LV_EVENT_PRESSING)
    {
        lv_indev_t * indev = lv_indev_active();
        if(indev == NULL)  return;

        lv_point_t vect;
        lv_indev_get_vect(indev, &vect);

        int32_t y = lv_obj_get_y_aligned(obj) + vect.y;
        lv_obj_set_y(obj, y);
#if LV_100ASK_GENERIC_UI_SCREEN_SIZE_320X480
    lv_obj_set_y(obj_page_dorp_down, y-480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_480X480
    lv_obj_set_y(obj_page_dorp_down, y-480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_1024X600
    lv_obj_set_y(obj_page_dorp_down, y-600);
#endif
        
    }
    else if(code == LV_EVENT_RELEASED)
    {
        int32_t y = lv_obj_get_y_aligned(obj);
        if(y >= 160)
        {
            lv_obj_set_y(obj_page_dorp_down, 0);
            lv_obj_set_y(obj, 0);
            g_lv_100ask_desktop_data.system_state = LV_SYSTEM_STATE_SHOW_DROP_DOWN_PAGE;
            return;
        }

        lv_obj_set_y(obj, 0);

        if(g_lv_100ask_desktop_data.system_state == LV_SYSTEM_STATE_SHOW_DESKTOP)
#if LV_100ASK_GENERIC_UI_SCREEN_SIZE_320X480
            lv_obj_set_y(obj_page_dorp_down, -480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_480X480
            lv_obj_set_y(obj_page_dorp_down, -480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_1024X600
            lv_obj_set_y(obj_page_dorp_down, -600);
#endif
            
    }
}


static void buttom_drag_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * cont_desktop = g_lv_100ask_desktop_data.cont_desktop;
    lv_obj_t * obj_page_dorp_down = g_lv_100ask_desktop_data.cont_drop_down_page;

    if(code == LV_EVENT_PRESSING)
    {
        lv_indev_t * indev = lv_indev_active();
        if(indev == NULL)  return;

        lv_point_t vect;
        lv_indev_get_vect(indev, &vect);

        int32_t y = lv_obj_get_y_aligned(obj) + vect.y;
        lv_obj_set_y(obj, y);

        if(g_lv_100ask_desktop_data.system_state == LV_SYSTEM_STATE_SHOW_DROP_DOWN_PAGE)
            lv_obj_set_y(obj_page_dorp_down, y);
    }
    else if(code == LV_EVENT_RELEASED)
    {
        int32_t y = lv_obj_get_y_aligned(obj);

        if(y <= -120)
        {
            if(g_lv_100ask_desktop_data.system_state == LV_SYSTEM_STATE_SHOW_DESKTOP)
            {
                if(lv_obj_has_flag(cont_desktop, LV_OBJ_FLAG_HIDDEN))
                {
                    lv_obj_clear_flag(cont_desktop, LV_OBJ_FLAG_HIDDEN);
                    g_lv_100ask_desktop_data.cur_app->close();
                }
                else
                {
                    int32_t index;
                    lv_obj_t * title;
                    lv_obj_t * tv;
#if LV_100ASK_GENERIC_UI_HAS_DESKTOP_BG
                    tv = lv_obj_get_child(cont_desktop, 1);
#else
                    tv = lv_obj_get_child(cont_desktop, 0);
#endif
                    title = lv_tileview_get_tile_active(tv);
                    index = lv_obj_get_index(title);

                    if(index == 0)
                    {
                        title = lv_obj_get_child(tv, 1);
                        lv_tileview_set_tile(tv, title, LV_ANIM_ON);
                    }
                    else if(index == 1)
                    {
                        title = lv_obj_get_child(tv, 0);
                        lv_tileview_set_tile(tv, title, LV_ANIM_ON);
                    }
                }
            }
            else if(g_lv_100ask_desktop_data.system_state == LV_SYSTEM_STATE_SHOW_DROP_DOWN_PAGE)
            {
#if LV_100ASK_GENERIC_UI_SCREEN_SIZE_320X480
            lv_obj_set_y(obj_page_dorp_down, -480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_480X480
            lv_obj_set_y(obj_page_dorp_down, -480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_1024X600
            lv_obj_set_y(obj_page_dorp_down, -600);
#endif
                g_lv_100ask_desktop_data.system_state = LV_SYSTEM_STATE_SHOW_DESKTOP;
            }

            lv_obj_set_y(obj, 0);
            return;
        }

        lv_obj_set_y(obj, 0);

        if(g_lv_100ask_desktop_data.system_state == LV_SYSTEM_STATE_SHOW_DROP_DOWN_PAGE)
#if LV_100ASK_GENERIC_UI_SCREEN_SIZE_320X480
            lv_obj_set_y(obj_page_dorp_down, 480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_480X480
            lv_obj_set_y(obj_page_dorp_down, 480);
#elif LV_100ASK_GENERIC_UI_SCREEN_SIZE_1024X600
            lv_obj_set_y(obj_page_dorp_down, 600);
#endif
    }
}



static void scroll_recent_app_item_event_cb(lv_event_t * e)
{
    lv_obj_t * cont = lv_event_get_target(e);

    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    int32_t cont_x_center = cont_a.x1 + lv_area_get_width(&cont_a) / 2;

    //int32_t r = lv_obj_get_height(cont) * 7 / 10;
    int32_t r = lv_obj_get_height(cont) * 7 / 9;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_count(cont);
    for(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        int32_t child_x_center = child_a.x1 + lv_area_get_width(&child_a) / 2;

        int32_t diff_x = child_x_center - cont_x_center;
        diff_x = LV_ABS(diff_x);

        /*Get the x of diff_y on a circle.*/
        int32_t y;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if(diff_x >= r) {
            y = r;
        }
        else {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t y_sqr = r * r - diff_x * diff_x;
            lv_sqrt_res_t res;
            lv_sqrt(y_sqr, &res, 0x8000);   /*Use lvgl's built in sqrt root function*/
            y = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        lv_obj_set_style_translate_y(child, y, 0);

        /*Use some opacity with larger translations*/
        lv_opa_t opa = lv_map(y, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
        //lv_obj_set_style_transform_scale(child, LV_OPA_COVER - opa, 0);
    }
}


static void app_icon_event_cb(lv_event_t * e)
{
    lv_obj_t * app_obj = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * cont_desktop = lv_event_get_user_data(e);

    lv_100ask_app_data_t * app_data = lv_obj_get_user_data(app_obj);
    g_lv_100ask_desktop_data.cur_app = app_data;

    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
        lv_obj_add_flag(cont_desktop, LV_OBJ_FLAG_HIDDEN);
        g_lv_100ask_desktop_data.cur_app->open();
    }
}
