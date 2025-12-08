add_library(Engine STATIC camera.cpp framebuffer.cpp light.cpp mesh.cpp
	entity.cpp shader.c shadow.cpp texture.cpp renderer.cpp display.c
	uniform.cpp material.cpp
)

target_link_libraries(Engine PUBLIC SDL2::SDL2)
target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/libs)
