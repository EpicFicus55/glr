#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "glr_shdr.h"
#include "glr_utils.h"

static void readFileInto
	(
	char*	input_file_name,
	char**	output_buffer
	);

static void compileShader
	(
	uint32_t*		shader,
	GLenum			shader_type,
	char*			input_file_name
	);

/*
 * Compiles and links a shader program based 
 * on the provided .glsl files.
 */
void glrGenerateShaderProgram
	(
	uint32_t*		shdrProg,
	char*			vertShaderName,
	char*			fragShaderName
	)
{
unsigned int	vertShdr = 0;
unsigned int	fragShdr = 0;
int				success = 0;
char			infoLog[ 512 ];

/* Clear _info_log */
memset(&infoLog[ 0 ], 0, sizeof(infoLog));

/* Compile the vertex shader */
compileShader(&vertShdr, GL_VERTEX_SHADER, vertShaderName);

/* Compile the fragment shader */
compileShader(&fragShdr, GL_FRAGMENT_SHADER, fragShaderName);

/* Create a new shader program */
__gl(*shdrProg = glCreateProgram());

/* Attach the shaders and compile the program */
__gl(glAttachShader(*shdrProg, vertShdr));
__gl(glAttachShader(*shdrProg, fragShdr));
__gl(glLinkProgram(*shdrProg));

/* Validate the compilation */
__gl(glGetProgramiv(*shdrProg, GL_LINK_STATUS, &success));
if (!success)
	{
	glGetProgramInfoLog(*shdrProg, sizeof(infoLog), NULL, infoLog);
	printf("Linking of the shader program failed: %s\n", infoLog);
	}

/* Delete the shader programs */
__gl(glDeleteShader(vertShdr));
__gl(glDeleteShader(fragShdr));
}


/*
Set a float uniform.
*/
void shdrSetFloatUniform
	(
	unsigned int	shader,
	const char*		name,
	float			value
	)
{
GLint location = 0;

__gl(glUseProgram(shader));
__gl(location = glGetUniformLocation(shader, name));
__gl(glUniform1f(location, value));
__gl(glUseProgram(0));

}
/*
Set a vec3 uniform.
*/
void shdrSetVec3Uniform
	(
	unsigned int	shader,
	const char*		name,
	vec3			value
	)
{
GLint location = 0;

__gl(glUseProgram(shader));
__gl(location = glGetUniformLocation(shader, name));
__gl(glUniform3fv(location, 1, value));
__gl(glUseProgram(0));

}

/*
Set a vec4 uniform.
*/
void shdrSetVec4Uniform
	(
	unsigned int	shader,
	const char*		name,
	vec4			value
	)
{
GLint location = 0;

__gl(glUseProgram(shader));
__gl(location = glGetUniformLocation(shader, name));
__gl(glUniform4fv(location, 1, value));
__gl(glUseProgram(0));

}


/*
Set a mat4 uniform.
*/
void shdrSetMat4Uniform
	(
	unsigned int	shader,
	const char*		name,
	mat4			value
	)
{
GLint location = 0;

__gl(glUseProgram(shader));
__gl(location = glGetUniformLocation(shader, name));
__gl(glUniformMatrix4fv(location, 1, GL_FALSE, value[0]));
__gl(glUseProgram(0));

}

/*
 * Compiles a .glsl file provided as argument.
 */
static void compileShader
	(
	uint32_t*	shader,
	GLenum		shaderType,
	char*		inputFileName
	)
{
char*	shaderSource = NULL;
int		success = 0;
char	infoLog[ 512 ];

/* Clear _info_log */
memset(&infoLog[ 0 ], 0, sizeof(infoLog));

/* Generate a shader */
__gl(*shader = glCreateShader(shaderType));

/* Save the shader source in the local buffer */
readFileInto( inputFileName, &shaderSource );

/* Compile the shader */
__gl(glShaderSource(*shader, 1, &shaderSource, NULL));
__gl(glCompileShader(*shader ));

/* Validate compilation */
__gl(glGetShaderiv(*shader, GL_COMPILE_STATUS, &success));
if (!success)
	{
	glGetShaderInfoLog(*shader, sizeof(infoLog), NULL, infoLog);
	printf("Compilation of the shader: %s\n", infoLog);
	}

/* Free the buffer */
free( shaderSource );
}

/*
 * Stores the contents of input_file_name
 * into the output_buffer and saves the
 * size of the file.
 *
 * NOTE: the output buffer needs to be freed
 * after it is no longer needed.
 */
static void readFileInto
	(
	char*	inputFileName,
	char**	outputBuffer
	)
{
/* Local variables */
FILE*		inputStream = NULL;
errno_t		err = 0;
uint64_t	fileSize = 0;

/* Begin the file stream. Print a message if file doesn't exist */
err = fopen_s(&inputStream, inputFileName, "r");
if( err )
	{
	printf("Error reading GLSL file: %s\n", inputFileName);
	return;
	}

/* Find out how long the file is */
fseek(inputStream, 0, SEEK_END);
fileSize = ftell(inputStream);

/* Reset the stream */
rewind(inputStream);

/* Allocate enough memory to keep all the contents of the file */
*outputBuffer = (char*)calloc(fileSize, sizeof(char));
fread_s(*outputBuffer, fileSize, 1, fileSize, inputStream);

/* Close the file stream */
fclose(inputStream);

}