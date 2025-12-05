include(FetchContent)
FetchContent_Declare(SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG release-3.2.26
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(SDL3_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
  GIT_TAG release-3.2.4
)
FetchContent_MakeAvailable(SDL3_image)

FetchContent_Declare(glew
  GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
  GIT_TAG glew-cmake-2.2.0
)
FetchContent_MakeAvailable(glew)

FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG v1.92.5
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 1.0.2
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(FastNoiseLite
  GIT_REPOSITORY https://github.com/Auburn/FastNoiseLite
  GIT_TAG v1.1.1
)
FetchContent_MakeAvailable(FastNoiseLite)

set(IMGUI_SOURCES
  ${imgui_SOURCE_DIR}/imgui.h
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

file(GLOB_RECURSE SOURCES
  ./*.cpp
  ./*.h
)
add_library(engine SHARED ${SOURCES} ${IMGUI_SOURCES}
  ${fastnoiselite_SOURCE_DIR}/Cpp/FastNoiseLite.h
)
set_property(TARGET engine PROPERTY CXX_STANDARD 23)
target_include_directories(engine PUBLIC
  .
  ${imgui_SOURCE_DIR}
  ${fastnoiselite_SOURCE_DIR}/Cpp
)
target_link_libraries(engine PUBLIC SDL3::SDL3 SDL3_image::SDL3_image glm libglew_static)
