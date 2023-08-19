 /*****************************************************************************
 * @file        Wifi.c
 * @copyright   COPYRIGHT (c) 2023 Darmor Inc. All rights reserved.
 * @author      Darren Morrison
 * @brief       Source file for WI-FI related functionality.
 ******************************************************************************/
/** INCLUDES ******************************************************************/
#include "Wifi.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include <string.h>

/** LOCAL (PRIVATE) CONSTANT AND MACRO DEFINITIONS ****************************/
__attribute__((unused)) static const char *DEBUG_TAG = "WIFI";


/** LOCAL (PRIVATE) TYPEDEFS, STRUCTURES AND ENUMERATIONS *********************/


/** VARIABLES *****************************************************************/
wifi_config_t stWifiConfig = {
    .sta = {
        .ssid = {0},
        .password = {0},
        .channel = 0,
    },
};


/** LOCAL (PRIVATE) FUNCTION PROTOTYPES ***************************************/
void _Callback_WiFiEvents(void *arg, esp_event_base_t event_base, int32_t eventId, void *event_data);


/** PUBLIC FUNCTION IMPLEMENTATIONS *******************************************/
esp_err_t Wifi_Init(void) {
    // Initialize TCP/IP network interface (should be called only once in application)
    if (esp_netif_init() != ESP_OK) { ESP_LOGI(DEBUG_TAG, "NETIF already initalized."); return !ESP_OK; }

    // Create default event loop needed by all interfaces.
    if (esp_event_loop_create_default() != ESP_OK) { ESP_LOGI(DEBUG_TAG, "NETIF already initalized."); return !ESP_OK; }

    // Initialize NVS needed by Wi-Fi this is to prevent calibrating RF every restart.
    if (nvs_flash_init() == (ESP_ERR_NVS_NO_FREE_PAGES - ESP_ERR_NVS_BASE)) {
        if (nvs_flash_erase()!= ESP_OK) { ESP_LOGI(DEBUG_TAG, "NVS flash failed to erase. "); return !ESP_OK; }
        if (nvs_flash_init()!= ESP_OK) { ESP_LOGI(DEBUG_TAG, "NVS flash failed to init. "); return !ESP_OK; }
    }

    // Create default config.
    wifi_init_config_t stWifiInitConfig = WIFI_INIT_CONFIG_DEFAULT();
    if (esp_wifi_init(&stWifiInitConfig) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Wifi failed to initalize. "); return !ESP_OK; }

    // Create Netif instance.
    esp_netif_config_t stNetifConfig = ESP_NETIF_DEFAULT_WIFI_STA();
    esp_netif_t *stNetifHandle = NULL;
    if ((stNetifHandle = esp_netif_new(&stNetifConfig)) == NULL) { ESP_LOGI(DEBUG_TAG, "Wifi Sta failed to Initalized."); return !ESP_OK; }

    // Register event handers
    if (esp_wifi_set_default_wifi_sta_handlers() != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to set default AP handlers. "); return !ESP_OK; }
    if (esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_START, &_Callback_WiFiEvents, NULL) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to add AP handlers. "); return !ESP_OK; }
    if (esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_STOP, &_Callback_WiFiEvents, NULL) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to add AP handlers. "); return !ESP_OK; }
    if (esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_CONNECTED, &_Callback_WiFiEvents, NULL) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to add AP handlers. "); return !ESP_OK; }
    if (esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &_Callback_WiFiEvents, NULL) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to add AP handlers. "); return !ESP_OK; }
    if (esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_AUTHMODE_CHANGE, &_Callback_WiFiEvents, NULL) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to add AP handlers. "); return !ESP_OK; }
	if (esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_SCAN_DONE, &_Callback_WiFiEvents, NULL) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to add AP handlers. "); return !ESP_OK; }
    if (esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &_Callback_WiFiEvents, NULL) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to add AP handlers. "); return !ESP_OK; }
    if (esp_event_handler_register(IP_EVENT, IP_EVENT_STA_LOST_IP, &_Callback_WiFiEvents, NULL) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to add AP handlers. "); return !ESP_OK; }

    // Attach to lwip stack.
    if (esp_netif_attach_wifi_station(stNetifHandle) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "WifiStation failed to attach to netif."); return !ESP_OK; }

    // Set Response.
    ESP_LOGI(DEBUG_TAG, "Wifi initalized.");
    return ESP_OK;
}
esp_err_t Wifi_Start(void) {
    // Init vars.
    esp_wifi_set_mode(WIFI_MODE_STA);
    if (esp_wifi_start() != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Wifi failed to start. "); return !ESP_OK; }

    // Set Response.
    return ESP_OK;
}
esp_err_t Wifi_Stop(void) {
    // Init vars.

    // Set Response.
    return ESP_OK;
}


/** LOCAL (PRIVATE) FUNCTION IMPLEMENTATIONS **********************************/
void _Callback_WiFiEvents(void *arg, esp_event_base_t event_base, int32_t eventId, void *event_data) {
    if (strcmp(event_base, WIFI_EVENT) == 0) {
        switch (eventId) {
            case WIFI_EVENT_STA_START:
                ESP_LOGI(DEBUG_TAG, "WIFI_EVENT_STA_START");

                // Update the connection authmode.
                stWifiConfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

                // Add SSID and Password to attempt. Password can be empty if Auth type is open.
                strcpy((char *)stWifiConfig.sta.ssid, WIFI_SSID);
                strcpy((char *)stWifiConfig.sta.password, WIFI_PASSWORD);

                // Set device to STA mode.
                if (esp_wifi_set_config(WIFI_IF_STA, &stWifiConfig) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Wifi failed to set config. "); }

                // Attempt Connect.
                esp_wifi_connect();

                break;

            case WIFI_EVENT_STA_STOP:
                ESP_LOGI(DEBUG_TAG, "WIFI_EVENT_STA_STOP");
                break;

            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(DEBUG_TAG, "WIFI_EVENT_STA_CONNECTED");
                break;

            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGI(DEBUG_TAG, "WIFI_EVENT_STA_DISCONNECTED");
                break;

            default:
                ESP_LOGI(DEBUG_TAG, "Unhandled Wifi Event");
                break;
        }
    } else if (strcmp(event_base, IP_EVENT) == 0) {
        switch (eventId) {
            case IP_EVENT_STA_GOT_IP:
                ESP_LOGI(DEBUG_TAG, "IP_EVENT_STA_GOT_IP");
                break;

            case IP_EVENT_STA_LOST_IP:
                ESP_LOGI(DEBUG_TAG, "IP_EVENT_STA_LOST_IP");
                break;

            default:
                ESP_LOGI(DEBUG_TAG, "Unhandled IP Event");
                break;
        }
    } else {
        ESP_LOGI(DEBUG_TAG, "Unhandled Event");
	}
}