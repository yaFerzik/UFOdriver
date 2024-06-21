#pragma once

#include "Model.h"

class DetectionBar
{
	unsigned int vao;

	Shader barShader;

	float margin;
	float height;

	vec3 bgColor;
	vec3 barColor;

	float anim;

public:

	DetectionBar()
	{
		barShader = Shader("frameBuffer.vert", "detectionBar.frag");

		margin = 0.05;
		height = 0.07;

		bgColor = vec3(41.0 / 255.0, 45.0 / 255.0, 51.0 / 255.0);
		barColor = vec3(0.0, 1.0, 0.0);

		anim = 0.0;

		float x1 = -1.0 + margin;
		float x2 = 1.0 - margin;
		float y1 = 1.0 - margin;
		float y2 = 1.0 - margin - height;

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

	void update()
	{
		anim += 0.1;
	}

	void Draw(GLFWwindow* win, float detection)
	{
		int realWidth;
		int realHeight;

		glfwGetWindowSize(win, &realWidth, &realHeight);

		realWidth *= 2.0 - margin * 2.0;
		realHeight *= height;

		barShader.Activate();

		glUniform3f(glGetUniformLocation(barShader.id, "bgColor"), bgColor.x, bgColor.y, bgColor.z);
		glUniform3f(glGetUniformLocation(barShader.id, "barColor"), barColor.x, barColor.y, barColor.z);
		glUniform1f(glGetUniformLocation(barShader.id, "detection"), detection);
		glUniform1f(glGetUniformLocation(barShader.id, "anim"), anim);
		glUniform1f(glGetUniformLocation(barShader.id, "aspect"), (float)realWidth / (float)realHeight);

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		glBindVertexArray(0);
	}
};