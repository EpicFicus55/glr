#pragma once
#include <stdint.h>

#include "cglm/cglm.h"

typedef enum 
	{
	GLR_POS3_TYPE = 0,
	GLR_POS3_CLR1_TYPE,
	GLR_POS3_CLR4_TYPE,
	GLR_POS3_TEX2_TYPE,
	GLR_POS3_CLR4_TEX2_TYPE,

	GLR_UNKNOWN_TYPE
	} glrVertDataType;

typedef struct {
	vec3 pos;	
	} glrPos3Type;

typedef struct {
	vec3		pos;
	uint32_t	clr;
	} glrPos3Clr1Type;

typedef struct {
	vec3		pos;
	vec4		clr;
	} glrPos3Clr4Type;

typedef struct {
	vec3		pos;
	vec2		tex;
	} glrPos3Tex2Type;

typedef struct {
	vec3		pos;
	vec4		clr;
	vec2		tex;
	} glrPos3Clr4Tex2Type;


typedef struct 
	{
	/* Buffer handles */
	uint32_t	vao;
	uint32_t	vbo;
	uint32_t	ebo;

	/* Data information */
	size_t		vertSize; /* Size of one vertex */

	/* Position and model matrix*/
	vec3		pos;
	mat4		modelMat;

	/* Texture handles */
	uint32_t	albedo_tex;
	} glrMeshType;
