#pragma once

#include "Model.h"

class Cow
{
	vec3 pos;

	bool isCapchured;
	float anim1;
	float anim2;

	float jumpHeight;

public:
	Cow()
	{
		float yaw = (float)(rand() % 1000) / 1000.0 * 2.0 * PI;
		float pitch = (float)(rand() % 1000) / 1000.0 * PI;

		pos = vec3(sin(pitch) * sin(yaw), cos(pitch), sin(pitch) * cos(yaw));
		
		anim1 = 0.0f;
		anim2 = 0.0f;
		isCapchured = false;

		jumpHeight = 0.1;
	}

	void update()
	{
		if (!isCapchured)
			anim1 += 0.1;
		else
		{
			anim2 += 1.0 / 60 / 3;
			if (anim2 > 1.0)
				anim2 = 1.0;
		}
	}

	float checkCatch(vec4 planetRotation)
	{
		return dot(vec3(0.0, 1.0, 0.0), quatRotate(pos, planetRotation));
	}

	bool isFullyCatched()
	{
		return anim2 == 1.0;
	}

	float getAnim()
	{
		return anim2;
	}

	void Catch()
	{
		isCapchured = true;
	}

	vec3& getPos()
	{
		return pos;
	}

	void Draw(Model& model, Shader& shader, Camera& cam, float planetR, vec4 planetRotation, float flightHeight)
	{
		float rotation = PI / 2;

		vec3 finalPos;
		vec4 finalRot;

		vec3 pos1 = mult(pos, planetR + jumpHeight * abs(sin(anim1)));
		vec3 rotPos = quatRotate(pos1, planetRotation);

		rotPos = add(rotPos, vec3(-1.0, -1.0, -1.0));

		finalPos = rotPos;

		vec4 firstRotation = vec4(0, 0, sin(rotation / 2), cos(rotation / 2));
		vec4 animRotation = vec4(0.0, sin(anim1 / 8), 0.0, cos(anim1 / 8));
		vec4 secondRotation = quatFromVectors(vec3(0.0, 1.0, 0.0), pos);

		finalRot = quatMult(planetRotation, quatMult(secondRotation, quatMult(animRotation, firstRotation)));

		float animScale = 1.0;

		if (isCapchured)
		{
			vec3 cowPos1 = rotPos;
			vec3 cowPos2 = vec3(0.0, planetR + flightHeight * 0.9, 0.0);

			cowPos2 = add(cowPos2, vec3(-1.0));

			finalPos = mix(cowPos1, cowPos2, anim2);

			animScale = pow(1.0 - anim2, 0.1);
		}
		
		model.Draw(shader, cam, finalPos, finalRot, vec3(0.01 * animScale));
	}
};