cmake_minimum_required(VERSION 3.16)
project(AsteroidsSDL3)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SOURCE_FILES
    src/main.cpp
    src/GameEngine.cpp
    src/Object.cpp
)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
set(SDLTTF_VENDORED ON) # tell SDL_ttf to build its own dependencies
set(SDLTTF_HARFBUZZ=OFF)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

# on Web targets, we need CMake to generate a HTML webpage.
if(EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html" CACHE INTERNAL "")
endif()

# Create your game executable target as usual
add_executable(AsteroidsSDL3 WIN32 ${SOURCE_FILES})

# Link to the actual SDL3 library and SDL3_ttf font library.
target_link_libraries(AsteroidsSDL3 PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

if(EMSCRIPTEN)
  target_link_options(AsteroidsSDL3 PRIVATE "--preload-file ${CMAKE_SOURCE_DIR}/ARCADECLASSIC.TTF@/ARCADECLASSIC.ttf")
endif() 
