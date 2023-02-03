file( GLOB_RECURSE APP_SOURCES *.cpp )

add_executable(MaxMustermann ${APP_SOURCES})

# target_link_libraries(OpenGLTraining game engine maths input ${GLEW_LIBRARIES} ${SDL2_LIBRARIES} OpenGL::GL OpenGL::GLU)
# tinygltf
target_link_libraries(MaxMustermann PUBLIC ${OPENGL_LIBRARIES} glm libglew_static glfw IMGUI STB Comdlg32 yaml-cpp ${CMAKE_DL_LIBS})

# target_include_directories(LibCore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(MaxMustermann PUBLIC ${PROJECT_SOURCE_DIR}/src)