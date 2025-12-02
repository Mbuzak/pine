add_executable(${PROJECT_NAME} main.cpp scene.cpp)

add_subdirectory(chess)
add_subdirectory(engine)

target_include_directories(${PROJECT_NAME} PUBLIC ${LIBS_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC glut GL GLU GLEW PRIVATE Engine chess)
