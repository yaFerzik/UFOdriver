#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "mat4.h"
#include <GLFW/glfw3.h>

class Camera
{
	vec3 pos;

	vec3 anchor;
	bool anchored;
	float anchorDist;
	float minAnchorDist;
	
	float pitch;
	float yaw;

	

	float near;
	float far;
	float fov;
	float aspectRatio;

	bool mouseClicked;

	void updateDir();

public:
	vec3 dir;
	vec3 up;
	vec3 right;

	Camera();
	Camera(vec3 pos, float near, float far, float fov, float aspectRatio, GLFWwindow* window);

	mat4 getMatrix(bool forSkybox = false);

	void getInputs(GLFWwindow* window);

	vec3& getPos() { return pos; }

	void updateDir(vec3 dir);
	void updateDir(vec3 dir, vec3 up);

	void setAnchor(vec3 anchor, float anchorDist);

	void setAnchorDist(double offset);
};

#endif

