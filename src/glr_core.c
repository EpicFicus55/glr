#include <string.h>

#include "stb/stb_image.h"

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

	glrSkyboxMeshType skybox;
	glrLightSourceType* lightSource;

	glrFramebufferType framebuffer;
	
	uint32_t	screen_vbo;

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
/* Quad covering the entire screen to be used as render space */
glrPos3Tex2Type screen_quad[] =
	{
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f
	};

/* Set the window properties */
GLR_core.windowWidth = width;
GLR_core.windowHeight = height;

/* Compile shaders */
glrGenerateShaderProgram
	(
	&GLR_core.shdr[GLR_SHADER_3P2T],
	"3p2t.vert", 
	"3p2t.frag"
	);
glrGenerateShaderProgram
	(
	&GLR_core.shdr[GLR_SHADER_3P2T_MVP],
	"3p2t_mvp.vert", 
	"3p2t_mvp.frag"
	);
glrGenerateShaderProgram
	(
	&GLR_core.shdr[GLR_SHADER_3P_MVP],
	"3p_mvp.vert", 
	"3p_mvp.frag"
	);
glrGenerateShaderProgram
	(
	&GLR_core.shdr[GLR_SHADER_3P_MVP_TERRAIN],
	"3p_mvp_terrain.vert", 
	"3p_mvp_terrain.frag"
	);
glrGenerateShaderProgram
	(
	&GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG],
	"3p3n2t_mvp_phong.vert", 
	"3p3n2t_mvp_phong.frag"
	);
glrGenerateShaderProgram
	(
	&GLR_core.shdr[GLR_SHADER_SKYBOX],
	"skybox.vert", 
	"skybox.frag"
	);

__gl(glCreateVertexArrays(GLR_VERTEX_FORMAT_MAX, GLR_core.vao));
/* GLR_VERTEX_FORMAT_3P */
__gl(glBindVertexArray(GLR_core.vao[GLR_VERTEX_FORMAT_3P]));

__gl(glEnableVertexAttribArray(0));
__gl(glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(glrPos3Type, pos)));
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

/* Create the framebuffer */
__gl(glCreateFramebuffers(1, &GLR_core.framebuffer.fbo));

/* Create the color attachment */
__gl(glCreateTextures(GL_TEXTURE_2D, 1, &GLR_core.framebuffer.colorAttachment));
__gl(glTextureStorage2D(GLR_core.framebuffer.colorAttachment, 1, GL_RGB8, width, height));

__gl(glCreateRenderbuffers(1, &GLR_core.framebuffer.rbo));
__gl(glNamedRenderbufferStorage(GLR_core.framebuffer.rbo, GL_DEPTH24_STENCIL8, width, height));

/* Bind the color attachment */
__gl(glNamedFramebufferTexture(GLR_core.framebuffer.fbo, GL_COLOR_ATTACHMENT0, GLR_core.framebuffer.colorAttachment, 0));

/* Bind the renderbuffer for depth/stencil */
__gl(glNamedFramebufferRenderbuffer(GLR_core.framebuffer.fbo, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, GLR_core.framebuffer.rbo));

if(glCheckNamedFramebufferStatus(GLR_core.framebuffer.fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{ 
	printf("Incomplete framebuffer.\n");
	exit(0);
	}

__gl(glBindFramebuffer(GL_FRAMEBUFFER, GLR_core.framebuffer.fbo));

__gl(glCreateBuffers(1, &GLR_core.screen_vbo));
__gl(glNamedBufferData(GLR_core.screen_vbo, sizeof(screen_quad), screen_quad, GL_STATIC_DRAW));


/* Initialize the View and Projection matrices */
glm_mat4_identity(GLR_core.viewMat);
glm_mat4_identity(GLR_core.projMat);

glm_perspective(45.0f, ((float)GLR_core.windowWidth / (float)GLR_core.windowHeight), 0.1f, 500.0f, GLR_core.projMat);

glrInitSkyboxMesh(&GLR_core.skybox, "skybox");
}


/*
 * Cleans up the shaders et al
 */
void glrTeardown
	(
	void
	)
{
for(uint8_t i = 0; i < GLR_SHADER_MAX; i++)
	{
	__gl(glDeleteProgram(GLR_core.shdr[i]));
	}

for(uint8_t i = 0; i < GLR_VERTEX_FORMAT_MAX; i++)
	{
	__gl(glDeleteVertexArrays(1, &GLR_core.vao[i]));
	}

__gl(glDeleteRenderbuffers(1, &GLR_core.framebuffer.rbo));
__gl(glDeleteTextures(1, &GLR_core.framebuffer.colorAttachment));
__gl(glDeleteFramebuffers(1, &GLR_core.framebuffer.fbo));

}


/*
 * Binds the default framebuffer and renders
 * the scene as a texture.
 */
void glrFinishRender
	(
	void
	)
{
/* Bind default framebuffer */
__gl(glBindFramebuffer(GL_FRAMEBUFFER, 0));
__gl(glBindVertexArray(GLR_core.vao[GLR_VERTEX_FORMAT_3P2T]));
__gl(glBindVertexBuffer(0, GLR_core.screen_vbo, 0, sizeof(glrPos3Tex2Type)));
__gl(glUseProgram(GLR_core.shdr[GLR_SHADER_3P2T]));

/* Bind GLR's main framebuffer as the texture, and draw */
__gl(glBindTextureUnit(0, GLR_core.framebuffer.colorAttachment));
__gl(glDrawArrays(GL_TRIANGLES, 0, 6));

/* Clean up */
__gl(glBindTextureUnit(0, 0));
__gl(glUseProgram(0));
__gl(glBindVertexArray(0));

/* Bind GLR's main framebuffer as a render targed */
__gl(glBindFramebuffer(GL_FRAMEBUFFER, GLR_core.framebuffer.fbo));
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

shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "uNormalMat", mesh->normalMat);
shdrSetVec3Uniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "uCameraPos", GLR_core.camera->pos);

shdrSetIntUniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "albedoTex", 0);
shdrSetIntUniform(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG], "specularTex", 1);

__gl(glEnable(GL_DEPTH_TEST));

__gl(glBindVertexArray(GLR_core.vao[GLR_VERTEX_FORMAT_3P3N2T]));
glrLoadMesh(mesh);

__gl(glUseProgram(GLR_core.shdr[GLR_SHADER_3P3N2T_MVP_PHONG]));

if(mesh->ebo)
	{
	__gl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo));
	__gl(glDrawElements(GL_TRIANGLES, mesh->indexCnt, GL_UNSIGNED_INT, 0));
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
 * Renders a model
 */
void glrRenderModel
	(
	glrModelType* model
	)
{
for(unsigned int i = 0; i < model->meshCount;i++)
    { 
    glrRenderMesh(&model->meshArray[i]);
    }
}



/*
 * Renders a skybox
 */
void glrRenderSkybox
	(
	glrSkyboxMeshType* skybox
	)
{
mat3 aux;
mat4 view;

glm_mat4_pick3(GLR_core.camera->lookAtMatrix, aux);
glm_mat4_identity(view);
glm_mat4_ins3(aux, view);
shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_SKYBOX], "viewMat", view);
shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_SKYBOX], "projMat", GLR_core.projMat);

__gl(glEnable(GL_DEPTH_TEST));
__gl(glDepthFunc(GL_LEQUAL));
__gl(glBindVertexArray(GLR_core.vao[GLR_VERTEX_FORMAT_3P]));
__gl(glBindVertexBuffer(0, skybox->vbo, 0, skybox->vertSize));

__gl(glUseProgram(GLR_core.shdr[GLR_SHADER_SKYBOX]));
__gl(glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubeMapTex));
__gl(glDrawArrays(GL_TRIANGLES, 0, skybox->vertCnt));
__gl(glUseProgram(0));
__gl(glBindVertexArray(0));
__gl(glDisable(GL_DEPTH_TEST));
}



/*
 * Renders a terrain mesh
 */
void glrRenderTerrainMesh
	(
	glrTerrainMeshType* terrain
	)
{
shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_3P_MVP_TERRAIN], "modelMat", terrain->modelMat);
shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_3P_MVP_TERRAIN], "viewMat", GLR_core.camera->lookAtMatrix);
shdrSetMat4Uniform(GLR_core.shdr[GLR_SHADER_3P_MVP_TERRAIN], "projMat", GLR_core.projMat);

__gl(glEnable(GL_DEPTH_TEST));
__gl(glBindVertexArray(GLR_core.vao[GLR_VERTEX_FORMAT_3P]));
__gl(glUseProgram(GLR_core.shdr[GLR_SHADER_3P_MVP_TERRAIN]));
__gl(glBindVertexBuffer(0, terrain->vbo, 0, terrain->vertSize));
__gl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->ebo));
//__gl(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
for(uint32_t i = 0; i < terrain->numStrips; i++)
	{
	__gl(glDrawElements
			(
			GL_TRIANGLE_STRIP, 
			terrain->numVertsPerStrip, 
			GL_UNSIGNED_INT,
			(void*)(sizeof(uint32_t) * (terrain->numVertsPerStrip) * i)
			)
	);
	}
//__gl(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
__gl(glBindVertexArray(0));
__gl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
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