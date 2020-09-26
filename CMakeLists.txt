cmake_minimum_required(VERSION 3.0)

project(keylayers)

add_executable(keylayers keyLayers.c)
target_compile_options(keylayers PRIVATE -Wall -Wextra)

install(TARGETS keylayers RUNTIME DESTINATION bin)
