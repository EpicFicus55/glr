#include "stb/stb_image.h"

#include "glad/glad.h"
#include "glr_core.h"
#include "glr_shdr.h"
#include "glr_texture.h"
#include "glr_utils.h"

typedef struct {
	uint32_t	shdr;
	uint32_t	vao;
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
/* Compile shaders */
glrGenerateShaderProgram(&GLR_core.shdr, "..\\src\\glsl\\test.vert", "..\\src\\glsl\\test.frag");

__gl(glGenVertexArrays(1, &GLR_core.vao));
__gl(glBindVertexArray(GLR_core.vao));

__gl(glEnableVertexAttribArray(0));
__gl(glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(glrPos3Clr4Tex2Type, pos)));
__gl(glVertexAttribBinding(0, 0));

__gl(glEnableVertexAttribArray(1));
__gl(glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, offsetof(glrPos3Clr4Tex2Type, clr)));
__gl(glVertexAttribBinding(1, 0));

__gl(glEnableVertexAttribArray(2));
__gl(glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(glrPos3Clr4Tex2Type, tex)));
__gl(glVertexAttribBinding(2, 0));

__gl(glBindVertexArray(0));

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
 * Initializes a triangle
 */
void glrInitTriangle
	(
	glrMeshType* mesh,
	glrPos3Clr4Tex2Type* data,
	uint32_t cnt,
	uint32_t* indices,
	uint32_t  indexCount
	)
{
if(!mesh)
	{
	printf("Invalid mesh pointer.\n");
	return;
	}

__gl(glGenBuffers(1, &mesh->vbo));
__gl(glGenBuffers(1, &mesh->ebo));

__gl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo));
__gl(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indexCount, indices, GL_STATIC_DRAW));

__gl(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo));
__gl(glBufferData(GL_ARRAY_BUFFER, sizeof(data[0]) * cnt, data, GL_STATIC_DRAW));

__gl(glBindBuffer(GL_ARRAY_BUFFER, 0));

mesh->albedo_tex = glrInitTexture("..\\Assets\\Textures\\container.jpg");
}


/*
 * Renders a triangle
 */
void glrRenderTriangle
	(
	glrMeshType* mesh
	)
{
__gl(glBindVertexArray(GLR_core.vao));
__gl(glBindTextureUnit(0, mesh->albedo_tex));
__gl(glBindVertexBuffer(0, mesh->vbo, 0, sizeof(glrPos3Clr4Tex2Type)));
__gl(glUseProgram(GLR_core.shdr));
__gl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo));
__gl(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
__gl(glUseProgram(0));
__gl(glBindVertexArray(0));
}
