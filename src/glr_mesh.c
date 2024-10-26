#include "glr_mesh.h"
#include "glr_shdr.h"

/*
 * Initializes a mesh
 * - position must be specified before initialization
 */
void glrInitMesh
	(
	glrMeshType*	mesh,
	glrVertDataType dataType,
	void*			vertData,
	uint32_t		vertCnt,
	uint32_t*		indices,
	uint32_t		indexCount,
	char*			albedoPath
	)
{
size_t dataSize = 0;

if (!mesh)
	{
	printf("Invalid mesh pointer.\n");
	return;
	}

/* Find out data size */
switch(dataType)
	{
	case GLR_POS3_TYPE:
		dataSize = sizeof(glrPos3Type);
		break;
	case GLR_POS3_CLR1_TYPE:
		dataSize = sizeof(glrPos3Clr1Type);
		break;
	case GLR_POS3_CLR4_TYPE:
		dataSize = sizeof(glrPos3Clr4Type);
		break;
	case GLR_POS3_TEX2_TYPE:
		dataSize = sizeof(glrPos3Tex2Type);
		break;
	case GLR_POS3_CLR4_TEX2_TYPE:
		dataSize = sizeof(glrPos3Clr4Tex2Type);
		break;
	default:
		printf("Invalid vertex data format.\n");
		return;
	}
mesh->vertSize = dataSize;

/* Initialize the buffers */
__gl(glGenBuffers(1, &mesh->vbo));

__gl(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo));
__gl(glBufferData(GL_ARRAY_BUFFER, dataSize * vertCnt, vertData, GL_STATIC_DRAW));

if (indices)
	{
	__gl(glGenBuffers(1, &mesh->ebo));
	__gl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo));
	__gl(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indexCount, indices, GL_STATIC_DRAW));
	}

__gl(glBindBuffer(GL_ARRAY_BUFFER, 0));

/* Initialize the textures */
if(albedoPath)
	{ 
	mesh->albedo_tex = glrInitTexture(albedoPath);
	}

/* Initialize the model matrix */
glm_mat4_identity(mesh->modelMat);
glm_translate(mesh->modelMat, mesh->pos);

}


/*
 * Loads a mesh for rendering
 */
void glrLoadMesh
	(
	glrMeshType* mesh
	)
{
__gl(glBindTextureUnit(0, mesh->albedo_tex));
__gl(glBindVertexBuffer(0, mesh->vbo, 0, mesh->vertSize));
}
