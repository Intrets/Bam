#include "common.h"
#include "DebugRenderer.h"

#include "ShaderLoader.h"

#include "GLEnableWrapper.h"
#include "DebugRenderInfo.h"
#include "RenderInfo.h"

void DebugRenderer::render(GLuint target, RenderInfo& renderInfo) {
	glBindFramebuffer(GL_FRAMEBUFFER, target);
	glViewport(0, 0, renderInfo.cameraInfo.x, renderInfo.cameraInfo.y);

	renderVector(renderInfo.debugRenderInfo.points, 0, renderInfo);
	for (auto& line : renderInfo.debugRenderInfo.lines) {
		renderVector(line, 1, renderInfo);
	}
}

DebugRenderer::DebugRenderer() :
	program(Locator<PathManager>::getService()->LoadShadersP("DebugLine.vert", "DebugLine.frag")),
	VP("VP", program) {

	VAO.gen(1);

	glGenBuffers(1, &data.ID);
	glBindBuffer(GL_ARRAY_BUFFER, data.ID);
	glVertexAttribPointer(
		0,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,				                  // stride
		(void*) 0                         // array buffer offset
	);

	VAO.unbind();
}


DebugRenderer::~DebugRenderer() {
}

void DebugRenderer::renderVector(std::vector<glm::vec2>& line, int type, RenderInfo& renderInfo) {
	GLEnabler glEnabler;

	glEnabler.disable(GL_DEPTH_TEST).disable(GL_BLEND);

	if (line.size() == 0) {
		return;
	}

	int drawtype = 0;
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

	VAO.bind();
	program.use();

	VP.set(renderInfo.cameraInfo.VP);

	glBindBuffer(GL_ARRAY_BUFFER, data.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * line.size(), &(line[0]), GL_STATIC_DRAW);

	glDrawArrays(
		drawtype,	// mode
		0,				// start
		static_cast<GLsizei>(line.size())   // count
	);

	VAO.unbind();
}
