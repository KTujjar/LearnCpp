cmake_minimum_required(VERSION 3.16)
project(ProjectP)
set(CMAKE_CXX_STANDARD 17)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

set(SOURCES
  src/main.cpp
  src/Game.cpp
  src/Player.cpp
  src/Map.cpp
  src/Texture.cpp
  src/Viewport.cpp
)

file(GLOB TMXLITE_SOURCES
  external/tmxlite/src/*.cpp
  external/tmxlite/src/*.c
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
# Create your game executable target as usual

add_executable(ProjectP WIN32 ${SOURCES})

target_include_directories(ProjectP PRIVATE
    external/tmxlite/include
)


target_sources(ProjectP PRIVATE ${TMXLITE_SOURCES})

# Link to the actual SDL3 library.
target_link_libraries(ProjectP PRIVATE SDL3_image::SDL3_image SDL3::SDL3)