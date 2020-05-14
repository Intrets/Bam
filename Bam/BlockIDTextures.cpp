#include "common.h"
#include "BlockIDTextures.h"

#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "GLEnableWrapper.h"

int32_t BlockIDTextures::getBlockTextureID(std::string name) {
	auto search = textures.find(name);
	if (search == textures.end()) {
		// TODO : actually make texture loading work
		loadBlockTexture(name);
		textures[name] = arrayLayers;
		return arrayLayers++;
	}
	else {
		return search->second;
	}
}

void BlockIDTextures::loadBlockTexture(std::string name) {
	GLuint newTex = Locator<PathManager>::getService()->LoadTextureP(name);
	// Failed to load texture
	if (newTex == 0) {
		printf("Failed to load texture: %s\n\n", name.c_str());
	}
	//textureDatas.push_back(newTex);

	GLEnabler glEnabler;
	glEnabler.disable(GL_BLEND);

	program.use();
	VAO.bind();
	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, textureBuffer.ID);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray.ID);

	glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureArray.ID, 0, arrayLayers);
	texture.set(newTex);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	VAO.unbind();
}

GLuint BlockIDTextures::getTextureArrayID() {
	return textureArray.ID;
}

BlockIDTextures::BlockIDTextures() :
	program(Locator<PathManager>::getService()->LoadShadersP("PassthroughArray.vert", "PassthroughArray.frag")),
	layer("layer", program),
	texture("texture", program, 0) {
	// The quad's FBO. Used only for visualizing the shadowmap.
	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	VAO.gen(1);

	glGenBuffers(1, &quad.ID);
	glBindBuffer(GL_ARRAY_BUFFER, quad.ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*) 0            // array buffer offset
	);
	VAO.unbind();

	const std::string textures_raw[] = { "debug.dds", "debug.dds", "sandstone.dds", "dirt.dds",
		"stone.dds", "gravel.dds", "cobblestone.dds", "gravel2.dds", "mossy_cobblestone.dds", "diorite.dds", "andesite.dds", "podzol.dds", "fruit.dds" };

	glGenFramebuffers(1, &textureBuffer.ID);
	glBindFramebuffer(GL_FRAMEBUFFER, textureBuffer.ID);

	glGenTextures(1, &textureArray.ID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray.ID);

	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxArrayLayers);
	printf("%d", maxArrayLayers);
	maxArrayLayers = glm::max(1024, maxArrayLayers);
	// TODO: remove hardcoding of texture size
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, 32, 32, maxArrayLayers);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//TODO: setting anything of GL_TEXTURE_2D_ARRAY messes things up
	//SOLUTION?: need to use uniformi?
	/*glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);*/
	//glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, textureArray.ID, 0, 0);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureArray.ID, 0);

	for (auto& s : textures_raw) {
		loadBlockTexture(s);
		textures[s] = arrayLayers++;
	}
	/*
		program.use();
		VAO.bind();

		glBindFramebuffer(GL_FRAMEBUFFER, textureBuffer.ID);

		glViewport(0, 0, 32, 32);
		for (size_t i = 0; i < ts.size(); i++) {
			glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureArray.ID, 0, i);
			layer.set(i);
			texture.set(ts[i]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glBindVertexArray(0);*/
}
