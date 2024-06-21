#include "Camera.h"

Camera::Camera()
{
	this->pos = vec3(0.0);
	this->near = 0.01;
	this->far = 100.0;
	this->fov = 3.1415 / 2.0;
	this->aspectRatio = 1.0;

	yaw = 0.0;
	pitch = 0.0;

	updateDir();

	mouseClicked = false;
	anchored = false;
}

Camera::Camera(vec3 pos, float near, float far, float fov, float aspectRatio, GLFWwindow* window)
{
	this->pos = pos;
	this->near = near;
	this->far = far;
	this->fov = fov;
	this->aspectRatio = aspectRatio;

	yaw = 0.0;
	pitch = 0.0;

	updateDir();

	mouseClicked = true;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	anchored = false;
}

mat4 Camera::getMatrix(bool forSkybox)
{
	
	mat4 view = generateView(pos, dir, up, right);
	mat4 proj = generateProjection(near, far, fov, aspectRatio);

	if (forSkybox)
	{
		view.data[3] = 0;
		view.data[7] = 0;
		view.data[11] = 0;
		view.data[15] = 0;

		view.data[12] = 0;
		view.data[13] = 0;
		view.data[14] = 0;
	}
	return mult(proj, view);
}

void Camera::getInputs(GLFWwindow* window)
{
	int width, height;
	float pi = 3.141592653;
	float speed = 10.0 / 60.0;

	glfwGetWindowSize(window, &width, &height);

	if (!anchored)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			pos = add(pos, mult(dir, speed));
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			pos = add(pos, mult(dir, -speed));
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			pos = add(pos, mult(right, speed));
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			pos = add(pos, mult(right, -speed));
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			pos = add(pos, mult(up, speed));
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			pos = add(pos, mult(up, -speed));
		
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		if (mouseClicked)
		{
			mouseClicked = false;
			glfwSetCursorPos(window, width / 2, height / 2);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		mouseClicked = true;
	}

	if (mouseClicked)
	{
		double mouseX, mouseY;
		double sensetivity = 1.0;

		glfwGetCursorPos(window, &mouseX, &mouseY);

		double rotX = (mouseX - (double)width / 2.0) / (double)width * sensetivity;
		double rotY = (mouseY - (double)height / 2.0) / (double)width * sensetivity;

		yaw += rotX;
		pitch += rotY;

		if (pitch > pi / 2.0)
			pitch = pi / 2.0;
		if (pitch < -pi / 2.0)
			pitch = -pi / 2.0;

		updateDir();


		glfwSetCursorPos(window, width / 2, height / 2);
	}
}

void Camera::updateDir()
{
	if (!anchored)
	{
		dir = vec3{ cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw) };
		right = normalize(vec3{ dir.z, 0.0, -dir.x });
		up = mult(normalize(cross(dir, right)), -1);
	}
	else
	{
		dir = vec3{ cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw) };
		right = normalize(vec3{ dir.z, 0.0, -dir.x });
		up = mult(normalize(cross(dir, right)), -1);

		pos = add(mult(anchor, -1), mult(dir, -1.0 * anchorDist));
	}
}

void Camera::updateDir(vec3 dir)
{
	this->dir = mult(dir, -1);
	right = normalize(vec3{ dir.z, 0.0, -dir.x });
	up = mult(normalize(cross(dir, right)), -11);
}
void Camera::updateDir(vec3 dir, vec3 up)
{
	this->dir = dir;
	this->up = up;
	this->right = normalize(cross(dir, up));
}

void Camera::setAnchor(vec3 anchor, float anchorDist)
{
	this->anchor = anchor;
	anchored = true;
	this->anchorDist = anchorDist;
	this->minAnchorDist = anchorDist;
}

void Camera::setAnchorDist(double offset)
{
	float scrollDist = 1.2;

	if (offset < 0)
	{
		anchorDist *= scrollDist;
	}
	else if (offset > 0)
	{
		anchorDist /= scrollDist;
		if (anchorDist < minAnchorDist)
			anchorDist = minAnchorDist;
	}
}