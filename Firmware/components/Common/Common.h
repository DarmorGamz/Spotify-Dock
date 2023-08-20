 /*****************************************************************************
 * @file        Common.h
 * @copyright   COPYRIGHT (c) 2023 Darmor Inc. All rights reserved.
 * @author      Darren Morrison
 * @brief       Header file for Common related functionality.
 ******************************************************************************/
#ifndef COMMON_H_
#define COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/** INCLUDES ******************************************************************/
#include "esp_log.h"
#include "esp_err.h"


/** COMMON CONSTANT DEFINITIONS ***********************************************/
#define LCD_PIN_BKL 14
#define LCD_PIN_DC 8
#define LCD_PIN_RST 9
#define LCD_PIN_CS 10
#define LCD_PIN_SCK 12
#define LCD_PIN_MISO 13
#define LCD_PIN_MOSI 11


/** COMMON MACRO DEFINITIONS **************************************************/


/** TYPEDEFS, STRUCTURES AND ENUMERATIONS *************************************/


/** PUBLIC FUNCTION PROTOTYPES ************************************************/


/** GLOBAL VARIABLES (SHARED ACROSS CODE MODULES) *****************************/


/** DEBUG *********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* COMMON_H_ */