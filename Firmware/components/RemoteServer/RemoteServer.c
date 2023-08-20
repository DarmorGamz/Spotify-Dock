 /*****************************************************************************
 * @file        RemoteServer.c
 * @copyright   COPYRIGHT (c) 2023 Darmor Inc. All rights reserved.
 * @author      Darren Morrison
 * @brief       Source file for RemotServer related functionality.
 ******************************************************************************/
/** INCLUDES ******************************************************************/
#include "RemoteServer.h"


/** LOCAL (PRIVATE) CONSTANT AND MACRO DEFINITIONS ****************************/
__attribute__((unused)) static const char *DEBUG_TAG = "REMOTESERVER";


/** LOCAL (PRIVATE) TYPEDEFS, STRUCTURES AND ENUMERATIONS *********************/


/** VARIABLES *****************************************************************/


/** LOCAL (PRIVATE) FUNCTION PROTOTYPES ***************************************/
int _Callback_RemoteServerEvents(esp_http_client_event_t *p_stHttpClientEvent);


/** PUBLIC FUNCTION IMPLEMENTATIONS *******************************************/


/** LOCAL (PRIVATE) FUNCTION IMPLEMENTATIONS **********************************/
int _Callback_RemoteServerEvents(esp_http_client_event_t *p_stHttpClientEvent) {
    switch(p_stHttpClientEvent->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(DEBUG_TAG, "HTTP_EVENT_ERROR");
            break;

        case HTTP_EVENT_ON_CONNECTED:
           ESP_LOGI(DEBUG_TAG, "HTTP_EVENT_ON_CONNECTED");
            break;

        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(DEBUG_TAG, "HTTP_EVENT_HEADER_SENT");
            break;

        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(DEBUG_TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", p_stHttpClientEvent->header_key, p_stHttpClientEvent->header_value);
            break;

        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(DEBUG_TAG, "HTTP_EVENT_ON_DATA: %d, %s", p_stHttpClientEvent->data_len, (char*)p_stHttpClientEvent->data);
            break;

        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(DEBUG_TAG, "HTTP_EVENT_ON_FINISH");
            break;

        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(DEBUG_TAG, "HTTP_EVENT_DISCONNECTED");
            break;

        default:
            ESP_LOGI(DEBUG_TAG, "Unhandled HTTP_EVENT");
            break;
    }
    return ESP_OK;
}