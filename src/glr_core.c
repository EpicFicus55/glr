#include "stb/stb_image.h"

#include "glad/glad.h"
#include "glr_camera.h"
#include "glr_core.h"
#include "glr_shdr.h"
#include "glr_texture.h"
#include "glr_mesh.h"
#include "glr_utils.h"

typedef struct {
	uint32_t	windowWidth;
	uint32_t	windowHeight;

	uint32_t	shdr;
	uint32_t	vao;

	glrCameraType*	camera;

	mat4		viewMat;
	mat4		projMat;
}glr_core_t;

static glr_core_t GLR_core;

/*
 * Initializes the GLR rendering library
 */
void glrInit
	(
	uint32_t width,
	uint32_t height
	)
{
/* Set the window properties */
GLR_core.windowWidth = width;
GLR_core.windowHeight = height;

/* Compile shaders */
glrGenerateShaderProgram(&GLR_core.shdr, "..\\src\\glsl\\test.vert", "..\\src\\glsl\\test.frag");

__gl(glGenVertexArrays(1, &GLR_core.vao));
__gl(glBindVertexArray(GLR_core.vao));

__gl(glEnableVertexAttribArray(0));
__gl(glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(glrPos3Tex2Type, pos)));
__gl(glVertexAttribBinding(0, 0));

__gl(glEnableVertexAttribArray(1));
__gl(glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(glrPos3Tex2Type, tex)));
__gl(glVertexAttribBinding(1, 0));

__gl(glBindVertexArray(0));

__gl(glViewport(0, 0, GLR_core.windowWidth, GLR_core.windowHeight));

/* Initialize the View and Projection matrices */
glm_mat4_identity(GLR_core.viewMat);
glm_mat4_identity(GLR_core.projMat);

glm_perspective(45.0f, ((float)GLR_core.windowWidth / (float)GLR_core.windowHeight), 0.1f, 100.0f, GLR_core.projMat);

}


/*
 * Initializes the GLR rendering library
 */
void glrAttachCamera
	(
	glrCameraType* camera
	)
{
GLR_core.camera = camera;

}


/*
 * Idk man
 */
void glrUpdateCameraAngle
	(
    double      xPosIn,
    double      yPosIn
	)
{
glrCameraMouseCallback(GLR_core.camera, xPosIn, yPosIn);

}


/*
 * Initializes the scene
 */
void glrInitScene
	(
	uint32_t clr
	)
{
__gl(glClearColor
		(
		0.1f, /* Placeholder colors */
		0.8f,
		0.5f,
		0.0f
		));
__gl(glClear(GL_COLOR_BUFFER_BIT));

__gl(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
__gl(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}


/*
 * Renders a mesh
 */
void glrRenderMesh
	(
	glrMeshType* mesh
	)
{
shdrSetMat4Uniform(GLR_core.shdr, "modelMat", mesh->modelMat);
shdrSetMat4Uniform(GLR_core.shdr, "viewMat", GLR_core.camera->lookAtMatrix);
shdrSetMat4Uniform(GLR_core.shdr, "projMat", GLR_core.projMat);

__gl(glEnable(GL_DEPTH_TEST));

__gl(glBindVertexArray(GLR_core.vao));
glrLoadMesh(mesh);

__gl(glUseProgram(GLR_core.shdr));

if(mesh->ebo)
	{
	__gl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo));
	__gl(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	}
else
	{
	__gl(glDrawArrays(GL_TRIANGLES, 0, 36));
	}
__gl(glUseProgram(0));
__gl(glBindVertexArray(0));

__gl(glDisable(GL_DEPTH_TEST));
}
