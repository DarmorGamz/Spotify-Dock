 /*****************************************************************************
 * @file        Wifi.h
 * @copyright   COPYRIGHT (c) 2023 Darmor Inc. All rights reserved.
 * @author      Darren Morrison
 * @brief       Header file for WI-FI related functionality.
 ******************************************************************************/
#ifndef WIFI_H_
#define WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif

/** INCLUDES ******************************************************************/
#include "Common.h"


/** COMMON CONSTANT DEFINITIONS ***********************************************/
#define WIFI_SSID "E2"
#define WIFI_PASSWORD "EyedroOne"


/** COMMON MACRO DEFINITIONS **************************************************/


/** TYPEDEFS, STRUCTURES AND ENUMERATIONS *************************************/


/** PUBLIC FUNCTION PROTOTYPES ************************************************/
esp_err_t Wifi_Init(void);
esp_err_t Wifi_Start(void);
esp_err_t Wifi_Stop(void);


/** GLOBAL VARIABLES (SHARED ACROSS CODE MODULES) *****************************/


/** DEBUG *********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* WIFI_H_ */