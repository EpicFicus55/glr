#pragma once
#include "cglm/cglm.h"

typedef struct 
	{
	/* Vectors */
	vec3	pos;
	vec3	front;
	vec3	up;
	vec3	right;

	/* Matrix to be used as View */
	mat4	lookAtMatrix;

	/* Camera parameters */
	float	cameraSpeed;
	float	lastX;
	float	lastY;
	char	firstMouse;

	/* Euler angles */
	float	yaw;
	float	pitch;
	} glrCameraType;

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
	);

/*
 * 'W' key functionality - move forward
 */
void glrCameraWCallback
	(
	glrCameraType* camera
	);

/*
 * 'A' key functionality - move left
 */
void glrCameraACallback
	(
	glrCameraType* camera
	);

/*
 * 'S' key functionality - move down
 */
void glrCameraSCallback
	(
	glrCameraType* camera
	);

/*
 * 'D' key functionality - move right
 */
void glrCameraDCallback
	(
	glrCameraType* camera
	);

/*
 * Mouse move functionality - follow cursor
 */
void glrCameraMouseCallback
	(
	glrCameraType* camera,
	double	xPosIn,
	double	yPosIn
	);
