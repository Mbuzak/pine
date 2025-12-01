add_library(Engine STATIC camera.cpp framebuffer.cpp light.cpp mesh.cpp
	model.cpp pipeline.cpp shadow.cpp texture.cpp
)

target_include_directories(Engine PUBLIC ${LIBS_DIR})
