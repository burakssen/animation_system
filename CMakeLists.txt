cmake_minimum_required(VERSION 3.30)
project(animation_system)
set(CMAKE_CXX_STANDARD 23)

# Build src files as a library

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_subdirectory(vendor/raylib)
include_directories(vendor/raylib/src)

add_subdirectory(vendor/entt)
include_directories(vendor/entt/src)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} raylib)

# Build example as executable
add_executable(example example/main.cpp)
target_link_libraries(example ${PROJECT_NAME})

# Copy resources to build directory
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})