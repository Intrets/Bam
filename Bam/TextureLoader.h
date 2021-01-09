#pragma once

#include <GL/glew.h>

#include "BufferWrappers.h"

#include <string>

/// Filename can be KTX or DDS files
GLuint loadTexture(const std::string Filename);
bwo::Texture2D load2DTexture(std::string const Filename);
