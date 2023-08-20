 /*****************************************************************************
 * @file        main.c
 * @copyright   COPYRIGHT (c) 2023 Darmor Inc. All rights reserved.
 * @author      Darren Morrison
 * @brief       Source file for main state machine related functionality.
 ******************************************************************************/
/** INCLUDES ******************************************************************/
#include "Common.h"
#include "Wifi.h"
#include "Display.h"
#include "Lvgl.h"


/** LOCAL (PRIVATE) CONSTANT AND MACRO DEFINITIONS ****************************/
static const char *TAG = "MAIN";


void app_main() {
    // Initalize Wifi.
    if (Wifi_Init() != ESP_OK) { ESP_LOGI(TAG, "Wifi failed to initalize."); }

    // Initalize the Display.
    if (Display_Init() != ESP_OK) { ESP_LOGI(TAG, "Wifi failed to initalize."); }
    if (Lvgl_Init() != ESP_OK) { ESP_LOGI(TAG, "Wifi failed to initalize."); }

    // Start Wifi.
    if (Wifi_Start() != ESP_OK) { ESP_LOGI(TAG, "Wifi failed to start."); }
}