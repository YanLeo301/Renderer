#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "shader.h"
#include "mesh.h"
#include "skybox.h"

int width = 1920;
int height = 1080;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(width, height, "Hello Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	const int grassBladeSegments = 10;
	float grassBladeBaseWidth = 0.02f;
	int grassBladeInstancesPerSide = 128;
	float grassPatchSize = 16.0f;

	std::vector<float> grassBladeVertices;
	std::vector<unsigned int> grassBladeIndices;

	for (int i = 0; i <= grassBladeSegments; i++)
	{
		float t = static_cast<float>(i) / grassBladeSegments;
		float segmentWidth = glm::mix(grassBladeBaseWidth, grassBladeBaseWidth * 0.4f, t);

		grassBladeVertices.push_back(-segmentWidth);
		grassBladeVertices.push_back(t);
		grassBladeVertices.push_back(0.0f);

		grassBladeVertices.push_back(segmentWidth);
		grassBladeVertices.push_back(t);
		grassBladeVertices.push_back(0.0f);
	}

	for (unsigned int i = 0; i < (grassBladeSegments * 2); i = i + 2)
	{
		grassBladeIndices.push_back(i);
		grassBladeIndices.push_back(i + 1);
		grassBladeIndices.push_back(i + 2);
		grassBladeIndices.push_back(i + 2);
		grassBladeIndices.push_back(i + 1);
		grassBladeIndices.push_back(i + 3);
	}
	Mesh grassBladeMesh(grassBladeVertices, grassBladeIndices, Position);
	Shader grassShader("res/shaders/grass.shader");
	glm::mat4 grassBladeModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

	Skybox skybox({
		"res/textures/skybox/right.png",
		"res/textures/skybox/left.png",
		"res/textures/skybox/top.png",
		"res/textures/skybox/bottom.png",
		"res/textures/skybox/front.png",
		"res/textures/skybox/back.png"
		});
	Shader skyboxShader("res/shaders/skybox.shader");
	glm::mat4 skyboxView;

	glm::mat4 view;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

	glm::vec3 cameraPosition = {0.0f, 3.0f, 5.0f};
	glm::vec3 cameraDirection = {0.0f, 0.0f, -1.0f};
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::vec3 right;
	float cameraSpeed = 0.1f;

	float mouseSensitivity = 0.1f;
	double currentMouseX;
	double currentMouseY;
	double lastMouseX = 1920 * 0.5;
	double lastMouseY = 1080 * 0.5;
	float offsetMouseX = 0.0f;
	float offsetMouseY = 0.0f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	glm::vec3 mouseDirection;

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, 1);
		}

		glfwGetFramebufferSize(window, &width, &height);
		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

		glfwGetCursorPos(window, &currentMouseX, &currentMouseY);

		offsetMouseX = static_cast<float>(currentMouseX - lastMouseX) * mouseSensitivity;
		offsetMouseY = static_cast<float>(lastMouseY - currentMouseY) * mouseSensitivity;
		lastMouseX = currentMouseX;
		lastMouseY = currentMouseY;
		yaw += offsetMouseX;
		pitch += offsetMouseY;

		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
		
		mouseDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		mouseDirection.y = sin(glm::radians(pitch));
		mouseDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		cameraDirection = glm::normalize(mouseDirection);

		right = glm::normalize(glm::cross(cameraDirection, up));

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cameraPosition += cameraDirection * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cameraPosition -= cameraDirection * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cameraPosition += right * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cameraPosition -= right * cameraSpeed;
		}

		view = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, up);

		skyboxView = glm::mat4(glm::mat3(view));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		grassShader.Use();
		grassShader.SetMat4("view", view);
		grassShader.SetMat4("projection", projection);
		grassShader.SetFloat("time", static_cast<float>(glfwGetTime()));
		grassShader.SetInt("instancesPerSide", grassBladeInstancesPerSide);
		grassShader.SetFloat("patchSize", grassPatchSize);
		grassShader.SetVec3("cameraPosition", cameraPosition);
		grassBladeMesh.DrawInstanced(grassBladeInstancesPerSide * grassBladeInstancesPerSide);

		skyboxShader.Use();
		skyboxShader.SetMat4("view", skyboxView);
		skyboxShader.SetMat4("projection", projection);
		skybox.Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}