cmake_minimum_required(VERSION 3.23.0)

project(glr)

add_executable(glr
    src/main.c
    src/glad.c
    src/glr_core.c
    src/glr_shdr.c
    )

add_subdirectory(glfw)
add_subdirectory(cglm EXCLUDE_FROM_ALL)

find_package(OpenGL REQUIRED)

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    include/
)

target_link_libraries(glr
    glfw
    cglm_headers
)