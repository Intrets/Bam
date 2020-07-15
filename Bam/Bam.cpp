#pragma once

#include "common.h"

#include <iostream>

#include <chrono>
#include <thread>
#include "ControlState.h"
#include "Main.h"
#include "InitManagers.h"
#include "InitModifyables.h"

GLFWwindow* window;

static int initGLFW() {
	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
														 /* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1000, 1000, "Hello World", NULL, NULL);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetWindowPos(window, 500, 40);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	printf("OpenGL version supported by this platform (%s): \n",
		   glGetString(GL_VERSION));

	// constrain cursor to window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// back culling
	//glDisable(GL_CULL_FACE);

	glfwSwapInterval(0);

	return 1;
}

int main() {
	initGLFW();
	initModifyables();
	initManagers(window);
	mainLoop(window);
}
