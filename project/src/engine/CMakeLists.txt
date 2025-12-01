set(
  ENGINE_SOURCES
  camera.cpp
  framebuffer.cpp
  light.cpp
  mesh.cpp
  model.cpp
  pipeline.cpp
  shadow.cpp
  texture.cpp
)

add_library(Engine STATIC ${ENGINE_SOURCES})

target_include_directories(Engine PUBLIC ${LIBS_DIR})
