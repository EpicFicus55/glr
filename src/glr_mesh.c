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

if(!mesh)
	{
	printf("Invalid mesh pointer.\n");
	return;
	}

/* Find out data size */
switch(dataType)
	{
	case GLR_POS3_TEX2_TYPE:
		dataSize = sizeof(glrPos3Tex2Type);
		break;
	default:
		printf("Invalid vertex data format.\n");
		return;
	}

mesh->vertCnt = vertCnt;
mesh->vertSize = dataSize;

/* Initialize the buffers */
__gl(glCreateBuffers(1, &mesh->vbo));
__gl(glNamedBufferData(mesh->vbo, dataSize * vertCnt, vertData, GL_STATIC_DRAW));

if (indices)
	{
	mesh->indexCnt = indexCount;
	__gl(glCreateBuffers(1, &mesh->ebo));
	__gl(glNamedBufferData(mesh->ebo, sizeof(indices[0]) * indexCount, indices, GL_STATIC_DRAW));
	}
else
	{
	mesh->indexCnt = 0;
	}

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
