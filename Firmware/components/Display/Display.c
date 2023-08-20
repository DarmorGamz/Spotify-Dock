 /*****************************************************************************
 * @file        Display.c
 * @copyright   COPYRIGHT (c) 2023 Darmor Inc. All rights reserved.
 * @author      Darren Morrison
 * @brief       Source file for Display related functionality.
 ******************************************************************************/
/** INCLUDES ******************************************************************/
#include "Display.h"
#include "Lvgl.h"

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_lcd_ili9488.h>
#include <esp_lcd_panel_vendor.h>


/** LOCAL (PRIVATE) CONSTANT AND MACRO DEFINITIONS ****************************/
__attribute__((unused)) static const char *DEBUG_TAG = "DISPLAY";


/** LOCAL (PRIVATE) TYPEDEFS, STRUCTURES AND ENUMERATIONS *********************/


/** VARIABLES *****************************************************************/
__attribute__((unused)) static const int DISPLAY_COMMAND_BITS = 8;
__attribute__((unused)) static const int DISPLAY_PARAMETER_BITS = 8;

__attribute__((unused)) static const unsigned int DISPLAY_REFRESH_HZ = 60000000;
__attribute__((unused)) static const int DISPLAY_SPI_QUEUE_LEN = 10;
__attribute__((unused)) static const int SPI_MAX_TRANSFER_SIZE = DISPLAY_HORIZONTAL_PIXELS * 80 * sizeof(uint16_t); // transfer 80 lines of pixels (assume pixel is RGB565) at most in one SPI transaction

__attribute__((unused)) static const ledc_mode_t BACKLIGHT_LEDC_MODE = LEDC_LOW_SPEED_MODE;
__attribute__((unused)) static const ledc_channel_t BACKLIGHT_LEDC_CHANNEL = LEDC_CHANNEL_0;
__attribute__((unused)) static const ledc_timer_t BACKLIGHT_LEDC_TIMER = LEDC_TIMER_1;
__attribute__((unused)) static const ledc_timer_bit_t BACKLIGHT_LEDC_TIMER_RESOLUTION = LEDC_TIMER_10_BIT;
__attribute__((unused)) static const uint32_t BACKLIGHT_LEDC_FRQUENCY = 5000;

__attribute__((unused)) static esp_lcd_panel_io_handle_t lcd_io_handle = NULL;
esp_lcd_panel_handle_t lcd_handle = NULL;


/** LOCAL (PRIVATE) FUNCTION PROTOTYPES ***************************************/
static esp_err_t _Display_Brightness_Init(void);
static esp_err_t _Display_Brightness_Set(uint16_t);


