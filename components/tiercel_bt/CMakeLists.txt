idf_component_register(
    SRCS "src/tiercel_bt.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES
        esp_rom
        esp_timer
        mbedtls
        tiercel_audio
        tiercel_board
)
