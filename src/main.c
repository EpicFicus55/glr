#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "cglm/cglm.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glr_core.h"
#include "glr_camera.h"

static const uint32_t  sWindowHeight = 1200;
static const uint32_t  sWindowWidth = 1600;

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

/* Camera parameters */
vec3 cameraPos =    {0.0f, 0.0f, 3.0f};
vec3 cameraFront =  {0.0f, 0.0f, -1.0f};
vec3 cameraUp =     {0.0f, 1.0f, 0.0f};

/*
 * Window creation function
 */
GLFWwindow* createWindow
    (
    uint32_t    width,
    uint32_t    height
    );

/*
 * Process input
 */
void processGLFWInput
    (
    GLFWwindow* window,
    glrCameraType* camera
    );

/*
 * Mouse callback
 */
void processGLFWMouse
    (
    GLFWwindow* window,
    double      xPosIn,
    double      yPosIn
    );

int main(void)
{
GLFWwindow* wnd = createWindow(sWindowWidth, sWindowHeight);
glrCameraType camera = { 0 };
glrMeshType containerMesh = { 0 };
glrMeshType containerMesh2 = { 0 };

/* Setup */
glrInit(sWindowWidth, sWindowHeight);
glrInitCamera(&camera, cameraPos, cameraFront, cameraUp, 0.005f);
glrAttachCamera(&camera);
glfwSetCursorPosCallback(wnd, processGLFWMouse);

containerMesh.pos[0] = 0.0f;
containerMesh.pos[1] = 0.0f;
containerMesh.pos[2] = -2.0f;

containerMesh2.pos[0] = 10.0f;
containerMesh2.pos[1] = 3.0f;
containerMesh2.pos[2] = -2.0f;

glrInitMesh
    (
    &containerMesh,
    GLR_POS3_TEX2_TYPE,
    triangleData,
    36, 
    NULL, 
    0, 
    "..\\Assets\\Textures\\container.jpg"
    );

glrInitMesh
    (
    &containerMesh2,
    GLR_POS3_TEX2_TYPE,
    triangleData,
    36, 
    NULL, 
    0, 
    "..\\Assets\\Textures\\container.jpg"
    );

while(!glfwWindowShouldClose(wnd))
    {
    /* Get keyboard input and update camera accordingly */
    processGLFWInput(wnd, &camera);

    glrInitScene(0x01050000);

    glrRenderMesh(&containerMesh);
    glrRenderMesh(&containerMesh2);

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
glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

GLFWwindow* wnd = glfwCreateWindow(width, height, "GLR", NULL, NULL);
assert(wnd != NULL);

glfwMakeContextCurrent(wnd);
glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
    printf( "Failed to initialize GLAD" );
    return NULL;
	} 

return wnd;
}

/*
 * Process input
 */
void processGLFWInput
    (
    GLFWwindow* window,
    glrCameraType* camera
    )
{
if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { glrCameraWCallback(camera); }
if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { glrCameraACallback(camera); }
if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { glrCameraSCallback(camera); }
if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { glrCameraDCallback(camera); }
if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, 1); }
}


/*
 * Mouse callback
 */
void processGLFWMouse
    (
    GLFWwindow* window,
    double      xPosIn,
    double      yPosIn
    )
{
glrUpdateCameraAngle(xPosIn, yPosIn);
}