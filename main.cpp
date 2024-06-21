#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Model.h"
#include "ModelUtils.h"
#include "Skybox.h"
#include "FrameBuffer.h"

#include "Game.h"

double mouseScrollInput;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mouseScrollInput = yoffset;
}



int main()
{
	//-------------------------Window initialisation--------------------------------
	const int width = 1600;
	const int height = 1000;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(width, height, "3D Renderer", NULL, NULL);
	if (win == NULL)
	{
		std::cout << "Error while creating a window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win);

	gladLoadGL();

	glViewport(0, 0, width, height);

	glfwSetScrollCallback(win, scroll_callback);
	//-----------------------------------------------------------------------------

	//Texture atmosphereTexture = Texture();
	//Mesh atmosphereMesh = createUVSphereMesh(atmR, 500, 500, atmosphereTexture);
	Game mainGame(win);


	//-----------------------------------------------------------------------------



	float t = 0;

	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	while (!glfwWindowShouldClose(win))
	{
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		if (mouseScrollInput != 0)
		{
			mainGame.HandleMouseScroll(mouseScrollInput);
			mouseScrollInput = 0;
		}

		mainGame.update(win);
		mainGame.Draw(win);

		if (mainGame.playerDead())
		{
			std::cout << "U dead :(\n";
			break;
		}

		if (mainGame.allCowsCatched())
		{
			std::cout << "You won! Your time is: " << mainGame.getTime() << "seconds\n";
			break;
		}

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwDestroyWindow(win);
	glfwTerminate();
	return 0;
}