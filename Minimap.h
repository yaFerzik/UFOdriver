#pragma once

#include "Model.h"
#include "Cow.h"
#include "Player.h"
#include "Radar.h"

class Minimap
{
	unsigned int vao;

	Shader mapShader;
	Shader objectShader;
	Shader radarShader;

	float width;
	float height;

	Texture mapTexture;
	Texture ufoTexture;
	Texture cowTexture;

public:
	Minimap() {};

	Minimap(Texture& mapTexture)
	{
		mapShader = Shader("frameBuffer.vert", "Minimap.frag");
		objectShader = Shader("frameBuffer.vert", "mapObject.frag");
		radarShader = Shader("frameBuffer.vert", "mapRadar.frag");

		this->mapTexture = mapTexture;
		ufoTexture = Texture("Textures\\ufo.png", "diffuse", 13);
		cowTexture = Texture("Textures\\cow.png", "diffuse", 13);

		width = 0.35;
		height = 0.33;

		float x1 = 1.0 - width * 2.0;
		float x2 = 1.0;
		float y1 = -1.0;
		float y2 = -1.0 + height * 2.0;
		
		float rectangleVertices[] =
		{
			// Coords    // texCoords
			 x2,  y1,  1.0f, 0.0f,
			 x1,  y2,  0.0f, 1.0f,
			 x1,  y1,  0.0f, 0.0f,

			 x2,  y2,  1.0f, 1.0f,
			 x1,  y2,  0.0f, 1.0f,
			 x2,  y1,  1.0f, 0.0f
		};

		unsigned int rectVBO;
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &rectVBO);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}

	void DrawMap()
	{
		mapShader.Activate();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mapTexture.id);
		glUniform1i(glGetUniformLocation(mapShader.id, "mapTexture"), 0);

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void DrawObject(vec3 objPos, Texture& texture, float aspect)
	{
		float objSize = 0.1;

		objectShader.Activate();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glUniform1i(glGetUniformLocation(texture.id, "objectTexture"), 0);

		glUniform1f(glGetUniformLocation(objectShader.id, "aspect"), aspect);
		glUniform3f(glGetUniformLocation(objectShader.id, "objPos"), objPos.x, objPos.y, objPos.z);
		glUniform1f(glGetUniformLocation(objectShader.id, "objSize"), 0.1);
		

		glBindVertexArray(vao);

		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisable(GL_BLEND);
	}

	void DrawRadar(Radar radar, float planetR, float flightHeight)
	{
		vec3 radarPos = mult(radar.getPos(), planetR - (radar.getR() - radar.getHeight()));
		//vec3 radarPos = mult(radar.getPos(), planetR);

		radarShader.Activate();

		glUniform3f(glGetUniformLocation(radarShader.id, "radarPos"), radarPos.x, radarPos.y, radarPos.z);
		glUniform3f(glGetUniformLocation(radarShader.id, "radarColor"), radar.getColor().x, radar.getColor().y, radar.getColor().z);

		glUniform1f(glGetUniformLocation(radarShader.id, "radarR"), radar.getR());
		glUniform1f(glGetUniformLocation(radarShader.id, "planetR"), planetR);
		glUniform1f(glGetUniformLocation(radarShader.id, "flightHeight"), flightHeight);

		glBindVertexArray(vao);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisable(GL_BLEND);
	}

	void Draw(vec4 planetRotation, std::vector<Cow> cows, std::vector<Radar> radars, GLFWwindow* win, float planetR, float flightHeight)
	{
		int realWidth;
		int realHeight;

		glfwGetWindowSize(win, &realWidth, &realHeight);

		realWidth *= width;
		realHeight *= height;

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		//glBindTexture(GL_TEXTURE_2D, framebufferTexture);
		
		DrawMap();
		DrawObject(quatRotate(vec3(0.0, 1.0, 0.0), inverse(planetRotation)), ufoTexture, (float)realWidth / (float)realHeight);

		for (int i = 0; i < cows.size(); i++)
		{
			if (!cows[i].isFullyCatched())
				DrawObject(cows[i].getPos(), cowTexture, (float)realWidth / (float)realHeight);
		}

		for (int i = 0; i < radars.size(); i++)
			DrawRadar(radars[i], planetR, flightHeight);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glBindVertexArray(0);
	}
};