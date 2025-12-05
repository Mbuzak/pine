add_executable(${PROJECT_NAME} main.cpp scene.cpp)

add_subdirectory(chess)
add_subdirectory(engine)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs)
target_link_libraries(${PROJECT_NAME} PUBLIC GL GLU GLEW PRIVATE Engine chess)
