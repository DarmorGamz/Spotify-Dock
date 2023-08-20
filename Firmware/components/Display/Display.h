 /*****************************************************************************
 * @file        Display.h
 * @copyright   COPYRIGHT (c) 2023 Darmor Inc. All rights reserved.
 * @author      Darren Morrison
 * @brief       Header file for Display related functionality.
 ******************************************************************************/
#ifndef DISPLAY_H_
#define DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

/** INCLUDES ******************************************************************/
#include "Common.h"
#include <driver/ledc.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>


/** COMMON CONSTANT DEFINITIONS ***********************************************/
__attribute__((unused)) static const int DISPLAY_HORIZONTAL_PIXELS = 480;
__attribute__((unused)) static const int DISPLAY_VERTICAL_PIXELS = 320;


/** COMMON MACRO DEFINITIONS **************************************************/


/** TYPEDEFS, STRUCTURES AND ENUMERATIONS *************************************/


/** PUBLIC FUNCTION PROTOTYPES ************************************************/
esp_err_t Display_Init(void);


/** GLOBAL VARIABLES (SHARED ACROSS CODE MODULES) *****************************/
extern esp_lcd_panel_handle_t lcd_handle;


/** DEBUG *********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_H_ */