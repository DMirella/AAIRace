cmake_minimum_required(VERSION 2.8)

add_library(${CAR_LIBRARY_NAME}
            tools.cc
            car.cc
            city_car.cc
            racing_car.cc
            road.cc
            game_business_logic.cc
            game_session.cc
            game_window.cc
            level_manager.cc
            screens.cc
            screen_state_machine.cc
            enemy_ai.cc)