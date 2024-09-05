/**
 ******************************************************************************
 * @file    app_settings.c
 * @author  百问科技
 * @version V1.0
 * @date    2024-8-14
 * @brief	app_settings
 ******************************************************************************
 * Change Logs:
 * Date           Author          Notes
 * 2024-8-14     zhouyuebiao     First version
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


#include "app_settings.h"
#include "../page_lock_screen/page_lock_screen.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2,
    LV_MENU_ITEM_BUILDER_VARIANT_3,
    LV_MENU_ITEM_BUILDER_VARIANT_4
} lv_menu_builder_variant_t;


typedef struct {
    lv_obj_t * obj;
}lv_100ask_app_settings_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static int16_t app_settings_open(void);
static int16_t app_settings_close(void);

static void switch_handler(lv_event_t * e);
static lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt,
                              lv_menu_builder_variant_t builder_variant);
static lv_obj_t * create_slider(lv_obj_t * parent,
                                const char * icon, const char * txt, int32_t min, int32_t max, int32_t val);
static lv_obj_t * create_switch(lv_obj_t * parent,
                                const char * icon, const char * txt, bool chk);

///////////////////
static void sw_switch_theme_event_handler(lv_event_t * e);
static void sw_sound_event_handler(lv_event_t * e);
static void slider_set_brightness_event_handler(lv_event_t * e);
static void btn_list_set_auto_lock_event_handler(lv_event_t * e);

static void sw_sysmon_memory_event_handler(lv_event_t * e);
static void sw_sysmon_performance_event_handler(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_100ask_app_data_t lv_100ask_app_settings_t = {
    .cont  = NULL,
    .name  = "Settings",
    .desc  = "System Settings",
    .open  = app_settings_open,
    .close = app_settings_close
};

static lv_100ask_app_settings_data_t g_lv_100ask_app_settings_data;

static lv_obj_t * root_page;

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
static int16_t app_settings_open(void)
{
    lv_obj_t * cont = lv_obj_create(lv_screen_active());
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));

    lv_obj_t * menu = lv_menu_create(cont);
    lv_obj_set_style_text_font(menu, &lv_font_montserrat_16, 0);

    lv_color_t bg_color = lv_obj_get_style_bg_color(menu, 0);
    if(lv_color_brightness(bg_color) > 127) {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 10), 0);
    }
    else {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 50), 0);
    }

    lv_obj_set_size(menu, lv_pct(100), lv_pct(100));
    lv_obj_center(menu);

    uint16_t index;
    lv_obj_t * sw;
    lv_obj_t * list;
    lv_obj_t * cont_sub;
    lv_obj_t * cont_tmp;
    lv_obj_t * section;
    lv_obj_t * label;
    lv_obj_t * slider;

    /*Create sub pages*/
    lv_obj_t * sub_appearance_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_appearance_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_appearance_page);
    section = lv_menu_section_create(sub_appearance_page);
    cont_sub = create_switch(section, LV_SYMBOL_EDIT, "Dark mode", false);
    lv_obj_add_event_cb(lv_obj_get_child(cont_sub, -1), sw_switch_theme_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t * sub_sound_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_sound_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_sound_page);
    section = lv_menu_section_create(sub_sound_page);
    cont_sub = create_switch(section, LV_SYMBOL_AUDIO, "Sound", true);
    lv_obj_add_event_cb(lv_obj_get_child(cont_sub, -1), sw_sound_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t * sub_auto_lock_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_auto_lock_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    section = lv_menu_section_create(sub_auto_lock_page);
    cont_tmp = lv_menu_cont_create(section);
    list = lv_list_create(cont_tmp);
    lv_obj_set_height(list, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(list, 0, 0);
    lv_obj_set_flex_grow(list, 1);
    lv_obj_set_style_bg_opa(list, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    //lv_obj_set_style_base_dir(list, LV_BASE_DIR_RTL, 0);
    lv_obj_t * btn;
    btn = lv_list_add_button(list, NULL, "5 seconds");
    lv_obj_add_event_cb(btn, btn_list_set_auto_lock_event_handler, LV_EVENT_CLICKED, list);
    btn = lv_list_add_button(list, NULL, "30 seconds");
    lv_obj_add_event_cb(btn, btn_list_set_auto_lock_event_handler, LV_EVENT_CLICKED, list);
    btn = lv_list_add_button(list, NULL, "1 minute");
    lv_obj_add_event_cb(btn, btn_list_set_auto_lock_event_handler, LV_EVENT_CLICKED, list);
    btn = lv_list_add_button(list, NULL, "2 minute");
    lv_obj_add_event_cb(btn, btn_list_set_auto_lock_event_handler, LV_EVENT_CLICKED, list);
    btn = lv_list_add_button(list, NULL, "3 minute");
    lv_obj_add_event_cb(btn, btn_list_set_auto_lock_event_handler, LV_EVENT_CLICKED, list);
    btn = lv_list_add_button(list, NULL, "4 minute");
    lv_obj_add_event_cb(btn, btn_list_set_auto_lock_event_handler, LV_EVENT_CLICKED, list);
    btn = lv_list_add_button(list, NULL, "5 minute");
    lv_obj_add_event_cb(btn, btn_list_set_auto_lock_event_handler, LV_EVENT_CLICKED, list);
    btn = lv_list_add_button(list, NULL, "Never");
    lv_obj_add_event_cb(btn, btn_list_set_auto_lock_event_handler, LV_EVENT_CLICKED, list);

    ////////////////////////////// About
    lv_obj_t * sub_software_info_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_software_info_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    section = lv_menu_section_create(sub_software_info_page);
    cont_tmp = lv_menu_cont_create(section);

    list = lv_list_create(cont_tmp);
    lv_obj_set_height(list, LV_SIZE_CONTENT);
    lv_obj_set_style_pad_all(list, 0, 0);
    lv_obj_set_flex_grow(list, 1);
    lv_obj_set_style_bg_opa(list, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(list, 0, 0);
    //lv_obj_set_style_base_dir(list, LV_BASE_DIR_RTL, 0);
    char * item1[] = {"Name", "Software Version", "Model Nmae", "Model chip", "Serial Number"};
    char * item2[] = {"100ASK", "V1.0", "DshanMCU-H7R", "STM32H7R7L8H6", "A1B2C3D4E5F"};
    for(index = 0; index < 5; index++)
    {
        btn = lv_list_add_button(list, NULL, item1[index]);
        label = lv_label_create(btn);
        lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_GREY), 0);
        lv_label_set_text(label, item2[index]);
    }


    //create_text(section, NULL, "Version 1.0\n\n""Docs:  https://lvgl.100ask.net\nForum:  https://forums.100ask.net", LV_MENU_ITEM_BUILDER_VARIANT_1);

    lv_obj_t * qr;
    section = lv_menu_section_create(sub_software_info_page);
    lv_obj_set_style_bg_opa(section, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(section, 8, 0);
    lv_obj_set_style_pad_column(section, 20, 0);
    lv_obj_set_flex_flow(section, LV_FLEX_FLOW_ROW);
    //lv_obj_set_flex_align(section, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    cont_tmp = lv_menu_cont_create(section);
    lv_obj_remove_style_all(cont_tmp);
    lv_obj_set_size(cont_tmp, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont_tmp, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont_tmp, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    qr = lv_qrcode_create(cont_tmp);
    lv_qrcode_set_size(qr, 80);
    lv_qrcode_update(qr, "https://lvgl.100ask.net", strlen("https://lvgl.100ask.net"));
    lv_obj_center(qr);
    label = lv_label_create(cont_tmp);
    lv_label_set_text(label, "Docs");

    cont_tmp = lv_menu_cont_create(section);
    lv_obj_remove_style_all(cont_tmp);
    lv_obj_set_size(cont_tmp, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont_tmp, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont_tmp, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    qr = lv_qrcode_create(cont_tmp);
    lv_qrcode_set_size(qr, 80);
    lv_qrcode_update(qr, "https://forums.100ask.net", strlen("https://forums.100ask.net"));
    lv_obj_center(qr);
    label = lv_label_create(cont_tmp);
    lv_label_set_text(label, "Forum");

    lv_obj_t * sub_legal_info_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_legal_info_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    section = lv_menu_section_create(sub_legal_info_page);
    lv_obj_set_style_bg_opa(section, LV_OPA_TRANSP, 0);
    create_text(section, NULL,
                "MIT licence\n\n"
                "Copyright (c) 2024 Shenzhen 100ASK Technology Co., Ltd\n\n"
                "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the \"Software\"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:\n\n"
                "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\n\n"
                "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
                ,LV_MENU_ITEM_BUILDER_VARIANT_4);

    ///////////////////////// set_load_page_event
    // Auto-Lock
    lv_obj_t * sub_display_brightness_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_display_brightness_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_display_brightness_page);
    section = lv_menu_section_create(sub_display_brightness_page);

    slider = create_slider(section, LV_SYMBOL_EYE_OPEN, "Brightness", 0, 150, 100);
    lv_slider_set_range(slider, LCD_MIN_BACKLIGHT, LCD_MAX_BACKLIGHT);
    lv_slider_set_value(slider, 1000, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, slider_set_brightness_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    cont_sub = create_text(section, LV_SYMBOL_RIGHT, "Auto-Lock", LV_MENU_ITEM_BUILDER_VARIANT_3);
    lv_menu_set_load_page_event(menu, cont_sub, sub_auto_lock_page);

    cont_sub = create_switch(section, LV_SYMBOL_SETTINGS, "System performance monitor", true);
    lv_obj_add_event_cb(lv_obj_get_child(cont_sub, -1), sw_sysmon_performance_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    cont_sub = create_switch(section, LV_SYMBOL_SETTINGS, "System memory monitor", true);
    lv_obj_add_event_cb(lv_obj_get_child(cont_sub, -1), sw_sysmon_memory_event_handler, LV_EVENT_VALUE_CHANGED, NULL);


    // about
    lv_obj_t * sub_about_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_about_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_about_page);
    section = lv_menu_section_create(sub_about_page);
    cont_sub = create_text(section, NULL, "Software information", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont_sub, sub_software_info_page);
    cont_sub = create_text(section, NULL, "Legal information", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont_sub, sub_legal_info_page);

    // memu mode
    lv_obj_t * sub_menu_mode_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(sub_menu_mode_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_menu_mode_page);
    section = lv_menu_section_create(sub_menu_mode_page);
    cont_sub = create_switch(section, LV_SYMBOL_REFRESH, "Sidebar enable", true);
    lv_obj_add_event_cb(lv_obj_get_child(cont_sub, 2), switch_handler, LV_EVENT_VALUE_CHANGED, menu);

    /*Create a root page*/
    root_page = lv_menu_page_create(menu, "Settings");
    //root_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(root_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    section = lv_menu_section_create(root_page);
    cont_sub = create_text(section, LV_SYMBOL_EDIT, "Appearance", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont_sub, sub_appearance_page);
    cont_sub = create_text(section, LV_SYMBOL_AUDIO, "Sound", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont_sub, sub_sound_page);
    cont_sub = create_text(section, LV_SYMBOL_EYE_OPEN, "Display & Brightness", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont_sub, sub_display_brightness_page);

    create_text(root_page, NULL, "Others", LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(root_page);
    cont_sub = create_text(section, LV_SYMBOL_HOME, "About", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont_sub, sub_about_page);
    cont_sub = create_text(section, LV_SYMBOL_REFRESH, "Menu mode", LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont_sub, sub_menu_mode_page);

    lv_menu_set_sidebar_page(menu, root_page);

    lv_obj_send_event(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 0), 0), LV_EVENT_CLICKED,
                      NULL);

    lv_100ask_app_settings_t.cont = cont;
    return 0;
}


/// @brief
/// @param
/// @return
static int16_t app_settings_close(void)
{
    lv_obj_delete(lv_100ask_app_settings_t.cont);
    return 0;
}


/// @brief
/// @param e
static void switch_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * menu = lv_event_get_user_data(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        if(lv_obj_has_state(obj, LV_STATE_CHECKED)) {
            lv_menu_set_page(menu, NULL);
            lv_menu_set_sidebar_page(menu, root_page);
            lv_obj_send_event(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 0), 0), LV_EVENT_CLICKED,
                              NULL);
        }
        else {
            lv_menu_set_sidebar_page(menu, NULL);
            lv_menu_clear_history(menu); /* Clear history because we will be showing the root page later */
            lv_menu_set_page(menu, root_page);
        }
    }
}


