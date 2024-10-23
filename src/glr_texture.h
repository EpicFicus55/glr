#pragma once
#include <stdint.h>

/*
 * Initializes a texture handle, loads the texture data from the 
 * path.
 */
uint32_t glrInitTexture
	(
	const char* texFilePath
	);
