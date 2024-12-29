#include <string.h>

#include "glr_mesh.h"
#include "glr_shdr.h"

/*
 * Initializes a mesh
 */
void glrInitMesh
	(
	glrMeshType*	mesh,
	vec3			meshPos,
	glrVertDataType dataType,
	void*			vertData,
	uint32_t		vertCnt,
	uint32_t*		indices,
	uint32_t		indexCount,
	char*			albedoPath,
	char*			specularPath
	)
{
size_t dataSize = 0;

if(!mesh)
	{
	printf("Invalid mesh pointer.\n");
	return;
	}

/* Set the position */
memcpy(mesh->pos, meshPos, sizeof(vec3));

/* Find out data size */
switch(dataType)
	{
	case GLR_POS3_TEX2_TYPE:
		dataSize = sizeof(glrPos3Tex2Type);
		break;
	case GLR_POS3_TYPE:
		dataSize = sizeof(glrPos3Type);
		break;
	case GLR_POS3_NORM3_TEX2_TYPE:
		dataSize = sizeof(glrPos3Norm3Tex2Type);
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
if(specularPath)
	{ 
	mesh->specular_tex = glrInitTexture(specularPath);
	}

/* Initialize the model matrix */
glm_mat4_identity(mesh->modelMat);
glm_translate(mesh->modelMat, mesh->pos);

/* If the vertex data has normals, compute the normal matrix */
glm_mat4_identity(mesh->normalMat);
if(dataType == GLR_POS3_NORM3_TEX2_TYPE)
	{
	glm_mat4_inv(mesh->modelMat, mesh->normalMat);
	glm_mat4_transpose(mesh->normalMat);
	}

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
__gl(glBindTextureUnit(1, mesh->specular_tex));

__gl(glBindVertexBuffer(0, mesh->vbo, 0, mesh->vertSize));
}


/*
 * Deletes all the OpenGL objects
 * associated with the mesh. Also
 * sets everything to 0.
 */
void glrFreeMesh
	(
	glrMeshType* mesh
	)
{
if(!mesh)
	{
	printf("Attempting to delete an invalid mesh.\n");
	return;
	}

__gl(glDeleteBuffers(1, &mesh->vbo));
__gl(glDeleteBuffers(1, &mesh->ebo));
__gl(glDeleteTextures(1, &mesh->albedo_tex));

memset(mesh, 0, sizeof(glrMeshType));
}	