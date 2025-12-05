add_library(Engine STATIC camera.cpp framebuffer.cpp light.cpp mesh.cpp
	model.cpp shader.cpp shadow.cpp texture.cpp renderer.cpp display.c
)

target_link_libraries(Engine PUBLIC SDL2::SDL2)
target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/libs)
