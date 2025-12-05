cmake_minimum_required(VERSION 3.28)
project(learnopengl LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

file(GLOB_RECURSE SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
  ${PROJECT_SOURCE_DIR}/src/*.h
)

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

add_subdirectory(lib)
add_executable(${PROJECT_NAME}
  ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic -g)
target_link_libraries(${PROJECT_NAME} engine)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  -E create_symlink
  ${CMAKE_SOURCE_DIR}/assets
  ${CMAKE_BINARY_DIR}/assets)
