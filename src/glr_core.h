#pragma once

#include "glr_camera.h"
#include "glr_types.h"

#define GLR_LIGHT_SOURCE_MAX 10

typedef enum
	{
	GLR_VERTEX_FORMAT_3P = 0,
	GLR_VERTEX_FORMAT_3P2T,
	GLR_VERTEX_FORMAT_3P3N2T,

	GLR_VERTEX_FORMAT_MAX
	} glrVertexFormatType;

typedef enum
	{
	GLR_SHADER_3P2T_MVP = 0,
	GLR_SHADER_3P_MVP,
	GLR_SHADER_3P3N2T_MVP_PHONG,
	GLR_SHADER_SKYBOX,

	GLR_SHADER_MAX
	} glrShaderProgramType;

/*
 * Initializes the GLR rendering library
 */
void glrInit
	(
	uint32_t width,
	uint32_t height
	);

/*
 * Initializes the GLR rendering library
 */
void glrAttachCamera
	(
	glrCameraType* camera
	);


/*
 * Idk man
 */
void glrUpdateCameraAngle
	(
    double      xPosIn,
    double      yPosIn
	);


/*
 * Initializes the scene
 */
void glrInitScene
	(
	uint32_t clr
	);

/*
 * Initializes a model from a given .obj file.
 * 
 */
void glrInitModel
	(
	glrModelType*	mesh,
	char*			modelName,
	vec3			modelPos
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
	);

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
	);


/*
 * Renders a mesh
 */
void glrRenderMesh
	(
	glrMeshType* mesh
	);

/*
 * Renders a model
 */
void glrRenderModel
	(
	glrModelType* model
	);

/*
 * Loads a mesh for rendering
 */
void glrInitSkyboxMesh
	(
	glrSkyboxMeshType* mesh,
	unsigned char* skyboxPath
	);

/*
 * Renders a skybox
 */
void glrRenderSkybox
	(
	glrSkyboxMeshType* skybox
	);

/*
 * Deletes all the OpenGL objects
 * associated with the mesh. Also
 * sets everything to 0.
 */
void glrFreeMesh
	(
	glrMeshType* mesh
	);

/*
 * Deletes all the OpenGL objects
 * associated with the model. Also
 * sets everything to 0.
 */
void glrFreeModel
	(
	glrModelType* model
	);

/* 
 * Deletes all the OpenGL objects
 * associated with the skybox. Also
 * sets everything to 0.
 */
void glrFreeSkybox
	(
	glrSkyboxMeshType*	skybox
	);


/*
 * Renders a light source mesh
 */
void glrRenderLightSource
	(
	glrMeshType* lightSource
	);
