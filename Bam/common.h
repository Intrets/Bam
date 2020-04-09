#pragma once


#pragma warning(push,3)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLI/gli.hpp>
#include <GLM/gtc/integer.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform.hpp>
#pragma warning(pop)

#define deleteDefaults(T) T(const T&) = delete; T(T&&) = delete; T& operator=(const T&) = delete; T& operator=(T&&) = delete;

#define MAX_STATIC_DRAW 1024

#include "Locator.h"
#include "PathManager.h"