/// @brief
/// @param parent
/// @param icon
/// @param txt
/// @param builder_variant
/// @return
static lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt,
                              lv_menu_builder_variant_t builder_variant)
{
    lv_obj_t * obj = lv_menu_cont_create(parent);

    lv_obj_t * img = NULL;
    lv_obj_t * label = NULL;

    if(icon) {
        img = lv_image_create(obj);
        lv_image_set_src(img, icon);
    }

    if(txt) {
        label = lv_label_create(obj);
        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
    }

    if(builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt) {
        lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_swap(img, label);
    }

    if(builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_3 && icon && txt) {
        lv_obj_swap(img, label);
    }

    if(builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_4 && txt) {
        lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    }

    return obj;
}

/// @brief
/// @param parent
/// @param icon
/// @param txt
/// @param min
/// @param max
/// @param val
/// @return
static lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max,
                                int32_t val)
{
    lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2);

    lv_obj_t * slider = lv_slider_create(obj);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_OFF);

    if(icon == NULL) {
        lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }

    return slider;
}

/// @brief
/// @param parent
/// @param icon
/// @param txt
/// @param chk
/// @return
static lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool chk)
{
    lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);

    lv_obj_t * sw = lv_switch_create(obj);
    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

    return obj;
}


/**********************
 *  Setting FUNCTIONS
 **********************/

