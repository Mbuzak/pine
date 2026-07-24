add_library(Engine STATIC camera.cpp frame.c light.cpp mesh.cpp
	entity.cpp shader.c shadow.cpp texture.c display.c terrain.cpp
	uniform.cpp material.cpp skybox.cpp controller.c rendered.cpp outline.cpp
)

target_link_libraries(Engine PUBLIC SDL2::SDL2)
target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/libs)
