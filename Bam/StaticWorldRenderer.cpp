#include "common.h"
#include "StaticWorldRenderer.h"

#include <vector>

#include "ShaderLoader.h"
#include "CameraInfo.h"
#include "BlockIDTextures.h"
#include "GLEnableWrapper.h"

#include "ModelResource.h"
#include "ModelStore.h"

StaticWorldRenderer::StaticWorldRenderer() :
	program(Locator<PathManager>::get()->LoadShadersP("StaticWorldShader.vert", "StaticWorldShader.frag")),
	texture("myTextureSampler", this->program, 0),
	VP("VP", this->program)
{
	this->VAO.gen(5);

	ModelResource tempp("devtile.obj");

	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, tempp.get()->vertexbufferHandle);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*) 0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, tempp.get()->uvbufferHandle);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*) 0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glBindBuffer(GL_ARRAY_BUFFER, tempp.get()->normalbufferHandle);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*) 0                          // array buffer offset
	);

	//// 1rst attribute buffer : vertices
	//glBindBuffer(GL_ARRAY_BUFFER, temp->model.shared->data.vertexbufferHandle);
	//glVertexAttribPointer(
	//	0,                  // attribute
	//	3,                  // size
	//	GL_FLOAT,           // type
	//	GL_FALSE,           // normalized?
	//	0,                  // stride
	//	(void*) 0            // array buffer offset
	//);

	//// 2nd attribute buffer : UVs
	//glBindBuffer(GL_ARRAY_BUFFER, temp->model.shared->data.uvbufferHandle);
	//glVertexAttribPointer(
	//	1,                                // attribute
	//	2,                                // size
	//	GL_FLOAT,                         // type
	//	GL_FALSE,                         // normalized?
	//	0,                                // stride
	//	(void*) 0                          // array buffer offset
	//);

	//// 3rd attribute buffer : normals
	//glBindBuffer(GL_ARRAY_BUFFER, temp->model.shared->data.normalbufferHandle);
	//glVertexAttribPointer(
	//	2,                                // attribute
	//	3,                                // size
	//	GL_FLOAT,                         // type
	//	GL_FALSE,                         // normalized?
	//	0,                                // stride
	//	(void*) 0                          // array buffer offset
	//);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 4th attribute buffer : offsets
	glGenBuffers(1, &this->offset.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->offset.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * MAX_STATIC_DRAW, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(
		3,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,								  // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(3, 1);

	glGenBuffers(1, &this->textureID.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->textureID.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(int32_t) * MAX_STATIC_DRAW, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(
		4,                                // attribute
		1,                                // size
		GL_INT,                           // type
		GL_FALSE,                         // normalized?
		0,							      // stride
		(void*) 0                         // array buffer offset
	);
	glVertexAttribDivisor(4, 1);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempp.get()->indexbufferHandle);

	this->VAO.unbind();
}

StaticWorldRenderer::~StaticWorldRenderer() {
}

void StaticWorldRenderer::render(StaticWorldRenderInfo & info, GLuint target, CameraInfo & cameraInfo) {
	ModelResource tempp("devtile.obj");

	this->VAO.bind();
	this->program.use();

	GLEnabler glEnabler;
	glEnabler.disable(GL_BLEND);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, target);
	glViewport(0, 0, cameraInfo.x, cameraInfo.y); 

	this->texture.set(Locator<BlockIDTextures>::get()->getTextureArrayID());

	if (info.offsets.size() == 0) {
		return;
	}

	this->VP.set(cameraInfo.VP);

	int32_t drawCount = glm::min(MAX_STATIC_DRAW, static_cast<int32_t>(info.offsets.size()));

	glBindBuffer(GL_ARRAY_BUFFER, this->offset.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(decltype(info.offsets)::value_type) * drawCount, &info.offsets[0]);

	glBindBuffer(GL_ARRAY_BUFFER, this->textureID.ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int32_t) * drawCount, &info.textureIDs[0]);

	glDrawElementsInstanced(
		GL_TRIANGLES,      // mode
		tempp.get()->indexbufferSize,    // count
		GL_UNSIGNED_SHORT, // type
		(void*) 0,           // element array buffer offset
		drawCount
	);

	this->VAO.unbind();
}

void StaticWorldRenderer::render(std::vector<StaticWorldRenderInfo*> infos, GLuint target, CameraInfo & cameraInfo) {
	ModelResource temp("devtile.obj");

	this->VAO.bind();
	this->program.use();

	GLEnabler glEnabler;
	glEnabler.disable(GL_BLEND);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, target);
	glViewport(0, 0, cameraInfo.x, cameraInfo.y);

	this->texture.set(Locator<BlockIDTextures>::get()->getTextureArrayID());

	if (infos.size() == 0) {
		return;
	}

	this->VP.set(cameraInfo.VP);

	int32_t offseti = 0;

	for (auto info : infos) {
		auto infoSize = static_cast<int32_t>(info->offsets.size());
		bool done = false;

		if (offseti + infoSize > MAX_STATIC_DRAW) {
			done = true;
			infoSize = MAX_STATIC_DRAW - offseti;
		}

		glBindBuffer(GL_ARRAY_BUFFER, this->offset.ID);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(decltype(info->offsets)::value_type) * offseti, sizeof(glm::vec2) * infoSize, &info->offsets[0]);

		glBindBuffer(GL_ARRAY_BUFFER, this->textureID.ID);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(int32_t) * offseti, sizeof(int32_t) * infoSize, &info->textureIDs[0]);
		offseti += infoSize;

		if (done) {
			break;
		}
	}

	glDrawElementsInstanced(
		GL_TRIANGLES,      // mode
		temp.get()->indexbufferSize,    // count
		GL_UNSIGNED_SHORT, // type
		(void*) 0,           // element array buffer offset
		offseti
	);

	this->VAO.unbind();
}