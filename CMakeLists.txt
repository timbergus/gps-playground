cmake_minimum_required(VERSION 3.28.3)
project(gps-playground VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror -Wpedantic)

file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/apps/*.cpp)

find_package(fmt REQUIRED)
find_package(raylib REQUIRED)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/main.cpp ${CPP_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/include)

target_link_libraries(${PROJECT_NAME} fmt::fmt)
target_link_libraries(${PROJECT_NAME} raylib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
