# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.5)

# Set the component's name
set(COMPONENT_NAME "Wifi")

# Set the list of source files for the component
set(SRCS "Wifi.c")

# Set the list of header files for the component (optional)
set(HDRS "Wifi.h")

# Register the component with the build system
idf_component_register(SRCS "${SRCS}" INCLUDE_DIRS "." REQUIRES Common PRIV_REQUIRES esp_wifi esp_netif nvs_flash)
