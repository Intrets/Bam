#pragma once

#include <GL/glew.h>
//#include <GLFW/glfw3.h>

#include <string>

/// Filename can be KTX or DDS files
GLuint loadTexture(const std::string Filename);
