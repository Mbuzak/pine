add_library(Engine STATIC camera.cpp framebuffer.cpp light.cpp mesh.cpp
	model.cpp shader.cpp shadow.cpp texture.cpp renderer.cpp
)

target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/libs)
