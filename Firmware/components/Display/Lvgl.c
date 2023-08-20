 /*****************************************************************************
 * @file        Lvgl.c
 * @copyright   COPYRIGHT (c) 2023 Darmor Inc. All rights reserved.
 * @author      Darren Morrison
 * @brief       Source file for Lvgl related functionality.
 ******************************************************************************/
/** INCLUDES ******************************************************************/
#include "Lvgl.h"
#include "esp_heap_caps.h"
#include <esp_timer.h>


/** LOCAL (PRIVATE) CONSTANT AND MACRO DEFINITIONS ****************************/
__attribute__((unused)) static const char *DEBUG_TAG = "LVGL";


/** LOCAL (PRIVATE) TYPEDEFS, STRUCTURES AND ENUMERATIONS *********************/


/** VARIABLES *****************************************************************/
static lv_disp_draw_buf_t lv_disp_buf;
static lv_disp_t *lv_display = NULL;
static lv_color_t *lv_buf_1 = NULL;
static lv_color_t *lv_buf_2 = NULL;
lv_disp_drv_t lv_disp_drv;


/** LOCAL (PRIVATE) FUNCTION PROTOTYPES ***************************************/
static void _Lvgl_Flush_Callback(lv_disp_drv_t *, const lv_area_t *, lv_color_t *);
static void IRAM_ATTR Lvgl_Tick_Callback(void *);


/** PUBLIC FUNCTION IMPLEMENTATIONS *******************************************/
esp_err_t Lvgl_Init(void) {
    // Init the lvgl library.
    lv_init();

    // Initalize the buffers.
    ESP_LOGI(DEBUG_TAG, "Allocating %zu bytes for LVGL buffer", LV_BUFFER_SIZE * sizeof(lv_color_t));
    lv_buf_1 = (lv_color_t *)heap_caps_malloc(LV_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    ESP_LOGI(DEBUG_TAG, "Allocating %zu bytes for second LVGL buffer", LV_BUFFER_SIZE * sizeof(lv_color_t));
    lv_buf_2 = (lv_color_t *)heap_caps_malloc(LV_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);

    lv_disp_draw_buf_init(&lv_disp_buf, lv_buf_1, lv_buf_2, LV_BUFFER_SIZE);

    lv_disp_drv_init(&lv_disp_drv);
    lv_disp_drv.hor_res = DISPLAY_HORIZONTAL_PIXELS;
    lv_disp_drv.ver_res = DISPLAY_VERTICAL_PIXELS;
    lv_disp_drv.flush_cb = _Lvgl_Flush_Callback;
    lv_disp_drv.draw_buf = &lv_disp_buf;
    lv_disp_drv.user_data = lcd_handle;
    // lv_disp_drv.antialiasing = 0;
    lv_display = lv_disp_drv_register(&lv_disp_drv);

    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &Lvgl_Tick_Callback,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    if (esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to create timer."); return !ESP_OK; }
    if (esp_timer_start_periodic(lvgl_tick_timer, LVGL_UPDATE_PERIOD_MS * 1000) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to start timer."); return !ESP_OK; }

    // Set Response.
    return ESP_OK;
}
bool Lvgl_Notify_Flush_Ready(esp_lcd_panel_io_handle_t stPanel_io, esp_lcd_panel_io_event_data_t *p_stPanelEventData, void* p_UserCtx) {
    lv_disp_drv_t *p_stDisplayDriver = (lv_disp_drv_t *)p_UserCtx;
    lv_disp_flush_ready(p_stDisplayDriver);

    // Set Response.
    return false;
}


/** LOCAL (PRIVATE) FUNCTION IMPLEMENTATIONS **********************************/
static void _Lvgl_Flush_Callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map) {
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;

    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
}
static void IRAM_ATTR Lvgl_Tick_Callback(void *param) {
	lv_tick_inc(LVGL_UPDATE_PERIOD_MS);
}

//     // lv_demo_music();
//     // while (1) {
//     //     vTaskDelay(pdMS_TO_TICKS(5));
//     //     lv_timer_handler();
//     // }
// #include <esp_err.h>
// #include <esp_freertos_hooks.h>
// #include <esp_log.h>

// #include "lv_demos.h"