idf_component_register(
    SRCS "src/tiercel_wifi.c"
    INCLUDE_DIRS "include"
    REQUIRES tiercel_config esp_wifi esp_netif esp_event nvs_flash
)
