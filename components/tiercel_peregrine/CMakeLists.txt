idf_component_register(
    SRCS "src/tiercel_peregrine.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_http_client
    PRIV_REQUIRES tiercel_audio esp_timer
)
