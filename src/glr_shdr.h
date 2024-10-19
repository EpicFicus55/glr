#include <stdint.h>

#include "cglm/cglm.h"

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
void shdr_set_vec4_uniform
	(
	unsigned int	shader,
	const char*		name,
	vec4			value
	);
