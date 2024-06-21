#pragma once

#include "Model.h"
#include "ModelUtils.h"
#include "FrameBuffer.h"

class Atmosphere
{

	Shader atmosphereShader;
	Mesh atmosphereMesh;

	float atmosphereR;
	float desityFalloff;
	int scatteringPointsNum;
	int opticalDepthPointsNum;
	vec3 scatteringCoefficients;
	float scatteringPower;

public:

	Atmosphere()
	{
		atmosphereShader = Shader("default.vert", "atmosphere.frag");

		atmosphereR = 12.0;
		desityFalloff = 8.0;
		scatteringPointsNum = 10;
		opticalDepthPointsNum = 10;
		scatteringPower = 2.0;
		scatteringCoefficients = vec3(
			pow(400.0 / 700.0, 4) * scatteringPower,
			pow(400.0 / 530.0, 4) * scatteringPower,
			pow(400.0 / 440.0, 4) * scatteringPower
		);

		Texture atmosphereTexture = Texture("C:\\Users\\User\\Desktop\\2k_earth_daymap.jpg", "diffuse", 10);
		atmosphereMesh = createUVSphereMesh(atmosphereR, 100, 100, atmosphereTexture);

		atmosphereShader.Activate();
	}

	~Atmosphere()
	{
		atmosphereShader.Destroy();
	}

	void sendUniforms(Camera cam, FrameBuffer framebuffer, vec3 lightDir, vec3 planetPos, float planetR)
	{
		atmosphereShader.Activate();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, framebuffer.framebufferTexture);
		glUniform1i(glGetUniformLocation(atmosphereShader.id, "mainTex"), 0);

		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, framebuffer.framebufferDepthTexture);
		glUniform1i(glGetUniformLocation(atmosphereShader.id, "depthTex"), 9);

		glUniform3f(glGetUniformLocation(atmosphereShader.id, "u_cameraPos"), cam.getPos().x, cam.getPos().y, cam.getPos().z);
		glUniform3f(glGetUniformLocation(atmosphereShader.id, "u_planetPos"), planetPos.x, planetPos.y, planetPos.z);
		glUniform1f(glGetUniformLocation(atmosphereShader.id, "u_planetR"), planetR);
		glUniform1f(glGetUniformLocation(atmosphereShader.id, "u_atmR"), atmosphereR);
		glUniform3f(glGetUniformLocation(atmosphereShader.id, "u_lightDir"), lightDir.x, lightDir.y, lightDir.z);

		glUniform1f(glGetUniformLocation(atmosphereShader.id, "u_density_falloff"), desityFalloff);
		glUniform1f(glGetUniformLocation(atmosphereShader.id, "u_scatteringPointsNum"), scatteringPointsNum);
		glUniform1f(glGetUniformLocation(atmosphereShader.id, "u_opticalDepthPointsNum"), opticalDepthPointsNum);
		glUniform3f(glGetUniformLocation(atmosphereShader.id, "u_sactteringCoefficients"), scatteringCoefficients.x, scatteringCoefficients.y, scatteringCoefficients.z);

		
	}

	void Draw(Camera cam)
	{
		

		glFrontFace(GL_CCW);
		atmosphereMesh.Draw(atmosphereShader, cam);

		glFrontFace(GL_CW);
	}
};