/**
 * @file lv_100ask_generic_ui_small.h
 *
 */

#ifndef LV_100ASK_GENERIC_UI_SMALLE_H
#define LV_100ASK_GENERIC_UI_SMALLE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../../lvgl/lvgl.h"
#endif

#if defined(LV_100ASK_GENERIC_UI_CONF_PATH)
#define __LV_TO_STR_AUX(x) #x
#define __LV_TO_STR(x) __LV_TO_STR_AUX(x)
#include __LV_TO_STR(LV_100ASK_GENERIC_UI_CONF_PATH)
#undef __LV_TO_STR_AUX
#undef __LV_TO_STR
#elif defined(LV_100ASK_GENERIC_UI_CONF_INCLUDE_SIMPLE)
#include "lv_100ask_generic_ui_small_conf.h"
#else
#include "../../lv_100ask_generic_ui_small_conf.h"
#endif

#include "src/main/lv_100ask_generic_ui_small_main.h"


#include "src/sys_generic/sys_generic.h"

/*********************
 *      DEFINES
 *********************/
/*Test  lvgl version*/
#if LV_VERSION_CHECK(9, 2, 0) == 0
#warning "lv_100ask_generic_ui_small: Wrong lvgl version"
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_100ASK_GENERIC_UI_SMALLE_H*/
