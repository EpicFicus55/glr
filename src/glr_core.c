#include "glad/glad.h"
#include "glr_core.h"
#include "glr_shdr.h"
#include "glr_utils.h"

typedef struct {
	uint32_t	shdr;
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


}


/* 
 * Initializes a triangle
 */
void glrInitTriangle
	(
	glrMeshType* mesh,
	glrPos3Clr1Type* data,
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

__gl(glGenVertexArrays(1, &mesh->vao));
__gl(glGenBuffers(1, &mesh->vbo));
__gl(glGenBuffers(1, &mesh->ebo));
__gl(glBindVertexArray(mesh->vao));

__gl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo));
__gl(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indexCount, indices, GL_STATIC_DRAW));

__gl(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo));
__gl(glBufferData(GL_ARRAY_BUFFER, sizeof(data[0]) * cnt, data, GL_STATIC_DRAW));

__gl(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glrPos3Clr1Type), (void*)offsetof(glrPos3Clr1Type, pos)));
__gl(glEnableVertexAttribArray(0));

__gl(glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(glrPos3Clr1Type), (void*)offsetof(glrPos3Clr1Type, clr)));
__gl(glEnableVertexAttribArray(1));

__gl(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


/*
 * Renders a triangle
 */
void glrRenderTriangle
	(
	glrMeshType* mesh
	)
{
__gl(glBindVertexArray(mesh->vao));
__gl(glUseProgram(GLR_core.shdr));
__gl(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
__gl(glUseProgram(0));
__gl(glBindVertexArray(0));
}