static void sw_switch_theme_event_handler(lv_event_t * e)
{
    bool theme_mode = true;

    lv_obj_t * sw = lv_event_get_target(e);

    if(lv_obj_has_state(sw, LV_STATE_CHECKED))  theme_mode = true;
    else                                        theme_mode = false;

    lv_theme_t * th = lv_theme_default_init(lv_disp_get_default(),  /*Use the DPI, size, etc from this display*/
                                            lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_CYAN),   /*Primary and secondary palette*/
                                            theme_mode,    /*Light or dark mode*/
                                            LV_FONT_DEFAULT);

    lv_disp_set_theme(lv_disp_get_default(), th); /*Assign the theme to the display*/
}

static void sw_sound_event_handler(lv_event_t * e)
{
    lv_obj_t * sw = lv_event_get_target(e);

    if(lv_obj_has_state(sw, LV_STATE_CHECKED))
    {
        sys_generic_set_sound(1);
    }
    else sys_generic_set_sound(0);
}

static void slider_set_brightness_event_handler(lv_event_t * e)
{
    int32_t slider_value = 0;
    lv_obj_t *slider = lv_event_get_current_target(e);

    slider_value = lv_slider_get_value(slider);

#if LV_100ASK_DESKTOP_USE_DshanMCUH7R_NoRTOS
    lcd_backlight_set_value(slider_value);
#endif
}


