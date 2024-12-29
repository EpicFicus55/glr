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

	GLR_SHADER_MAX
	} glrShaderProgramType;\

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
 * Initializes a mesh
 */
void glrInitMeshFromModel
	(
	glrMeshType*	mesh,
	char*			modelPath,
	vec3			meshPos
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
 * Renders a light source mesh
 */
void glrRenderLightSource
	(
	glrMeshType* lightSource
	);
