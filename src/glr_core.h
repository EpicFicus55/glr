#pragma once

#include "glr_camera.h"
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
void glrInitMesh
	(
	glrMeshType* mesh,
	glrPos3Tex2Type* data,
	uint32_t cnt,
	uint32_t* indices,
	uint32_t  indexCount,
	char*	albedoPath
	);

/*
 * Renders a mesh
 */
void glrRenderMesh
	(
	glrMeshType* mesh
	);
