cmake_minimum_required(VERSION 3.15)
project(OpenGlGeoEx VERSION 0.1.0)

set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )

add_definitions(-DGLEW_STATIC)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glew-cmake EXCLUDE_FROM_ALL)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glfw3.3)

set(GLM_TEST_ENABLE OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glm)

# set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
# set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
# set(TINYGLTF_USE_CPP14 ON CACHE INTERNAL "" FORCE)
# set(TINYGLTF_NO_INCLUDE_STB_IMAGE ON CACHE INTERNAL "" FORCE) 
# add_subdirectory(${PROJECT_SOURCE_DIR}/external/tinygltf)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/imgui-cmake)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/stb-cmake)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/yaml-cpp)

add_subdirectory(src)

# file( GLOB_RECURSE SOURCEFILES src/*.cpp )
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)