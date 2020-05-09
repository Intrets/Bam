#pragma once

#pragma warning(push,0)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLI/gli.hpp>
#include <GLM/gtc/integer.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform.hpp>
#pragma warning(pop)

#define deleteDefaults(T) T(const T&) = delete; T(T&&) = delete; T& operator=(const T&) = delete; T& operator=(T&&) = delete;

#define MAX_STATIC_DRAW 10000 

#define CHUNKSIZE 32

#include "Locator.h"
#include "PathManager.h"
#include "DebugRenderInfo.h"

inline int floordiv(int x, int y) {
	return (x + (x < 0)) / y - (x < 0);
}

inline glm::ivec2 floordiv(glm::ivec2 v, int y) {
	return glm::ivec2(floordiv(v.x, y), floordiv(v.y, y));
};

inline std::pair<int, int> floordivmod(int x, int y) {
	int div = floordiv(x, y);
	int mod = x - div * y;
	return std::make_pair(div, mod);
}

inline std::pair<glm::ivec2, glm::ivec2> floordivmod(glm::ivec2 v, int y) {
	auto pair1 = floordivmod(v.x, y);
	auto pair2 = floordivmod(v.y, y);
	return std::make_pair(glm::ivec2(pair1.first, pair2.first), glm::ivec2(pair1.second, pair2.second));
}

template<class T>
inline bool indexInVector(size_t i, std::vector<T>& V) {
	return i >= 0 && i < V.size();
}

inline int idot(glm::ivec2 v1, glm::ivec2 v2) {
	return v1.x * v2.x + v1.y * v2.y;
}
