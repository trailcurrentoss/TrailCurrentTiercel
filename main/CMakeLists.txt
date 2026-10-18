# Tiercel main component.
#
# GENERATED UI: main/ui/ is written by EEZ Studio's Build (Ctrl+B) from
# GUI/Tiercel.eez-project and is entirely disposable -- delete everything in
# it and re-export at any time. Nothing hand-written lives there, which is why
# actions.c and vars.c (the functions the export declares) sit here in main/.

set(UI_DIR "ui")

# CONFIGURE_DEPENDS re-checks the glob when the file set changes, so a fresh
# export is picked up without a manual reconfigure. It is illegal in the CMake
# script mode ESP-IDF uses for its first pass over every component.
if(CMAKE_SCRIPT_MODE_FILE)
    set(UI_GLOB_MODE "")
else()
    set(UI_GLOB_MODE CONFIGURE_DEPENDS)
endif()

file(GLOB UI_CORE   ${UI_GLOB_MODE} "${UI_DIR}/screens.c" "${UI_DIR}/styles.c"
                                    "${UI_DIR}/images.c"  "${UI_DIR}/ui.c")
file(GLOB UI_FONTS  ${UI_GLOB_MODE} "${UI_DIR}/ui_font_*.c")
file(GLOB UI_IMAGES ${UI_GLOB_MODE} "${UI_DIR}/ui_image_*.c")

# A stale glob links a firmware with NO UI and still reports success. Fail
# loudly instead.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${UI_DIR}/ui.h" AND NOT UI_CORE)
    message(FATAL_ERROR
        "main/ui/ui.h exists but no generated sources were found -- the glob "
        "is stale. Run `idf.py reconfigure` after an EEZ Studio export.")
endif()

# The include path only when the folder exists: main/ui may be emptied or
# removed outright before a re-export, and that must not break the build.
set(UI_INCLUDE "")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${UI_DIR}")
    set(UI_INCLUDE "${UI_DIR}")
endif()

# Whether the generated UI exists is decided HERE and passed as a define.
# Not `__has_include` in C: that records no dependency on an absent file, so
# a build made before the first export never notices the export appear and
# keeps shipping the bring-up screen. Capstan lost a debugging round to it.
if(UI_CORE)
    set(TIERCEL_HAVE_UI 1)
    set(APP_SRCS
        "ui_nav.c" "ui_settings.c" "ui_data.c" "ui_setup.c" "ui_clock.c"
        "ui_devices.c" "ui_alerts.c" "ui_climate.c" "ui_peregrine.c"
        "ui_playbill.c" "ui_bluetooth.c" "actions.c" "vars.c")
else()
    set(TIERCEL_HAVE_UI 0)
    set(APP_SRCS "bringup.c")
    message(WARNING
        "No generated UI in main/ui -- building the bring-up screen. Open "
        "GUI/Tiercel.eez-project in EEZ Studio and press Ctrl+B to export.")
endif()

idf_component_register(
    SRCS
        "main.c"
        "discovery.c"
        ${APP_SRCS}
        ${UI_CORE}
        ${UI_FONTS}
        ${UI_IMAGES}
    INCLUDE_DIRS
        "."
        ${UI_INCLUDE}
    REQUIRES
        tiercel_board
        tiercel_config
        tiercel_model
        tiercel_wifi
        tiercel_mqtt
        tiercel_portal
        tiercel_audio
        tiercel_peregrine
        tiercel_bt
        nvs_flash
        esp_timer
        espressif__mdns
        esp_http_server
        esp_app_format
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE
    TIERCEL_HAVE_UI=${TIERCEL_HAVE_UI})
