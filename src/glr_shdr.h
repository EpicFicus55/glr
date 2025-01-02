#include <stdint.h>

#include "cglm/cglm.h"

#define GLR_SHADER_DIR "..\\src\\glsl\\"

/*
 * Compiles and links a shader program based 
 * on the provided .glsl files.
 */
void glrGenerateShaderProgram
	(
	uint32_t*		shdrProg,
	char*			vertShaderName,
	char*			fragShaderName
	);

/*
Set a vec4 uniform.
*/
void shdrSetVec4Uniform
	(
	unsigned int	shader,
	const char*		name,
	vec4			value
	);

/*
Set a float uniform.
*/
void shdrSetFloatUniform
	(
	unsigned int	shader,
	const char*		name,
	float			value
	);

/*
Set an int uniform.
*/
void shdrSetIntUniform
	(
	unsigned int	shader,
	const char*		name,
	int	value
	);

/*
Set a vec3 uniform.
*/
void shdrSetVec3Uniform
	(
	unsigned int	shader,
	const char*		name,
	vec3			value
	);

/*
Set a mat4 uniform.
*/
void shdrSetMat4Uniform
	(
	unsigned int	shader,
	const char*		name,
	mat4			value
	);
