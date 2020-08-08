#pragma once

#include "common.h"

#include <iostream>

#include <chrono>
#include <thread>
#include "ControlState.h"
#include "Main.h"
#include "InitManagers.h"
#include "InitModifyables.h"
#include <sstream>

#include "Log.h"

#define OPENGL_DEBUG true

GLFWwindow* window;

static void GLAPIENTRY
MessageCallback(GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar* message,
				const void* userParam) {
	static int32_t highCount = 0;
	static int32_t mediumCount = 0;
	static int32_t lowCount = 0;
	static int32_t notificationCount = 0;
	static int32_t otherCount = 0;
	std::string sourceMessage;
	switch (source) {
		case 0x8246:
			sourceMessage = "DEBUG_SOURCE_WINDOW_SYSTEM";
			break;
		case 0x8247:
			sourceMessage = "DEBUG_SOURCE_THIRD_PARTY";
			break;
		case 0x8248:
			sourceMessage = "DEBUG_SOURCE_SHADER_COMPILER";
			break;
		case 0x8249:
			sourceMessage = "DEBUG_SOURCE_THIRD_PARTY";
			break;
		case 0x824a:
			sourceMessage = "DEBUG_SOURCE_APPLICATION";
			break;
		case 0x824b:
			sourceMessage = "DEBUG_SOURCE_OTHER";
			break;
		default:
			{
				std::stringstream mes;
				mes << "0x" << std::hex << severity;
				sourceMessage = mes.str();
				break;
			}
	}


	std::string severityMessage;
	switch (severity) {
		case 0x9146:
			severityMessage = "DEBUG_SEVERITY_HIGH";
			highCount++;
			break;
		case 0x9147:
			severityMessage = "DEBUG_SEVERITY_MEDIUM";
			mediumCount++;
			break;
		case 0x9148:
			severityMessage = "DEBUG_SEVERITY_LOW";
			lowCount++;
			break;
		case 0x826B:
			severityMessage = "DEBUG_SEVERITY_NOTIFICATION";
			notificationCount++;
			break;
		default:
			{
				otherCount++;
				std::stringstream mes;
				mes << "0x" << std::hex << severity;
				severityMessage = mes.str();
				break;
			}
	}

	std::string typeMessage;
	switch (type) {
		case 0x824C:
			typeMessage = "DEBUG_TYPE_ERROR";
			break;
		case 0x824D:
			typeMessage = "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
			break;
		case 0x824E:
			typeMessage = "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
			break;
		case 0x824F:
			typeMessage = "DEBUG_TYPE_PORTABILITY";
			break;
		case 0x8250:
			typeMessage = "DEBUG_TYPE_PERFORMANCE";
			break;
		case 0x8251:
			typeMessage = "DEBUG_TYPE_OTHER";
			break;
		default:
			{
				std::stringstream mes;
				mes << "0x" << std::hex << type;
				severityMessage = mes.str();
				break;
			}
			break;
	}

	fprintf(stderr,
			"--- GL DEBUG CALLBACK --- %d %d %d %d\nsource: %s\ntype: %s\nseverity: %s\nid: %d\nmessage: %s\n\n",
			highCount, mediumCount, lowCount, notificationCount,
			sourceMessage.c_str(),
			typeMessage.c_str(),
			severityMessage.c_str(),
			id,
			message
	);
}


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
	window = glfwCreateWindow(1800, 1000, "Hello World", NULL, NULL);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetWindowPos(window, 80, 40);
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

	if (OPENGL_DEBUG) {
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
	}

	return 1;
}

int main() {
	initGLFW();
	initModifyables();
	initManagers(window);
	mainLoop(window);
}
