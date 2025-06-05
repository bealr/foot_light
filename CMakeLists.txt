cmake_minimum_required(VERSION 3.13)
project(foot_light)

set(SOURCE_FILES main.c dmx.c starville_4_bundle.c)

add_executable(foot_light ${SOURCE_FILES})