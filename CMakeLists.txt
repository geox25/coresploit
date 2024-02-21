# CS CMakeLists.txt
# Modified 2/17/2024



# =============================================================================
# Setup all project configuration
cmake_minimum_required(VERSION 3.27.8)
project(CS)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -static")
# =============================================================================



# =============================================================================
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

# Add the imgui library
add_library(imgui ${IMGUI_SOURCES})
# =============================================================================



# =============================================================================
# Specify the path to the toml11 directory
set(TOML11_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/toml11")
set(TOML11_TOML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/toml11/toml")

# Add all the toml11 source files
file(GLOB TOML11_SOURCES
        ${TOML11_DIR}/toml.hpp
        ${TOML11_TOML_DIR}/*.hpp
)

# Add the toml11 library
add_library(toml11 ${TOML11_SOURCES})

set_target_properties(toml11 PROPERTIES LINKER_LANGUAGE CXX)
# =============================================================================



# =============================================================================
# Specify the path to the glfw directory (via adding subdir)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/glfw)
# =============================================================================



# =============================================================================
# Setup CS cpp files
add_executable(CS src/main.cpp
        src/boot/trivial-boot.cpp
        src/boot/boot.cpp
        src/boot/config.cpp
        src/svc/svc-example.cpp
        src/svc/svc.cpp
        src/svc/prg-ustat.cpp
        src/svc/system/security/svc-security.cpp
)
# =============================================================================



# =============================================================================
# Setup final project settings

# Make sure opengl is found
# find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# Include the imgui and glfw library directories
# (allows imgui backends to find glfw)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} PUBLIC ${IMGUI_BACKENDS_DIR} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include")

# Include the GLFW directory
target_include_directories(CS PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include")

# Include the toml11 directory
target_include_directories(CS PUBLIC ${TOML11_DIR} PUBLIC ${TOML11_TOML_DIR})

# Link needed libraries to executable
target_link_libraries(CS imgui)
target_link_libraries(CS glfw)
target_link_libraries(CS OpenGL::GL)
target_link_libraries(CS toml11)
# =============================================================================