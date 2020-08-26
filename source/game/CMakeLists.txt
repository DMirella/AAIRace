cmake_minimum_required(VERSION 2.8)

set(DRAWABLE_UNITS_LIBRARY_NAME drawable_units_library)
add_subdirectory(drawable_units)

add_library(${GAME_LIBRARY_NAME}
            game_business_logic.cc
            game_session.cc
            level_manager.cc)
target_link_libraries(${GAME_LIBRARY_NAME} ${DRAWABLE_UNITS_LIBRARY_NAME})