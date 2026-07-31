add_library(Engine STATIC camera.c frame.c light.cpp mesh.cpp
	entity.cpp shader.c shadow.cpp texture.c display.c terrain.cpp
	uniform.cpp material.c skybox.cpp controller.c rendered.cpp outline.cpp
)

target_link_libraries(Engine PUBLIC SDL2::SDL2 PRIVATE cglm)
target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/libs)
