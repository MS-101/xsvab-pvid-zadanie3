cmake_minimum_required(VERSION 3.0)
project(pvid_zadanie3)

find_package(OpenCV REQUIRED)

file(GLOB SRC_FILES src/*.cpp)
add_executable(main.exe ${SRC_FILES})

target_link_libraries(main.exe ${OpenCV_LIBS})