# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.5)

# Set the component's name
set(COMPONENT_NAME "Display")

# Set the list of source files for the component
set(SRCS "Lvgl.c" "Display.c" "Lvgl.c")

# Set the list of header files for the component (optional)
set(HDRS "Display.h" "Lvgl.h")

# Register the component with the build system
idf_component_register(SRCS "${SRCS}" INCLUDE_DIRS "." REQUIRES Common driver freertos esp_lcd esp_timer lvgl PRIV_REQUIRES esp_lcd_ili9488 heap)
