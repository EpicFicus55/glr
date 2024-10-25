#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "glr_camera.h"

/*
 * Update the lookAt matrix based on the camera
 * vectors.
 */
static void updateCameraLookAtMatrix
	(
	glrCameraType* camera
	);

/*
 * Initializes a camera based on the position,
 * target, and up vector.
 */
void glrInitCamera
	(
	glrCameraType*  camera,
	vec3			pos,
	vec3			front,
	vec3			up,
	float			cameraSpeed
	)
{
vec3 cameraTarget;

assert(camera != NULL);

/* Initialize the parameters */
camera->yaw = -90.0f;
camera->pitch = 0.0f;
camera->lastX = 0.0f;
camera->lastY = 0.0f;
camera->firstMouse = 1;
camera->cameraSpeed = cameraSpeed;

memcpy(camera->pos, pos, sizeof(camera->pos));
memcpy(camera->front, front, sizeof(camera->front));
memcpy(camera->up, up, sizeof(camera->up));

updateCameraLookAtMatrix(camera);

}

/*
 * 'W' key functionality - move forward
 */
void glrCameraWCallback
	(
	glrCameraType* camera
	)
{
vec3 aux;
glm_vec3_scale(camera->front, camera->cameraSpeed, aux);
glm_vec3_add(camera->pos, aux, camera->pos);
updateCameraLookAtMatrix(camera);
}

/*
 * 'A' key functionality - move left
 */
void glrCameraACallback
	(
	glrCameraType* camera
	)
{
vec3 aux;
glm_vec3_cross(camera->front, camera->up, aux);
glm_vec3_scale(aux, camera->cameraSpeed, aux);
glm_vec3_sub(camera->pos, aux, camera->pos);
updateCameraLookAtMatrix(camera);
}

/*
 * 'S' key functionality - move down
 */
void glrCameraSCallback
	(
	glrCameraType* camera
	)
{
vec3 aux;
glm_vec3_scale(camera->front, -camera->cameraSpeed, aux);
glm_vec3_add(camera->pos, aux, camera->pos);
updateCameraLookAtMatrix(camera);
}

/*
 * 'D' key functionality - move right
 */
void glrCameraDCallback
	(
	glrCameraType* camera
	)
{
vec3 aux;
glm_vec3_cross(camera->front, camera->up, aux);
glm_vec3_scale(aux, camera->cameraSpeed, aux);
glm_vec3_add(camera->pos, aux, camera->pos);
updateCameraLookAtMatrix(camera);
}

/*
 * Mouse move functionality - follow cursor
 */
void glrCameraMouseCallback
	(
	glrCameraType* camera,
	double	xPosIn,
	double	yPosIn
	)
{
float xPos = (float)xPosIn;
float yPos = (float)yPosIn;

/* For a smoother transition */
if(camera->firstMouse)
	{
	camera->lastX = xPos;
	camera->lastY = yPos;
	camera->firstMouse = 0;
	}

float xOffset = xPos - camera->lastX;
float yOffset = camera->lastY - yPos;

camera->lastX = xPos;
camera->lastY = yPos;

xOffset *= 0.35f;
yOffset *= 0.35f;

/* Update Euler angles based on the mouse pointer evolution */
camera->yaw		+= xOffset;
camera->pitch	+= yOffset;

/* Limit pitch in order to avoid image flips */
if(camera->pitch >  89.0f) { camera->pitch =  89.0f; }
if(camera->pitch < -89.0f) { camera->pitch = -89.0f; }

updateCameraLookAtMatrix(camera);

}

/*
 * Update the lookAt matrix based on the camera
 * vectors.
 */
static void updateCameraLookAtMatrix
	(
	glrCameraType* camera
	)
{
vec3 aux;
vec3 front;

front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
front[1] = sin(glm_rad(camera->pitch));
front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));

glm_vec3_normalize(front);
memcpy(camera->front, front, sizeof(camera->front));

glm_vec3_add(camera->pos, camera->front, aux);
glm_lookat(camera->pos, aux, camera->up, camera->lookAtMatrix);
}
