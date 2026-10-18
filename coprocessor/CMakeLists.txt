# Tiercel -- the ESP32-U4WDH co-processor's firmware: Bluetooth headphones.
# A separate ESP-IDF project, target esp32; Tiercel itself is the ESP32-S3
# project one folder up. See docs/bluetooth.md for building and flashing.
cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(tiercel_bt)
