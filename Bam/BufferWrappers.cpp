#include "common.h"
#include "BufferWrappers.h"


bwo::VertexArrayObject::~VertexArrayObject() {
	glDeleteVertexArrays(1, &ID);
}

bwo::Buffer::~Buffer() {
	glDeleteFramebuffers(1, &ID);
}

bwo::Texture::~Texture() {
	glDeleteTextures(1, &ID);
}

void bwo::Program::use() {
	glUseProgram(ID);
}

bwo::Program::~Program() {
	glDeleteProgram(ID);
}

bwo::UniformMatrix4fv::UniformMatrix4fv(std::string name, Program& program) {
	location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::UniformMatrix4fv::set(glm::mat4& mat) {
	glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}

void bwo::UniformTexture2D::set(GLuint texture) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(location, unit);
}

bwo::UniformTexture2D::UniformTexture2D(std::string name, Program & program, int unit_) {
	unit = unit_;
	location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform3fv::set(glm::vec3 vec) {
	glUniform3fv(location, 1, &vec[0]);
}

bwo::Uniform3fv::Uniform3fv(std::string name, Program & program) {
	location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform1f::set(float f) {
	glUniform1f(location, f);
}

bwo::Uniform1f::Uniform1f(std::string name, Program & program) {
	location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::UniformTexture2DArray::set(GLuint texture) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	glUniform1i(location, unit);
}

bwo::UniformTexture2DArray::UniformTexture2DArray(std::string name, Program & program, int unit_) {
	unit = unit_;
	location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform1i::set(int i) {
	glUniform1i(location, i);
}

bwo::Uniform1i::Uniform1i(std::string name, Program & program) {
	location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform2fv::set(glm::vec2 vec) {
	glUniform2fv(location, 1, &vec[0]);
}

bwo::Uniform2fv::Uniform2fv(std::string name, Program & program) {
	location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform2iv::set(glm::ivec2 vec) {
	glUniform2iv(location, 1, &vec[0]);
}

bwo::Uniform2iv::Uniform2iv(std::string name, Program & program) {
	location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform4fv::set(glm::vec4 vec) {
	glUniform4fv(location, 1, &vec[0]);
}

bwo::Uniform4fv::Uniform4fv(std::string name, Program & program) {
	location = glGetUniformLocation(program.ID, name.c_str());
}
