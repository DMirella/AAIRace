cmake_minimum_required(VERSION 2.8)

set(GAME_UNITS_LIBRARY_NAME game_units_library)
add_subdirectory(units)

add_library(${GAME_LIBRARY_NAME}
            game_business_logic.cc
            game_session.cc
            level_manager.cc
            user_profile.cc)
target_link_libraries(${GAME_LIBRARY_NAME} ${GAME_UNITS_LIBRARY_NAME})