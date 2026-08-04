add_library(Engine STATIC camera.c frame.c light.c mesh.c
	entity.c shader.c shadow.c texture.c display.c terrain.c
	material.c controller.c rendered.c outline.c
)

target_link_libraries(Engine PUBLIC SDL2::SDL2 PRIVATE cglm)
target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/libs)
