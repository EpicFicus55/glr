#pragma once
#include <stdint.h>

#include "cglm/cglm.h"

typedef enum 
	{
	GLR_POS3_TEX2_TYPE = 0,

	GLR_UNKNOWN_TYPE
	} glrVertDataType;


typedef struct {
	vec3		pos;
	vec2		tex;
	} glrPos3Tex2Type;


typedef struct 
	{
	/* Buffer handles */
	uint32_t	vao;
	uint32_t	vbo;
	uint32_t	ebo;

	/* Data information */
	size_t		vertSize; /* Size of one vertex */
	size_t		vertCnt;  /* Number of vertices  */
	size_t		indexCnt; /* Number of idices (optional) */

	/* Position and model matrix */
	vec3		pos;
	mat4		modelMat;

	/* Texture handles */
	uint32_t	albedo_tex;
	} glrMeshType;
