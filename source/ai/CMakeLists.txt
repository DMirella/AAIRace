cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/external/MiniDNN/include)

add_library(${AI_LIBRARY_NAME}
            enemy_ai.cc
            ai.cc
            ai_types.cc)