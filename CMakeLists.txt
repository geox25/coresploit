# PXL CMakeLists.txt
# Modified 1/25/2024

cmake_minimum_required(VERSION 3.28)
project(PXL)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

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

# Include the imgui and glfw directories
# (allows imgui backends to find glfw)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} PUBLIC ${IMGUI_BACKENDS_DIR} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/glfw)

add_executable(PXL src/main.cpp
        src/boot/trivial-boot.cpp
        src/boot/boot.cpp
        src/boot/config.cpp
        src/svc/svc-example.cpp
        src/svc/svc.cpp
        src/svc/prg-ustat.cpp
        src/svc/system/security/svc-security.cpp
)

# Make sure glfw3, opengl are found
# find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Include the GLFW directory
target_include_directories(PXL PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include")

# Link needed libraries to executable
target_link_libraries(PXL imgui)
target_link_libraries(PXL glfw)
target_link_libraries(PXL OpenGL::GL)