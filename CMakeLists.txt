cmake_minimum_required(VERSION 2.8)

project(AAIRace)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_LIBRARY_NAME source_library)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(AI_LIBRARY_NAME ai_library)
set(GAME_LIBRARY_NAME game_library)
set(COMMON_LIBRARY_NAME common_library)
set(UI_LIBRARY_NAME ui_library)

find_package(SFML 2 REQUIRED system window graphics network audio)
find_package (Eigen REQUIRED)
find_package(OpenMP)
if (OPENMP_FOUND)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(./source/
                    ${PROJECT_SOURCE_DIR}/external/MiniDNN/include
                    ${SFML_INCLUDE_DIR}
                    ${EIGEN3_INCLUDE_DIR})

add_subdirectory(source)


add_executable(${PROJECT_NAME} source/main.cc)
target_link_libraries(${PROJECT_NAME} 
                      ${UI_LIBRARY_NAME}
                      ${GAME_LIBRARY_NAME}
                      ${COMMON_LIBRARY_NAME}
                      ${AI_LIBRARY_NAME}
                      ${SFML_LIBRARIES})
