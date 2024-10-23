#include "glr_types.h"

/*
 * Initializes the GLR rendering library
 */
void glrInit
	(
	uint32_t width,
	uint32_t height
	);


/*
 * Initializes the scene
 */
void glrInitScene
	(
	uint32_t clr
	);

/* 
 * Initializes a triangle
 */
void glrInitTriangle
	(
	glrMeshType* mesh,
	glrPos3Tex2Type* data,
	uint32_t cnt,
	uint32_t* indices,
	uint32_t  indexCount,
	char*	albedoPath
	);

/*
 * Renders a triangle
 */
void glrRenderTriangle
	(
	glrMeshType* mesh
	);
