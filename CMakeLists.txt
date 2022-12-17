cmake_minimum_required(VERSION 3.25.1)

project(gps_playground)

add_definitions("-std=c++2b")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

#3 This is the Conan configuartion.
conan_basic_setup(NO_OUTPUT_DIRS)

add_executable(gps_playground src/app/main.cpp)

target_include_directories(gps_playground PRIVATE src/include)

target_link_libraries(gps_playground ${CONAN_LIBS})
