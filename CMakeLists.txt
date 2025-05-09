cmake_minimum_required(VERSION 3.23.0)

project(glr)

add_executable(glr
    src/main.c
    src/glad.c
    src/glr_core.c
    src/glr_shdr.c
    src/glr_texture.c
    src/glr_camera.c
    src/glr_mesh.c
    )

add_subdirectory(glfw)
add_subdirectory(cglm EXCLUDE_FROM_ALL)
add_subdirectory(assimp)

find_package(OpenGL REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    include/
    assimp/incldue
)

target_link_libraries(glr
    glfw
    cglm_headers
    assimp
)