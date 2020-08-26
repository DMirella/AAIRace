cmake_minimum_required(VERSION 2.8)

add_library(${UI_LIBRARY_NAME}
            game_window.cc
            screens.cc
            screen_state_machine.cc)