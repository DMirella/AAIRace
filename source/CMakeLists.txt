cmake_minimum_required(VERSION 2.8)

set(AI_LIBRARY_NAME ai_library)
set(GAME_LIBRARY_NAME game_library)
set(TOOLS_LIBRARY_NAME tools_library)
set(UI_LIBRARY_NAME ui_library)

include_directories(./)

add_subdirectory(ai)
add_subdirectory(game)
add_subdirectory(tools)
add_subdirectory(ui)

add_library(${SOURCE_LIBRARY_NAME} main.cc)

target_link_libraries(${SOURCE_LIBRARY_NAME}
                      ${AI_LIBRARY_NAME}
                      ${GAME_LIBRARY_NAME}
                      ${TOOLS_LIBRARY_NAME}
                      ${UI_LIBRARY_NAME})