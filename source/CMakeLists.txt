cmake_minimum_required(VERSION 2.8)

set(AI_LIBRARY_NAME ai_library)
set(GAME_LIBRARY_NAME game_library)
set(COMMON_LIBRARY_NAME common_library)
set(UI_LIBRARY_NAME ui_library)

include_directories(${PROJECT_SOURCE_DIR}/external/MiniDNN/include)

add_subdirectory(ai)
add_subdirectory(common)
add_subdirectory(game)
add_subdirectory(ui)

add_library(${SOURCE_LIBRARY_NAME} main.cc)

target_link_libraries(${SOURCE_LIBRARY_NAME}
                      ${AI_LIBRARY_NAME}
                      ${GAME_LIBRARY_NAME}
                      ${COMMON_LIBRARY_NAME}
                      ${UI_LIBRARY_NAME})
