cmake_minimum_required(VERSION 2.8)

project(AAIRace)

set(CMAKE_CXX_STANDARD 14)

set(CAR_LIBRARY_NAME car_library)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SFML 2 REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})

add_subdirectory(source)

add_executable(${PROJECT_NAME} source/main.cc)
target_link_libraries(${PROJECT_NAME} 
                      ${CAR_LIBRARY_NAME} 
                      ${SFML_LIBRARIES})