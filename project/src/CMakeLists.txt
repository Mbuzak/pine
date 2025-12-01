set(
  SOURCES
  main.cpp
  scene.cpp
  game.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(chess)

add_subdirectory(engine)

function(copy_folder name)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_DIR}/${name} ${BIN_FOLDER}/${name}
        WORKING_DIRECTORY ${PROJECT_DIR}
    )
endfunction()

copy_folder(models)
copy_folder(textures)
copy_folder(shaders)
copy_folder(skybox)

target_include_directories(${PROJECT_NAME} PUBLIC ${LIBS_DIR})

target_link_libraries(${PROJECT_NAME}
    PUBLIC glut GL GLU GLEW
    PRIVATE Engine chess
)