static void btn_list_set_auto_lock_event_handler(lv_event_t * e)
{
    // 5 seconds, 30 seconds, 1 minute, 2 minute, 3 minute, 4 minute, 5 minute, Never
    // 0 - 7
    static int32_t last_btn_index = -1;
    lv_obj_t * img;
    int32_t btn_index = 0;

    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * list = lv_event_get_user_data(e);

    btn_index = lv_obj_get_index(btn);

    if(last_btn_index != -1)
    {
        img = lv_obj_get_child(list, last_btn_index);
        if(lv_obj_get_child_count(img) == 2)
        {
            img = lv_obj_get_child(img, -1);
            lv_obj_del(img);
        }
    }

    img = lv_image_create(btn);
    lv_image_set_src(img, LV_SYMBOL_OK);
#if LV_100ASK_HAS_LOCK_SCREEN
    switch (btn_index)
    {
    case 0:
        set_page_lock_screen_time(5 * 1000);
        break;
    case 1:
        set_page_lock_screen_time(30 * 1000);
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        set_page_lock_screen_time((btn_index - 1) * 60 * 1000);
        break;
    case 7:
        set_page_lock_screen_time(0);
        break;
    default:
        break;
    }
#endif
    last_btn_index = btn_index;

}

static void sw_sysmon_performance_event_handler(lv_event_t * e)
{
    lv_obj_t * sw = lv_event_get_target(e);

    lv_display_t * disp = lv_display_get_default();
    if(disp == NULL) {
        LV_LOG_WARN("There is no default display");
        return;
    }

    if(lv_obj_has_state(sw, LV_STATE_CHECKED))
    {
        lv_sysmon_show_performance(disp);
    }
    else
    {
        lv_sysmon_hide_performance(disp);
    }
}

static void sw_sysmon_memory_event_handler(lv_event_t * e)
{
    lv_obj_t * sw = lv_event_get_target(e);

    lv_display_t * disp = lv_display_get_default();
    if(disp == NULL) {
        LV_LOG_WARN("There is no default display");
        return;
    }

    if(lv_obj_has_state(sw, LV_STATE_CHECKED))
    {
        lv_sysmon_show_memory(disp);
    }
    else
    {
        lv_sysmon_hide_memory(disp);
    }
}

