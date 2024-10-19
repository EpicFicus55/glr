#include <stdio.h>
#include "glad/glad.h"

#if defined(_DEBUG)
	#define	__gl( stmt ) do { \
		GLenum	err = GL_NO_ERROR; \
		glGetError(); \
		stmt; \
		err = glGetError(); \
		if(err != GL_NO_ERROR)	\
			{ \
			printf("%s failed with %d\n.", #stmt, err); \
			} \
		} while(0)
#else
	#define	_gl( stmt ) (stmt)
#endif