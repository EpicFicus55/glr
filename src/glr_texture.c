#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "glr_types.h"
#include "glr_utils.h"
#include "glr_texture.h"

static glrTextureType gTextures[16];
static unsigned int	  gTextureCnt = 0;

/*
 * Initializes a texture handle, loads the texture data from the 
 * path.
 */
uint32_t glrInitTexture
	(
	const char* texFilePath
	)
{
uint32_t	texture = 0;
uint32_t	width = 0;
uint32_t	height = 0;
uint32_t	nrChannels = 0;
uint8_t*	texData = NULL;
GLenum		format = 0;

/* Find if the texture was already loaded before */
for(unsigned int i = 0; i < gTextureCnt; i++)
	{
	if(strcmp(texFilePath, gTextures[i].path) == 0)
		{
		return gTextures[i].glHndl;
		}
	}

__gl(glCreateTextures(GL_TEXTURE_2D, 1, &texture));
__gl(glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
__gl(glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
__gl(glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
__gl(glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

texData = stbi_load(texFilePath, &width, &height, &nrChannels, 0);

if(!texData)
	{
	return (uint32_t)(-1);
	}

switch(nrChannels) 
	{
	case 1:
		format = GL_RED;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		return (uint32_t)(-1);
	}

__gl(glTextureStorage2D(texture, 1, GL_RGB8, width, height));
__gl(glTextureSubImage2D(texture, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texData));

stbi_image_free(texData);

/* Save the current texture for later use */
gTextures[gTextureCnt].glHndl = texture;
strcpy(gTextures[gTextureCnt].path, texFilePath);
gTextureCnt++;

return texture;
}
