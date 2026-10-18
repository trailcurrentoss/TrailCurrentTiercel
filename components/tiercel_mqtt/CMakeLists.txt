idf_component_register(
    SRCS "src/tiercel_mqtt.c"
    INCLUDE_DIRS "include"
    REQUIRES tiercel_config tiercel_model mqtt json esp_timer esp_hw_support
)
