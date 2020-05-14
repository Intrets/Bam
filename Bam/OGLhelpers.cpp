#include "common.h"
#include "OGLhelpers.h"

#include "ModelResource.h"
#include "ModelStore.h"

void ModelBinder::bind(ModelResource model) {
	if (vertex > -1) {
		glEnableVertexAttribArray(vertex);
		glBindBuffer(GL_ARRAY_BUFFER, model.get()->vertexbufferHandle);
		glVertexAttribPointer(
			vertex,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*) 0            // array buffer offset
		);
	}

	if (uv > -1) {
		glEnableVertexAttribArray(uv);
		glBindBuffer(GL_ARRAY_BUFFER, model.get()->uvbufferHandle);
		glVertexAttribPointer(
			uv,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*) 0                          // array buffer offset
		);
	}

	if (normal > -1) {
		glEnableVertexAttribArray(normal);
		glBindBuffer(GL_ARRAY_BUFFER, model.get()->normalbufferHandle);
		glVertexAttribPointer(
			normal,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*) 0                          // array buffer offset
		);
	}

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.get()->indexbufferHandle);
}

ModelBinder::ModelBinder(int32_t vertex_, int32_t uv_, int32_t normal_) :
	vertex(vertex_), uv(uv_), normal(normal_) {
}
