#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glr_core.h"

static const uint32_t  sWindowHeight = 500;
static const uint32_t  sWindowWidth = 500;

glrPos3Clr4Type triangleData[] = {
    { -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
    {  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {  0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
    {  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }
    };

uint32_t indexData[] = { 0, 1, 2, 1, 2, 3 };

/*
 * Window creation function
 */
GLFWwindow* createWindow
    (
    uint32_t    width,
    uint32_t    height
    );

int main(void)
{
GLFWwindow* wnd = createWindow(sWindowHeight, sWindowWidth);
glrMeshType triangle = { 0 };

/* Setup */
glrInit(sWindowHeight, sWindowWidth);
glrInitTriangle(&triangle, triangleData, 4, indexData, 6);

while(!glfwWindowShouldClose(wnd))
    {
    glrInitScene(0x00000000);
    glrRenderTriangle(&triangle);

    glfwSwapBuffers(wnd);
    glfwPollEvents();
    }

glfwTerminate();

return 0;
}


/*
 * Window creation function
 */
GLFWwindow* createWindow
    (
    uint32_t    width,
    uint32_t    height
    )
{
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

GLFWwindow* wnd = glfwCreateWindow(width, height, "GLR", NULL, NULL);
assert(wnd != NULL);

glfwMakeContextCurrent(wnd);

if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
    printf( "Failed to initialize GLAD" );
    return NULL;
	} 

return wnd;
}