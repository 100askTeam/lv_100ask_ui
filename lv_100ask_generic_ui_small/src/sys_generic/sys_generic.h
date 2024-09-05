#ifndef __SYS_GENERIC_H__
#define __SYS_GENERIC_H__

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/
#include "../lv_100ask_generic_ui_small.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    lv_obj_t * cont;
    uint8_t name[16];
    uint8_t desc[32];
    int16_t (*open)(void);
    int16_t (*close)(void);
}lv_100ask_app_data_t;


/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_100ask_boot_animation(uint32_t delay);

void sys_generic_click_check_init(void);

void sys_generic_set_sound(int16_t sound);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __SYS_GENERIC_H__ */

