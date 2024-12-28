#include <string.h>
#include "glad/glad.h"
#include "glr_camera.h"
#include "glr_core.h"
#include "glr_shdr.h"
#include "glr_texture.h"
#include "glr_mesh.h"
#include "glr_utils.h"
#include "glr_vertex_data.h"

typedef struct {
	uint32_t	windowWidth;
	uint32_t	windowHeight;

	uint32_t	shdr[GLR_SHADER_MAX];
	uint32_t	vao[GLR_VERTEX_FORMAT_MAX];

	glrCameraType*	camera;

	mat4		viewMat;
	mat4		projMat;

	glrLightSourceType* lightSource;
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
glrGenerateShaderProgram
	(
	&GLR_core.shdr[GLR_SHADER_3P2T_MVP],
	"..\\src\\glsl\\3p2t_mvp.vert", 
	"..\\src\\glsl\\3p2t_mvp.frag"
	);
glrGenerateShaderProgram
	(
	&GLR_core.shdr[GLR_SHADER_3P_MVP],
	"..\\src\\glsl\\3p_mvp.vert", 
	"..\\src\\glsl\\3p_mvp.frag"
	);
glrGenerateShaderProgram
	(
	&GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG],
	"..\\src\\glsl\\3p3n2t_mvp_phong.vert", 
	"..\\src\\glsl\\3p3n2t_mvp_phong.frag"
	);

__gl(glCreateVertexArrays(GLR_VERTEX_FORMAT_MAX, GLR_core.vao));
/* GLR_VERTEX_FORMAT_3P */
__gl(glBindVertexArray(GLR_core.vao[GLR_VERTEX_FORMAT_3P]));

__gl(glEnableVertexAttribArray(0));
__gl(glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(glrPos3Tex2Type, pos)));
__gl(glVertexAttribBinding(0, 0));

/* GLR_VERTEX_FORMAT_3P2T */
__gl(glBindVertexArray(GLR_core.vao[GLR_VERTEX_FORMAT_3P2T]));

__gl(glEnableVertexAttribArray(0));
__gl(glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(glrPos3Tex2Type, pos)));
__gl(glVertexAttribBinding(0, 0));

__gl(glEnableVertexAttribArray(1));
__gl(glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(glrPos3Tex2Type, tex)));
__gl(glVertexAttribBinding(1, 0));

/* GLR_VERTEX_FORMAT_3P3N2T */
__gl(glBindVertexArray(GLR_core.vao[GLR_VERTEX_FORMAT_3P3N2T]));

__gl(glEnableVertexAttribArray(0));
__gl(glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(glrPos3Norm3Tex2Type, pos)));
__gl(glVertexAttribBinding(0, 0));

__gl(glEnableVertexAttribArray(1));
__gl(glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(glrPos3Norm3Tex2Type, norm)));
__gl(glVertexAttribBinding(1, 0));

__gl(glEnableVertexAttribArray(2));
__gl(glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(glrPos3Norm3Tex2Type, tex)));
__gl(glVertexAttribBinding(2, 0));

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
		0.1f,
		0.1f,
		0.0f
		));
__gl(glClear(GL_COLOR_BUFFER_BIT));

__gl(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
__gl(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}


/*
 * Initializes a light source for the scene.
 */
void glrInitLightSource
(
	glrLightSourceType* lightSource,
	vec3				pos,
	vec4				clr,
	float				ambientIntensity,
	glrMeshType*		mesh
	)
{
memcpy(lightSource->pos, pos, sizeof(vec3));
memcpy(lightSource->clr, clr, sizeof(vec4));
lightSource->ambientIntensity = ambientIntensity;
lightSource->mesh = mesh;

GLR_core.lightSource = lightSource;

}
/*
 * Renders a mesh
 */
void glrRenderMesh
	(
	glrMeshType* mesh
	)
{
shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "modelMat", mesh->modelMat);
shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "viewMat", GLR_core.camera->lookAtMatrix);
shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "projMat", GLR_core.projMat);

shdrSetVec3Uniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "uLightPos", GLR_core.lightSource->pos);
shdrSetVec4Uniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "uLightColor", GLR_core.lightSource->clr);
shdrSetFloatUniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "uLightAmbientIntensity", GLR_core.lightSource->ambientIntensity);

shdrSetVec3Uniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "uCameraPos", GLR_core.camera->pos);

shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "uNormalMat", mesh->normalMat);

__gl(glEnable(GL_DEPTH_TEST));

__gl(glBindVertexArray(GLR_core.vao[GLR_VERTEX_FORMAT_3P3N2T]));
glrLoadMesh(mesh);

__gl(glUseProgram(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG]));

if(mesh->ebo)
	{
	__gl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo));
	__gl(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	}
else
	{
	__gl(glDrawArrays(GL_TRIANGLES, 0, mesh->vertCnt));
	}
__gl(glUseProgram(0));
__gl(glBindVertexArray(0));

__gl(glDisable(GL_DEPTH_TEST));
}


/*
 * Renders a light source mesh
 */
void glrRenderLightSource
	(
	glrMeshType* lightSource
	)
{
shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_3P_MVP], "modelMat", lightSource->modelMat);
shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_3P_MVP], "viewMat", GLR_core.camera->lookAtMatrix);
shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_3P_MVP], "projMat", GLR_core.projMat);
shdrSetVec4Uniform(GLR_core.shdr[GLR_SHADER_3P_MVP], "aClr", GLR_core.lightSource->clr);

__gl(glEnable(GL_DEPTH_TEST));

__gl(glBindVertexArray(GLR_core.vao[GLR_VERTEX_FORMAT_3P]));
glrLoadMesh(lightSource);

__gl(glUseProgram(GLR_core.shdr[GLR_SHADER_3P_MVP]));

if (lightSource->ebo)
	{
	__gl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightSource->ebo));
	__gl(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	}
else
	{
	__gl(glDrawArrays(GL_TRIANGLES, 0, lightSource->vertCnt));
	}
__gl(glUseProgram(0));
__gl(glBindVertexArray(0));

__gl(glDisable(GL_DEPTH_TEST));
}