cmake_minimum_required(VERSION 3.25)
project(Platformer2 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    )
FetchContent_MakeAvailable(SFML)


add_executable(main
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Map.cpp
)

target_include_directories(main PRIVATE
    external/tmxlite/include
)

file(GLOB TMXLITE_SOURCES
  external/tmxlite/src/*.cpp
  external/tmxlite/src/*.c
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})


target_sources(main PRIVATE ${TMXLITE_SOURCES})
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)
