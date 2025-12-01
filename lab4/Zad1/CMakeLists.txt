project(lab4_ex1)

add_executable(${PROJECT_NAME} main.cpp)

file(COPY shaders/ DESTINATION shaders/)
file(COPY models/ DESTINATION models/)


target_link_libraries(${PROJECT_NAME} PUBLIC glut GL GLU GLEW)
