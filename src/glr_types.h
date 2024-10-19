#include <stdint.h>

#include "cglm/cglm.h"

typedef struct {
	vec3 pos;	
	} glrPos3Type;

typedef struct {
	vec3		pos;
	uint32_t	clr;
	} glrPos3Clr1Type;


typedef struct {
	uint32_t	vao;
	uint32_t	vbo;
	uint32_t	ebo;
} glrMeshType;
