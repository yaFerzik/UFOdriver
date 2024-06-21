#pragma once

#include "Model.h"
#include "ModelUtils.h"
#include "Skybox.h"
#include "FrameBuffer.h"
#include "Player.h"
#include "Atmosphere.h"
#include "Cow.h"
#include "Minimap.h"
#include <vector>
#include "Radar.h"
#include "DetectionBar.h"

class Game
{
	float far = 100.0;
	float near = 0.01;
	float fov = 3.1415 / 2;

	float cameraDistance;
	float time;

	Camera mainCamera;
	Player player;

	vec3 lightDir;

	Texture earthTexture;
	Mesh planetMesh;
	float planetR;
	float flightHeight;

	Atmosphere atmosphere;

	Skybox skybox1;

	vec4 planetRotation;

	Mesh dummyMesh;

	Minimap minimap;
	DetectionBar detectionBar;

	std::vector<Cow> cows;
	Model cowModel;
	int capchuringCowNum;

	std::vector<Radar> radars;
	Model radarModel;

	float timeInRadar;
	float detectionRate;

	int width;
	int height;
	FrameBuffer mainFrameBuffer;

	Shader sh1 = Shader("default.vert", "default.frag");
	Shader skyboxShader = Shader("skybox.vert", "skybox.frag");
	Shader frameBufferShader = Shader("frameBuffer.vert", "frameBuffer.frag");

public:

	Game() {}

	Game(GLFWwindow* win)
	{
		glfwGetWindowSize(win, &width, &height);

		mainFrameBuffer = FrameBuffer(1, 2, width, height);

		planetR = 10.0;
		flightHeight = 1.0;

		skybox1 = Skybox("SkyBoxes\\spaceSkyBox2\\", 0);

		earthTexture = Texture("C:\\Users\\User\\Desktop\\2k_earth_daymap.jpg", "diffuse", 10);
		planetMesh = createUVSphereMesh(planetR, 50, 50, earthTexture);

		minimap = Minimap(earthTexture);

		planetRotation = vec4(0.0, 0.0, 0.0, 1.0);

		dummyMesh = createUVSphereMesh(0.1, 10, 10, earthTexture);

		player = Player();
		player.getPos() = vec3(0, planetR + flightHeight, 0);


		mainCamera = Camera(vec3(0.0), near, far, fov, (float)(width) / (float)(height), win);
		cameraDistance = 0.3;

		mainCamera.setAnchor(player.getPos(), 0.5);

		lightDir = normalize(vec3(1.0, 1.0, 0.0));

		cows.clear();
		for (int i = 0; i < 10; i++)
			cows.push_back(Cow());

		cowModel = Model("Models\\cow1\\scene.gltf");
		capchuringCowNum = -1;

		timeInRadar = 0.0;
		detectionRate = 0.3 / 60;

		radars.clear();
		for (int i = 0; i < 4; i++)
			radars.push_back(Radar());

		float timeInRadar;
		float detectionRate;

		time = 0.0;
		
		radarModel = Model("Models\\radar2\\scene.gltf");
	}

	~Game()
	{
		sh1.Destroy();
		skyboxShader.Destroy();
		frameBufferShader.Destroy();
	}

	void getInputs(GLFWwindow* win, vec3 dir, vec3 right)
	{
		float rotationSpeed = 2.0 * PI / 60 / 20;

		if (capchuringCowNum >= 0)
			return;

		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		{
			vec4 newRotation = vec4(right.x * sin(-rotationSpeed / 2),
									right.y * sin(-rotationSpeed / 2),
									right.z * sin(-rotationSpeed / 2),
									cos(-rotationSpeed / 2));
			planetRotation = quatMult(newRotation, planetRotation);
		}
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		{
			vec4 newRotation = vec4(right.x * sin(rotationSpeed / 2),
				right.y * sin(rotationSpeed / 2),
				right.z * sin(rotationSpeed / 2),
				cos(rotationSpeed / 2));
			planetRotation = quatMult(newRotation, planetRotation);
		}
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		{
			vec4 newRotation = vec4(dir.x * sin(rotationSpeed / 2),
				dir.y * sin(rotationSpeed / 2),
				dir.z * sin(rotationSpeed / 2),
				cos(rotationSpeed / 2));
			planetRotation = quatMult(newRotation, planetRotation);
		}
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		{
			vec4 newRotation = vec4(dir.x * sin(-rotationSpeed / 2),
				dir.y * sin(-rotationSpeed / 2),
				dir.z * sin(-rotationSpeed / 2),
				cos(-rotationSpeed / 2));
			planetRotation = quatMult(newRotation, planetRotation);
		}
	}

