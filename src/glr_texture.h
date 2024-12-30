#pragma once
#include <stdint.h>

#define GLR_TEXTURES_DIR	"..\\Assets\\Textures\\"

/*
 * Initializes a texture handle, loads the texture data from the 
 * path.
 */
uint32_t glrInitTexture
	(
	const char* texFilePath
	);

/*
 * Initializes a cubemap texture.
 */
uint32_t glrInitCubemap
	(
	const char* cubemapName
	);
