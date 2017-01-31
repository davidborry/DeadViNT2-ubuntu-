cmake_minimum_required(VERSION 3.6)
project(DeadViNT_2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCES main.cpp)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HPP_FILES ${PROJECT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE H_FILES ${PROJECT_SOURCE_DIR}/*.h)


add_executable(DeadViNT_2 ${SRC_FILES} ${HPP_FILES} ${H_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML 2 COMPONENTS system window graphics audio REQUIRED)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(DeadViNT_2 ${SFML_LIBRARIES})
endif()