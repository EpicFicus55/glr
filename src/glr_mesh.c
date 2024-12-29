#include <string.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glr_mesh.h"
#include "glr_shdr.h"

static unsigned int gMeshIdx = 0;
static char pathToModel[128];


/*
 * Function for recursively processing a model node.
 */
static void modelNodeProcess
	(
	glrModelType*			model,
	struct aiNode*			node,
	const struct aiScene*	scene
	);
/*
 * Processes an assimp mesh and saves the data to a GLR mesh.
 */
static void processMesh
	(
	glrModelType*			model,
	glrMeshType*			node,
	struct aiMesh*			assimpMesh,		
	const struct aiScene*	scene
	);

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
 * Initializes a model from a given .obj file.
 * 
 */
void glrInitModel
	(
	glrModelType*	model,
	char*			modelName,
	vec3			modelPos
	)
{
char pathToObj[128];

if(!model || !modelName)
	{ 
	printf("Invalid model pointer or path.\n");
	return;
	}

sprintf(pathToModel, "%s%s\\", GLR_MODELS_DIR, modelName);
sprintf(pathToObj, "%s%s\\%s.obj", GLR_MODELS_DIR, modelName, modelName);

/* Load the model using Assimp */
const struct aiScene* scene = aiImportFile
	(
	(const char*)pathToObj,
	aiProcess_Triangulate | aiProcess_FlipUVs
	);

if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
	printf("Invalid model!\n");
	return;
	}

gMeshIdx = 0;

model->meshCount = scene->mNumMeshes;
model->meshArray = (glrMeshType*)calloc(model->meshCount, sizeof(glrMeshType));

/* Process the Assimp model */
modelNodeProcess(model, scene->mRootNode, scene);

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


/*
 * Function for recursively processing a model node.
 */
static void modelNodeProcess
	(
	glrModelType*			model,
	struct aiNode*			node,
	const struct aiScene*	scene
	)
{
if(!node)
	{
	return;
	}

/* Process each mesh in the current node */
for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
	/* */
	processMesh(model, &model->meshArray[gMeshIdx++], scene->mMeshes[node->mMeshes[i]], scene);
	}

/* Recursively process the child nodes */
for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
	if(node->mChildren)
		{
		modelNodeProcess(model, node->mChildren[i], scene);
		}
	}

}


/*
 * Processes an assimp mesh and saves the data to a GLR mesh.
 */
static void processMesh
	(
	glrModelType*			model,
	glrMeshType*			mesh,
	struct aiMesh*			assimpMesh,		
	const struct aiScene*	scene
	)
{
glrPos3Norm3Tex2Type* vertexData = NULL;
unsigned int vertexCount = 0;
unsigned int* indexData = NULL;
unsigned int indexCount = 0;
char diffuseTexName[64] = {0};
char specularTexName[64] = {0};

/* Allocate memory for the mesh vertices */
vertexCount = assimpMesh->mNumVertices;
vertexData = (glrPos3Norm3Tex2Type*)malloc(vertexCount * sizeof(glrPos3Norm3Tex2Type));
if(!vertexData)
	{
	printf("Vertex memory allocation failed.\n");
	return;
	}

/* Construct the vertex data */
for(unsigned int i = 0; i < assimpMesh->mNumVertices; i++)
	{
	vertexData[i].pos[0] = assimpMesh->mVertices[i].x;
	vertexData[i].pos[1] = assimpMesh->mVertices[i].y;
	vertexData[i].pos[2] = assimpMesh->mVertices[i].z;

	vertexData[i].norm[0] = assimpMesh->mNormals[i].x;
	vertexData[i].norm[1] = assimpMesh->mNormals[i].y;
	vertexData[i].norm[2] = assimpMesh->mNormals[i].z;

	vertexData[i].tex[0] = assimpMesh->mTextureCoords[0][i].x;
	vertexData[i].tex[1] = assimpMesh->mTextureCoords[0][i].y;
	}

/* Allocate memory for the indices */
indexCount = assimpMesh->mNumFaces * assimpMesh->mFaces[0].mNumIndices;
indexData = (unsigned int*)malloc(indexCount * sizeof(unsigned int));
if(!indexData)
	{
	printf("Vertex index memory allocation failed.\n");
	return;
	}

/* Construct the index data */
unsigned int currIdx = 0;
for (unsigned int i = 0; i < assimpMesh->mNumFaces; i++ )
	{
	struct aiFace face = assimpMesh->mFaces[i];
	for(unsigned int j = 0; j < face.mNumIndices; j++)
		{
		indexData[currIdx++] = face.mIndices[j];
		}
	}

/* Get the material textures */
char pathToDiffuse[128];
char pathToSpecular[128];

if(assimpMesh->mMaterialIndex >= 0)
	{
	struct aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];
	unsigned int diffuseTexCnt = 0; /* Diffuse map count */
	unsigned int specularTexCnt = 0; /* Specular map count */
	unsigned int currTxt = 0;
	struct aiString assimpDiffuseName = {0};
	struct aiString assimpSpecularName = {0};

	diffuseTexCnt = aiGetMaterialTextureCount(material, aiTextureType_DIFFUSE);
	specularTexCnt = aiGetMaterialTextureCount(material, aiTextureType_SPECULAR);

	/* Get the diffuse and specular textures in a nice way */
	if(diffuseTexCnt > 0)
		{
		aiGetMaterialTexture(material, aiTextureType_DIFFUSE, 0, &assimpDiffuseName, NULL, NULL, NULL, NULL, NULL, NULL);
		if(assimpDiffuseName.length < sizeof(diffuseTexName))
			{
			strcpy(diffuseTexName, assimpDiffuseName.data);
			}
		}
	if(specularTexCnt > 0)
		{
		aiGetMaterialTexture(material, aiTextureType_SPECULAR, 0, &assimpSpecularName, NULL, NULL, NULL, NULL, NULL, NULL);
		if(assimpSpecularName.length < sizeof(specularTexName))
			{
			strcpy(specularTexName, assimpSpecularName.data);
			}
		}

	/* Construct the paths */
	sprintf(pathToDiffuse, "%s%s", pathToModel, diffuseTexName);
	sprintf(pathToSpecular, "%s%s", pathToModel, specularTexName);

	}

/* Initialize the GLR mesh */
glrInitMesh
	(
	mesh,
	model->modelPos,
	GLR_POS3_NORM3_TEX2_TYPE,
	vertexData,
	vertexCount,
	indexData,
	indexCount,
	pathToDiffuse,
	pathToSpecular
	);

/* Deallocate the buffers */
free(vertexData);
free(indexData);
}