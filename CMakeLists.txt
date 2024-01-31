# PXL CMakeLists.txt
# Modified 1/25/2024

cmake_minimum_required(VERSION 3.28)
project(PXL)

set(CMAKE_CXX_STANDARD 20)

# Specify the path to the imgui directory
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui")
set(IMGUI_BACKENDS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends")

# Add all the imgui source files
file(GLOB IMGUI_SOURCES
        ${IMGUI_DIR}/*.cpp
        ${IMGUI_DIR}/*.h
        ${IMGUI_BACKENDS_DIR}/*.cpp
        ${IMGUI_BACKENDS_DIR}/*.h
)

# Add the library
add_library(imgui ${IMGUI_SOURCES})

# Include the imgui directory
target_include_directories(imgui PUBLIC ${IMGUI_DIR} PUBLIC ${IMGUI_BACKENDS_DIR})

add_executable(PXL src/main.cpp
        src/boot/trivial-boot.cpp
        src/boot/boot.cpp
        src/svc/example-svc.cpp
        src/svc/svc.cpp)

# Make sure glfw3, opengl are found
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Link needed libraries to executable
target_link_libraries(PXL imgui)
target_link_libraries(PXL glfw)
target_link_libraries(PXL OpenGL::GL)