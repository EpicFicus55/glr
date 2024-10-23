#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glr_core.h"

static const uint32_t  sWindowHeight = 500;
static const uint32_t  sWindowWidth = 500;

glrPos3Tex2Type triangleData[] = 
    {
    { -0.5f, -0.5f, -0.5f,  0.0f, 0.0f },
    {  0.5f, -0.5f, -0.5f,  1.0f, 0.0f },
    {  0.5f,  0.5f, -0.5f,  1.0f, 1.0f },
    {  0.5f,  0.5f, -0.5f,  1.0f, 1.0f },
    { -0.5f,  0.5f, -0.5f,  0.0f, 1.0f },
    { -0.5f, -0.5f, -0.5f,  0.0f, 0.0f },
    { -0.5f, -0.5f,  0.5f,  0.0f, 0.0f },
    {  0.5f, -0.5f,  0.5f,  1.0f, 0.0f },
    {  0.5f,  0.5f,  0.5f,  1.0f, 1.0f },
    {  0.5f,  0.5f,  0.5f,  1.0f, 1.0f },
    { -0.5f,  0.5f,  0.5f,  0.0f, 1.0f },
    { -0.5f, -0.5f,  0.5f,  0.0f, 0.0f },
    { -0.5f,  0.5f,  0.5f,  1.0f, 0.0f },
    { -0.5f,  0.5f, -0.5f,  1.0f, 1.0f },
    { -0.5f, -0.5f, -0.5f,  0.0f, 1.0f },
    { -0.5f, -0.5f, -0.5f,  0.0f, 1.0f },
    { -0.5f, -0.5f,  0.5f,  0.0f, 0.0f },
    { -0.5f,  0.5f,  0.5f,  1.0f, 0.0f },
    {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f },
    {  0.5f,  0.5f, -0.5f,  1.0f, 1.0f },
    {  0.5f, -0.5f, -0.5f,  0.0f, 1.0f },
    {  0.5f, -0.5f, -0.5f,  0.0f, 1.0f },
    {  0.5f, -0.5f,  0.5f,  0.0f, 0.0f },
    {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f },
    { -0.5f, -0.5f, -0.5f,  0.0f, 1.0f },
    {  0.5f, -0.5f, -0.5f,  1.0f, 1.0f },
    {  0.5f, -0.5f,  0.5f,  1.0f, 0.0f },
    {  0.5f, -0.5f,  0.5f,  1.0f, 0.0f },
    { -0.5f, -0.5f,  0.5f,  0.0f, 0.0f },
    { -0.5f, -0.5f, -0.5f,  0.0f, 1.0f },
    { -0.5f,  0.5f, -0.5f,  0.0f, 1.0f },
    {  0.5f,  0.5f, -0.5f,  1.0f, 1.0f },
    {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f },
    {  0.5f,  0.5f,  0.5f,  1.0f, 0.0f },
    { -0.5f,  0.5f,  0.5f,  0.0f, 0.0f },
    { -0.5f,  0.5f, -0.5f,  0.0f, 1.0f }
    };

uint32_t indexData[] = { 0, 1, 3, 1, 2, 3 };

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
GLFWwindow* wnd = createWindow(sWindowWidth, sWindowHeight);
glrMeshType triangle = { 0 };

/* Setup */
glrInit(sWindowHeight, sWindowWidth);

triangle.pos[0] = 0.0f;
triangle.pos[1] = 0.0f;
triangle.pos[2] = -2.0f;

//glrInitTriangle(&triangle, triangleData, 4, indexData, 6, "..\\Assets\\Textures\\container.jpg");
glrInitTriangle(&triangle, triangleData, 36, NULL, 0, "..\\Assets\\Textures\\container.jpg");

while(!glfwWindowShouldClose(wnd))
    {
    glrInitScene(0x01050000);
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