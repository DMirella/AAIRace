cmake_minimum_required(VERSION 2.8)

add_library(${CAR_LIBRARY_NAME}
            tools.cc
            car.cc
            city_car.cc
            racing_car.cc
            road.cc
            game_session.cc)