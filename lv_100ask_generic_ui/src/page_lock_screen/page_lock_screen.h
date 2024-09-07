#ifndef __PAGE_LOCK_SCREEN_H__
#define __PAGE_LOCK_SCREEN_H__

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/
#include "../lv_100ask_generic_ui.h"

#if LV_100ASK_GENERIC_UI_HAS_LOCK_SCREEN != 0

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/
lv_obj_t * page_lock_screen_init(void);

// ms
void set_page_lock_screen_time(uint32_t period);

void set_page_lock_screen_reset(void);

#endif /* LV_100ASK_GENERIC_UI_HAS_LOCK_SCREEN */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __PAGE_LOCK_SCREEN_H__ */

