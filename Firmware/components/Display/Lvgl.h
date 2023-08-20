 /*****************************************************************************
 * @file        Lvgl.h
 * @copyright   COPYRIGHT (c) 2023 Darmor Inc. All rights reserved.
 * @author      Darren Morrison
 * @brief       Header file for Lvgl related functionality.
 ******************************************************************************/
#ifndef LVGL_H_
#define LVGL_H_

#ifdef __cplusplus
extern "C" {
#endif

/** INCLUDES ******************************************************************/
#include "Common.h"
#include "Display.h"
#include <lvgl.h>


/** COMMON CONSTANT DEFINITIONS ***********************************************/
__attribute__((unused)) static const size_t LV_BUFFER_SIZE = DISPLAY_HORIZONTAL_PIXELS * 50;
__attribute__((unused)) static const int LVGL_UPDATE_PERIOD_MS = 5;


/** COMMON MACRO DEFINITIONS **************************************************/


/** TYPEDEFS, STRUCTURES AND ENUMERATIONS *************************************/


/** PUBLIC FUNCTION PROTOTYPES ************************************************/
esp_err_t Lvgl_Init(void);
bool Lvgl_Notify_Flush_Ready(esp_lcd_panel_io_handle_t, esp_lcd_panel_io_event_data_t *, void *);


/** GLOBAL VARIABLES (SHARED ACROSS CODE MODULES) *****************************/
extern lv_disp_drv_t lv_disp_drv;


/** DEBUG *********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* LVGL_H_ */