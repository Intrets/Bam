#include "common.h"
#include "DebugRenderer.h"

#include "ShaderLoader.h"

#include "GLEnableWrapper.h"
#include "RenderInfo.h"

void DebugRenderer::render(GLuint target, RenderInfo& renderInfo) {
	glBindFramebuffer(GL_FRAMEBUFFER, target);
	glViewport(0, 0, renderInfo.cameraInfo.x, renderInfo.cameraInfo.y);

	this->renderVector(renderInfo.debugRenderInfo.points, 0, renderInfo);
	for (auto& line : renderInfo.debugRenderInfo.lines) {
		this->renderVector(line, 1, renderInfo);
	}

	RenderInfo screenRenderInfo;
	screenRenderInfo.cameraInfo.VP = glm::mat4(1.0f);

	for (auto& line : renderInfo.debugRenderInfo.linesScreen) {
		this->renderVector(line, 1, screenRenderInfo);
	}
}

DebugRenderer::DebugRenderer() :
	program(Locator<PathManager>::get()->LoadShadersP("DebugLine.vert", "DebugLine.frag")),
	VP("VP", this->program) {

	this->VAO.gen(1);

	glGenBuffers(1, &this->data.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->data.ID);
	glVertexAttribPointer(
		0,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,				                  // stride
		(void*) 0                         // array buffer offset
	);

	this->VAO.unbind();
}


DebugRenderer::~DebugRenderer() {
}

void DebugRenderer::renderVector(std::vector<glm::vec2>& line, int32_t type, RenderInfo& renderInfo) {
	GLEnabler glEnabler;

	glEnabler.disable(GL_DEPTH_TEST).disable(GL_BLEND);

	if (line.size() == 0) {
		return;
	}

	int32_t drawtype = 0;
	// POINTS
	if (type == 0) {
		glEnabler.enable(GL_PROGRAM_POINT_SIZE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		drawtype = GL_POINTS;
	}
	// LINE STRIP
	if (type == 1) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawtype = GL_LINE_STRIP;
	}

	this->VAO.bind();
	this->program.use();

	this->VP.set(renderInfo.cameraInfo.VP);

	glBindBuffer(GL_ARRAY_BUFFER, this->data.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * line.size(), &(line[0]), GL_STATIC_DRAW);

	glDrawArrays(
		drawtype,	// mode
		0,				// start
		static_cast<GLsizei>(line.size())   // count
	);

	this->VAO.unbind();
}
