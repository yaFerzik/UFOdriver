#pragma once

#include "Model.h"


class Radar
{
	vec3 pos;

	Mesh sphereMesh;
	Shader sphereShader;
	float sphereR;

	float radarHeight;

	vec3 sphereColor;

	float anim1;

public:
	Radar()
	{

		float yaw = (float)(rand() % 1000) / 1000.0 * 2.0 * PI;
		float pitch = (float)(rand() % 1000) / 1000.0 * PI;

		pos = vec3(sin(pitch) * sin(yaw), cos(pitch), sin(pitch) * cos(yaw));

		sphereR = 5.5;
		Texture dummy = Texture();
		sphereMesh = createUVSphereMesh(1, 50, 50, dummy);
		sphereShader = Shader("default.vert", "radar.frag");

		sphereColor = vec3(38.0 / 255.0, 87.0 / 255.0, 222.0 / 255.0);

		radarHeight = 2.0;

		anim1 = 0.0f;
	}

	vec3& getPos()
	{
		return pos;
	}

	float& getHeight()
	{
		return radarHeight;
	}

	float& getR()
	{
		return sphereR;
	}

	vec3& getColor()
	{
		return sphereColor;
	}

	void update()
	{
		anim1 += 0.1;
	}

	bool checkCollision(vec4 planetRotation, float planetR, float flightHeight)
	{
		vec3 pos1 = mult(pos, planetR - sphereR + radarHeight);
		vec3 rotPos = quatRotate(pos1, planetRotation);

		vec3 playerPos = vec3(0.0, planetR + flightHeight, 0.0);

		return length(add(playerPos, mult(rotPos, -1))) < sphereR;
	}

	void Draw(Model& model, Shader& shader, Camera& cam, float planetR, vec4 planetRotation)
	{
		float rotation = 0;

		vec3 finalPos;
		vec4 finalRot;

		vec3 pos1 = mult(pos, planetR);
		vec3 rotPos = quatRotate(pos1, planetRotation);

		rotPos = add(rotPos, vec3(-1.0, -1.0, -1.0));

		if (rotPos.y < 0)
			return;

		finalPos = rotPos;

		vec4 firstRotation = vec4(0, 0, sin(rotation / 2), cos(rotation / 2));
		vec4 secondRotation = quatFromVectors(vec3(0.0, 1.0, 0.0), pos);

		finalRot = quatMult(planetRotation, quatMult(secondRotation, firstRotation));

		model.Draw(shader, cam, finalPos, finalRot, vec3(0.005));
	}

	void DrawShpere(Camera& cam, FrameBuffer& framebuffer, float planetR, vec4 planetRotation)
	{
		sphereShader.Activate();

		glUniform3f(glGetUniformLocation(sphereShader.id, "sphereColor"), sphereColor.x, sphereColor.y, sphereColor.z);
		glUniform3f(glGetUniformLocation(sphereShader.id, "camPos"), cam.getPos().x, cam.getPos().y, cam.getPos().z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, framebuffer.framebufferDepthTexture);
		glUniform1i(glGetUniformLocation(sphereShader.id, "depthTex"), 0);

		glUniform1f(glGetUniformLocation(sphereShader.id, "anim"), anim1);


		vec3 finalPos;
		vec4 finalRot;

		vec3 pos1 = mult(pos, planetR - sphereR + radarHeight);

		vec3 rotPos = quatRotate(pos1, planetRotation);

		if (rotPos.y < 0)
			return;

		finalPos = rotPos;
		
		vec4 secondRotation = quatFromVectors(vec3(0.0, 1.0, 0.0), pos);

		finalRot = quatMult(planetRotation, secondRotation);


		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		sphereMesh.Draw(sphereShader, cam, mat4(), finalPos, finalRot, vec3(sphereR));

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}
};