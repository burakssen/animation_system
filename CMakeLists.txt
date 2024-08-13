cmake_minimum_required(VERSION 3.30)
project(animation_system)
set(CMAKE_CXX_STANDARD 23)

# Build src files as a library
file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_subdirectory(vendor/raylib)
include_directories(vendor/raylib/src)

add_subdirectory(vendor/entt)
include_directories(vendor/entt/single_include)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# Set include directories as PUBLIC to propagate to dependent targets
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/raylib/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/entt/single_include
)

target_link_libraries(${PROJECT_NAME} raylib)

if (BUILD_EXAMPLE)
    # Build example as executable
    add_executable(example example/main.cpp)
    target_link_libraries(example ${PROJECT_NAME})
endif()

# Copy resources to build directory
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})
