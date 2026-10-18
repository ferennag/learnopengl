cmake_minimum_required(VERSION 4.0)
project(gl LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.93.0-docking
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.h src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3 SDL3_image OpenGL GLEW::GLEW glm::glm assimp)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
target_include_directories(${PROJECT_NAME} PRIVATE src ${imgui_SOURCE_DIR})

file(
    CREATE_LINK
    "${CMAKE_SOURCE_DIR}/resources"
    "${CMAKE_BINARY_DIR}/resources"
    SYMBOLIC
)
