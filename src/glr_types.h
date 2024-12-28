#pragma once
#include <stdint.h>

#include "cglm/cglm.h"

typedef enum 
	{
	GLR_POS3_TYPE = 0,
	GLR_POS3_TEX2_TYPE,
	GLR_POS3_NORM3_TEX2_TYPE,

	GLR_UNKNOWN_TYPE
	} glrVertDataType;


typedef struct {
	vec3		pos;
	} glrPos3Type;

typedef struct {
	vec3		pos;
	vec2		tex;
	} glrPos3Tex2Type;

typedef struct {
	vec3		pos;
	vec3		norm;
	vec2		tex;
	} glrPos3Norm3Tex2Type;

typedef struct 
	{
	/* Buffer handles */
	uint32_t	vbo;
	uint32_t	ebo;

	/* Data information */
	size_t		vertSize; /* Size of one vertex */
	size_t		vertCnt;  /* Number of vertices  */
	size_t		indexCnt; /* Number of idices (optional) */

	/* Position and model matrix */
	vec3		pos;
	mat4		modelMat;
	mat4		normalMat;

	/* Texture handles */
	uint32_t	albedo_tex;
	} glrMeshType;

typedef struct {
	vec3		pos;
	vec4		clr;
	float		ambientIntensity;

	/* Optional, if there is a mesh
	associated with the light source. */
	glrMeshType* mesh;

} glrLightSourceType;