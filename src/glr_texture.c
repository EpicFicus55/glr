#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "glr_types.h"
#include "glr_utils.h"
#include "glr_texture.h"

const char* gCubeFaces[] = 
				{
				"right",
				"left",
				"top",
				"bottom",
				"front",
				"back"
				};

static glrTextureType gTextures[16];
static uint32_t		  gTextureCnt = 0;

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
	if((strcmp(texFilePath, gTextures[i].path) == 0) && (gTextures[i].type == GL_TEXTURE_2D))
		{
		return gTextures[i].glHndl;
		}
	}

__gl(glCreateTextures(GL_TEXTURE_2D, 1, &texture));
__gl(glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
__gl(glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
__gl(glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
__gl(glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

stbi_set_flip_vertically_on_load(1);
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
gTextures[gTextureCnt].type = GL_TEXTURE_2D;
strcpy(gTextures[gTextureCnt].path, texFilePath);
gTextureCnt++;

return texture;
}


/*
 * Initializes a cubemap texture.
 */
uint32_t glrInitCubemap
	(
	const char*	cubemapName
	)
{
uint32_t texture;
unsigned char* texData[6] = {0};
glrDimType		texDim;
uint32_t format;

/* Find if the texture was already loaded before */
for(unsigned int i = 0; i < gTextureCnt; i++)
	{
	if((strcmp(cubemapName, gTextures[i].path) == 0) && (gTextures[i].type == GL_TEXTURE_CUBE_MAP))
		{
		return gTextures[i].glHndl;
		}
	}

__gl(glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &texture));

/* Set the texture dimensionss */
texDim.width = 0;
texDim.height = 0;

stbi_set_flip_vertically_on_load(0);
for(unsigned int i = 0; i < 6; i++)
	{
	char imagePath[128];
	int width, height, nrChannels;

	sprintf(imagePath, "%s%s_%s.jpg", GLR_TEXTURES_DIR, cubemapName, gCubeFaces[i]);
	texData[i] = stbi_load(imagePath, &width, &height, &nrChannels, 0);

	if(!texData[i])
		{
		printf("Invalid cubemap face for: %s.\n", gCubeFaces[i]);
		return 0;
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
	
	texDim.width = max(texDim.width, width);
	texDim.height = max(texDim.height, height);
	
	}

__gl(glTextureStorage2D(texture, 1, GL_RGBA8, texDim.width, texDim.height));
__gl(glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
__gl(glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
__gl(glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

for(unsigned int face = 0; face < 6; face++)
	{
	__gl(glTextureSubImage3D
			(
			texture,
			0,
			0,
			0,
			face,
			texDim.width,
			texDim.height,
			1,
			format,
			GL_UNSIGNED_BYTE,
			texData[face]
			));
	}

/* Save the texture for future use */
sprintf(gTextures[gTextureCnt].path, "%s%s", GLR_TEXTURES_DIR, cubemapName);
gTextures[gTextureCnt].type = GL_TEXTURE_CUBE_MAP;
gTextures[gTextureCnt].glHndl = texture;

/* Cleanup */
for(unsigned int i = 0; i < 6; i++)
	{
	stbi_image_free(texData[i]);
	}

return texture;
}


/* 
 * Deletes all the OpenGL objects
 * associated with the skybox. Also
 * sets everything to 0.
 */
void glrFreeSkybox
	(
	glrSkyboxMeshType*	skybox
	)
{
if(!skybox)
	{
	printf("Attempting to delete an invalid skybox.\n");
	return;
	}

__gl(glDeleteBuffers(1, &skybox->vbo));
__gl(glDeleteTextures(1, &skybox->cubeMapTex));

memset(skybox, 0, sizeof(glrSkyboxMeshType));
}