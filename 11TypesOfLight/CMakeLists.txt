cmake_minimum_required (VERSION 3.5)

project(OpenGL)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(src/CMakeLists.txt)
include(include/CMakeLists.txt)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(opengl ${primary_sources})
target_compile_definitions(opengl PRIVATE SOURCE_ROOT=${CMAKE_SOURCE_DIR})

find_package(glfw3 3.4 REQUIRED)
TARGET_LINK_LIBRARIES(opengl PRIVATE glfw)
TARGET_LINK_LIBRARIES(opengl PRIVATE ${CMAKE_DL_LIBS})

TARGET_INCLUDE_DIRECTORIES(opengl PUBLIC include)