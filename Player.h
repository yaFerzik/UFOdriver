#pragma once
#include "mat4.h"
#include "vec.h"
#include "Model.h"


class Player
{
	vec3 pos;
	float rotation;

	Model ufo;

	Mesh tractorBeam;
	Shader tractorBeamShader;

	vec3 tractorBeamColor;

public:
	Player()
	{
		ufo = Model("Models\\ufo3\\scene.gltf");

		Texture dummyTex = Texture();
		tractorBeam = createCylinder(0.1, 0.25, 1.0, 3, 20, dummyTex);
		tractorBeamShader = Shader("default.vert", "tractorBeam.frag");
		tractorBeamColor = vec3(3.0 / 255.0, 252.0 / 255.0, 44.0 / 255.0);


		pos = vec3(1.0, 0.0, 0.0);
		rotation = 0;
	}

	void Draw(Shader& shader, Camera& cam)
	{
		vec3 axis = normalize(pos);
		vec4 rotationQuat = vec4(axis.x * sin(rotation / 2),
								axis.y * sin(rotation / 2),
								axis.z * sin(rotation / 2),
								cos(rotation / 2));
		vec4 firstQuat = vec4(0, 0, sin(3.1415926 / 2), cos(3.1415926 / 2));

		ufo.Draw(shader, cam, add(pos, vec3(-1.0, -1, -1.00)), rotationQuat, vec3(0.005));
	}

	void DrawTractorBeam(Camera& cam, FrameBuffer& framebuffer, float flightHeight, float anim)
	{
		tractorBeamShader.Activate();

		glUniform3f(glGetUniformLocation(tractorBeamShader.id, "beamColor"), tractorBeamColor.x, tractorBeamColor.y, tractorBeamColor.z);
		glUniform3f(glGetUniformLocation(tractorBeamShader.id, "camPos"), cam.getPos().x, cam.getPos().y, cam.getPos().z);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, framebuffer.framebufferDepthTexture);
		glUniform1i(glGetUniformLocation(tractorBeamShader.id, "depthTex"), 0);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		float openingLen = 0.1;
		float k1 = anim / openingLen;
		float k2 = (anim - 1.0) / -openingLen;

		float beamWidth = k1;
		if (k2 < k1)
			beamWidth = k2;
		if (beamWidth > 1.0)
			beamWidth = 1.0;

		tractorBeam.Draw(tractorBeamShader, cam, mat4(), add(pos, vec3(0.0, -flightHeight / 2, 0.0)), vec4(0.0, 0.0, 0.0, 1.0), vec3(beamWidth, flightHeight, beamWidth));
		glDisable(GL_BLEND);
	}

	vec3& getPos()
	{
		return pos;
	}

	float getRotation()
	{
		return rotation;
	}

	void update()
	{
		rotation += 2.0 * PI / 60.0 / 3;
	}
};