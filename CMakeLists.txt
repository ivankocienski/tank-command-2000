cmake_minimum_required (VERSION 2.6)

# this gives us warnings about fishy code (-Wall)
#  and symbol tables (-dynamic) and gdb support (-g)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

# makefile will print out what it is doing
#set(CMAKE_VERBOSE_MAKEFILE ON)

# a hack for SDL
ENABLE_LANGUAGE(C)

# SDL. only this for now
find_package (SDL REQUIRED)
find_package (SDL_image REQUIRED)

# our exe name
project (wireframe)

# source files. sure, there's a better way of doing this
set (SRC
  src/application.cc
  src/main.cc
  src/window.cc
  src/line.cc
  src/player.cc
  src/mesh.cc
  src/camera.cc
  src/plane.cc
  src/draw-mesh.cc

  src/math/matrix4.cc
  src/math/matrix3.cc
  src/math/operations.cc
  src/math/vector3.cc
)

link_libraries(
    ${SDL_LIBRARY}
    ${SDL_IMAGE_LIBRARY}
)

add_executable(wireframe ${SRC} )

add_custom_command(TARGET wireframe POST_BUILD 
  COMMAND rm ${CMAKE_CURRENT_BINARY_DIR}/data
  COMMAND ln -sf ${CMAKE_SOURCE_DIR}/data
  ${CMAKE_CURRENT_BINARY_DIR}/data COMMENT "linking data directory"
)