	void checkRadarDetection()
	{
		bool inRadar = false;

		for (int i = 0; i < radars.size(); i++)
			if (radars[i].checkCollision(planetRotation, planetR, flightHeight))
			{
				timeInRadar += detectionRate;
				inRadar = true;
			}

		if (!inRadar)
		{
			timeInRadar -= detectionRate;
			if (timeInRadar < 0.0)
				timeInRadar = 0.0;
		}


		if (timeInRadar >= 1.0)
		{
			timeInRadar = 1.0;
		}
	}

	void update(GLFWwindow* win)
	{
		for (int i = 0; i < cows.size(); i++)
			cows[i].update();

		for (int i = 0; i < radars.size(); i++)
			radars[i].update();

		player.update();

		mainCamera.getInputs(win);

		vec3 dir = mult(mainCamera.dir, -1.0);
		dir = normalize(vec3(dir.x, 0.0, dir.z));

		vec3 right = cross(normalize(player.getPos()), dir);

		getInputs(win, dir, right);

		
		for (int i = 0; i < cows.size() && capchuringCowNum == -1; i++)
		{
			if (!cows[i].isFullyCatched() && cows[i].checkCatch(planetRotation) >= 0.99999)
			{
				capchuringCowNum = i;
				cows[i].Catch();
			}
		}

		if (capchuringCowNum >= 0 && cows[capchuringCowNum].isFullyCatched())
			capchuringCowNum = -1;

		time += 1.0 / 60.0;
		
		checkRadarDetection();

		detectionBar.update();
	}

	bool playerDead()
	{
		return timeInRadar == 1.0;
	}

	bool allCowsCatched()
	{
		bool all = true;

		for (int i = 0; i < cows.size() && all; i++)
			if (!cows[i].isFullyCatched())
				all = false;

		return all;
	}

	float getTime()
	{
		return time;
	}

	void Draw(GLFWwindow* win)
	{
		
		vec3 rotatedLightDir = quatRotate(lightDir, planetRotation);
		mat4 rotationMatrix = generateRotation(planetRotation);

		sh1.Activate();
		GLuint scUni = glGetUniformLocation(sh1.id, "lightDir");
		glUniform3f(scUni, rotatedLightDir.x, rotatedLightDir.y, rotatedLightDir.z);

		skyboxShader.Activate();
		glUniform3f(glGetUniformLocation(skyboxShader.id, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.id, "rotMat"), 1, GL_FALSE, rotationMatrix.data);

		mainFrameBuffer.Activate();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		for (int i = 0; i < cows.size(); i++)
			cows[i].Draw(cowModel, sh1, mainCamera, planetR, planetRotation, flightHeight);
		
		//for (int i = 0; i < radars.size(); i++)
		//	radars[i].Draw(radarModel, sh1, mainCamera, planetR, planetRotation);
		
		planetMesh.Draw(sh1, mainCamera, mat4(), vec3(0, 0, 0), planetRotation);
		player.Draw(sh1, mainCamera);

		skybox1.Draw(skyboxShader, mainCamera);

		mainFrameBuffer.Deactivate();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mainFrameBuffer.Draw(frameBufferShader);

		

		atmosphere.sendUniforms(mainCamera, mainFrameBuffer, rotatedLightDir, vec3(0.0), planetR);
		atmosphere.Draw(mainCamera);

		
		for (int i = 0; i < radars.size(); i++)
			radars[i].DrawShpere(mainCamera, mainFrameBuffer, planetR, planetRotation);
		
		if (capchuringCowNum >= 0)
			player.DrawTractorBeam(mainCamera, mainFrameBuffer, flightHeight, cows[capchuringCowNum].getAnim());
		
		
		minimap.Draw(planetRotation, cows, radars, win, planetR, flightHeight);

		detectionBar.Draw(win, timeInRadar);
		
		
	}

	void HandleMouseScroll(double offset)
	{
		mainCamera.setAnchorDist(offset);
	}
};