/** PUBLIC FUNCTION IMPLEMENTATIONS *******************************************/
esp_err_t Display_Init(void) {
    // Initialize Display Brightness.
    if (_Display_Brightness_Init() != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to initalize brightness."); return !ESP_OK; }

    // Set brightness to 0.
    if (_Display_Brightness_Set(0) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to set brightness."); return !ESP_OK; }

    // Initalize Spi bus
    spi_bus_config_t stLcdSpiConfig = {
        .mosi_io_num = LCD_PIN_MOSI,
        .miso_io_num = LCD_PIN_MISO,
        .sclk_io_num = LCD_PIN_SCK,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .data4_io_num = GPIO_NUM_NC,
        .data5_io_num = GPIO_NUM_NC,
        .data6_io_num = GPIO_NUM_NC,
        .data7_io_num = GPIO_NUM_NC,
        .max_transfer_sz = SPI_MAX_TRANSFER_SIZE,
        .flags = SPICOMMON_BUSFLAG_SCLK | SPICOMMON_BUSFLAG_MISO | SPICOMMON_BUSFLAG_MOSI | SPICOMMON_BUSFLAG_MASTER,
        .intr_flags = ESP_INTR_FLAG_LOWMED | ESP_INTR_FLAG_IRAM
    };
    if (spi_bus_initialize(SPI2_HOST, &stLcdSpiConfig, SPI_DMA_CH_AUTO) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to initalize lcd spi."); return !ESP_OK; }

    const esp_lcd_panel_io_spi_config_t stLcdIOConfig = {
        .cs_gpio_num = LCD_PIN_CS,
        .dc_gpio_num = LCD_PIN_DC,
        .spi_mode = 0,
        .pclk_hz = DISPLAY_REFRESH_HZ,
        .trans_queue_depth = DISPLAY_SPI_QUEUE_LEN,
        .on_color_trans_done = Lvgl_Notify_Flush_Ready,
        .user_ctx = &lv_disp_drv,
        .lcd_cmd_bits = DISPLAY_COMMAND_BITS,
        .lcd_param_bits = DISPLAY_PARAMETER_BITS,
        .flags = {
            .dc_low_on_data = 0,
            .octal_mode = 0,
            .sio_mode = 0,
            .lsb_first = 0,
            .cs_high_active = 0
        }
    };

    const esp_lcd_panel_dev_config_t lcd_config =  {
        .reset_gpio_num = LCD_PIN_RST,
        .color_space = CONFIG_DISPLAY_COLOR_MODE,
        .bits_per_pixel = 18,
        .flags = {
            .reset_active_high = 0
        },
        .vendor_config = NULL
    };

    // Create ned LCD panel IO.
    if (esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &stLcdIOConfig, &lcd_io_handle) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to initalize lcd io spi."); return !ESP_OK; }

    // Create new ILI9488 driver.
    if (esp_lcd_new_panel_ili9488(lcd_io_handle, &lcd_config, LV_BUFFER_SIZE, &lcd_handle) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to initalize ILI9488 layer."); return !ESP_OK; }

    if (esp_lcd_panel_reset(lcd_handle) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to reset panel."); return !ESP_OK; }
    if (esp_lcd_panel_init(lcd_handle) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to initialize panel."); return !ESP_OK; }
    if (esp_lcd_panel_invert_color(lcd_handle, false) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to invert color."); return !ESP_OK; }
    if (esp_lcd_panel_swap_xy(lcd_handle, true) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to swap xy."); return !ESP_OK; }
    if (esp_lcd_panel_mirror(lcd_handle, false, true) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to mirror."); return !ESP_OK; }
    if (esp_lcd_panel_set_gap(lcd_handle, 0, 0) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to set gap."); return !ESP_OK; }
    if (esp_lcd_panel_disp_on_off(lcd_handle, true) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to turn display on."); return !ESP_OK; }

    // Set brightness to 100.
    if (_Display_Brightness_Set(100) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to set brightness."); return !ESP_OK; }

    // Set Response.
    return ESP_OK;
}


/** LOCAL (PRIVATE) FUNCTION IMPLEMENTATIONS **********************************/
static esp_err_t _Display_Brightness_Init(void) {
    const ledc_channel_config_t LCD_backlight_channel = {
        .gpio_num = (gpio_num_t)LCD_PIN_BKL,
        .speed_mode = BACKLIGHT_LEDC_MODE,
        .channel = BACKLIGHT_LEDC_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = BACKLIGHT_LEDC_TIMER,
        .duty = 0,
        .hpoint = 0,
        .flags = {
            .output_invert = 0
        }
    };
    const ledc_timer_config_t LCD_backlight_timer = {
        .speed_mode = BACKLIGHT_LEDC_MODE,
        .duty_resolution = BACKLIGHT_LEDC_TIMER_RESOLUTION,
        .timer_num = BACKLIGHT_LEDC_TIMER,
        .freq_hz = BACKLIGHT_LEDC_FRQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };

    if (ledc_timer_config(&LCD_backlight_timer) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to set led timer config."); return !ESP_OK; }
    if (ledc_channel_config(&LCD_backlight_channel) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to set led channel config."); return !ESP_OK; }

    // Set Response.
    ESP_LOGI(DEBUG_TAG, "Display brightness initalized.");
    return ESP_OK;
}
static esp_err_t _Display_Brightness_Set(uint16_t u16BrightnessPercent) {
    // Ensure brightness is within 0->100
    if (u16BrightnessPercent > 100) u16BrightnessPercent = 100;

    // LEDC resolution set to 10bits, thus: 100% = 1023
    uint32_t u32DutyCycle = (1023 * u16BrightnessPercent) / 100;

    if (ledc_set_duty(BACKLIGHT_LEDC_MODE, BACKLIGHT_LEDC_CHANNEL, u32DutyCycle) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to set backlight dutycycle."); return !ESP_OK; }
    if (ledc_update_duty(BACKLIGHT_LEDC_MODE, BACKLIGHT_LEDC_CHANNEL) != ESP_OK) { ESP_LOGI(DEBUG_TAG, "Failed to update backlight dutycycle."); return !ESP_OK; }

    // Set Response.
    ESP_LOGI(DEBUG_TAG, "Setting backlight to %d%%", u16BrightnessPercent);
    return ESP_OK;
}
