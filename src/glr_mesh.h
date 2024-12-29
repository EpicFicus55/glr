#pragma once
#include "glr_types.h"
#include "glr_utils.h"

#define GLR_MODELS_DIR		"..\\Assets\\Models\\"

/*
 * Loads a mesh for rendering
 */
void glrLoadMesh
	(
	glrMeshType* mesh
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
