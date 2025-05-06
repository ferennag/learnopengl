cmake_minimum_required(VERSION 3.31)
project(learnopengl)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp src/Application.cpp src/Game.cpp src/Mesh.cpp src/Shader.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 GLEW::GLEW OpenGL)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -g)

add_custom_target(resources)
add_custom_command(TARGET resources POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources)

add_dependencies(${PROJECT_NAME} resources)
