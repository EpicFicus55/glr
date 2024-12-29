#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "cglm/cglm.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glr_vertex_data.h"
#include "glr_core.h"
#include "glr_camera.h"

static const uint32_t  sWindowHeight = 1200;
static const uint32_t  sWindowWidth = 1600;

/* Camera parameters */
vec3 cameraPos =    {0.0f, 0.0f, 3.0f};
vec3 cameraFront =  {0.0f, 0.0f, -1.0f};
vec3 cameraUp =     {0.0f, 1.0f, 0.0f};

/* Mesh data */
vec3 containerPos  = {0.0f, 0.0f, -3.0f};
vec3 container2Pos = {5.0f, 3.0f,-7.0f};

/* Light data */
vec3 lightPos = { 5.0f, 1.5f, -1.0f };
vec4 lightClr = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightAmbientIntensity = 0.2f;

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

glrMeshType lightMesh = { 0 };
glrLightSourceType lightSource = { 0 };

/* Setup */
glrInit(sWindowWidth, sWindowHeight);
glrInitCamera(&camera, cameraPos, cameraFront, cameraUp, 0.005f);
glrAttachCamera(&camera);
glfwSetCursorPosCallback(wnd, processGLFWMouse);

/* Initialize objects in the scene */
glrInitMesh
    (
    &containerMesh,
    containerPos,
    GLR_POS3_NORM3_TEX2_TYPE,
    cubeData3p3n2t,
    36, 
    NULL, 
    0, 
    "..\\Assets\\Textures\\container2.png",
    "..\\Assets\\Textures\\container2_specular.png"
    );

glrInitMesh
    (
    &containerMesh2,
    container2Pos,
    GLR_POS3_NORM3_TEX2_TYPE,
    cubeData3p3n2t,
    36, 
    NULL, 
    0, 
    "..\\Assets\\Textures\\container2.png",
    "..\\Assets\\Textures\\container2_specular.png"
    );

glrInitMesh
    (
    &lightMesh,
    lightPos,
    GLR_POS3_TYPE,
    cubeData3p,
    36,
    NULL,
    0,
    NULL,
    NULL
    );

glrInitLightSource
    (
    &lightSource,
    lightPos,
    lightClr,
    lightAmbientIntensity,
    &lightMesh
    );

while(!glfwWindowShouldClose(wnd))
    {
    /* Get keyboard input and update camera accordingly */
    processGLFWInput(wnd, &camera);
    glrInitScene(0x01050000);

    /* Optional - render the light mesh */
    glrRenderLightSource(&lightMesh);

    /* Render the objects in the scene */
    glrRenderMesh(&containerMesh);
    glrRenderMesh(&containerMesh2);

    glfwSwapBuffers(wnd);
    glfwPollEvents();
    }

/* Cleanup */
glrFreeMesh(&containerMesh);
glrFreeMesh(&containerMesh2);

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