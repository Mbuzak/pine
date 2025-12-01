project(lab5_ex1)

add_executable(${PROJECT_NAME} main.cpp)

#add_custom_command(
#    copy_shader_files
#    ${CMAKE_COMMAND}
#    -D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/shanders
#    -D DESTINATION_DIR=${CMAKE_CURRENT_BINARY_DIR}/shaders
#    -P CopyFile.cmake
#    COMMENT "Copying Files for target: test_SphereShader" VERBATIM
#)
#
#add_dependencies(test_SphereShader copy_shader_files)

#add_custom_command(
#    TARGET lab5_ex1 POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory              
#        ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:lab5_ex1>/shaders
#    COMMENT "Copying shaders" VERBATIM
#)

#add_custom_command(
#    TARGET lab5_ex1 POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory              
#        ${CMAKE_CURRENT_SOURCE_DIR}/textures $<TARGET_FILE_DIR:lab5_ex1>/textures
#    COMMENT "Copying textures" VERBATIM
#)

file(COPY shaders/ DESTINATION shaders/)
file(COPY textures/ DESTINATION textures/)

target_link_libraries(${PROJECT_NAME} PUBLIC glut GL GLU GLEW)
