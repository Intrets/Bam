#include "common.h"
#include "BufferWrappers.h"


bwo::VertexArrayObject::~VertexArrayObject() {
	glDeleteVertexArrays(1, &this->ID);
}

bwo::Buffer::~Buffer() {
	glDeleteFramebuffers(1, &this->ID);
}

bwo::Texture::~Texture() {
	glDeleteTextures(1, &this->ID);
}

void bwo::Program::use() {
	glUseProgram(this->ID);
}

bwo::Program::~Program() {
	glDeleteProgram(this->ID);
}

bwo::UniformMatrix4fv::UniformMatrix4fv(std::string name, Program const& program) {
	this->location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::UniformMatrix4fv::set(glm::mat4 const& mat) {
	glUniformMatrix4fv(this->location, 1, GL_FALSE, &mat[0][0]);
}

void bwo::UniformTexture2D::set(GLuint texture) {
	glActiveTexture(GL_TEXTURE0 + this->unit);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(this->location, this->unit);
}

bwo::UniformTexture2D::UniformTexture2D(std::string name, Program const& program, int32_t unit_) {
	this->unit = unit_;
	this->location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform3fv::set(glm::vec3 vec) {
	glUniform3fv(this->location, 1, &vec[0]);
}

bwo::Uniform3fv::Uniform3fv(std::string name, Program const& program) {
	this->location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform1f::set(float f) {
	glUniform1f(this->location, f);
}

bwo::Uniform1f::Uniform1f(std::string name, Program const& program) {
	this->location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::UniformTexture2DArray::set(GLuint texture) {
	glActiveTexture(GL_TEXTURE0 + this->unit);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
	glUniform1i(location, this->unit);
}

bwo::UniformTexture2DArray::UniformTexture2DArray(std::string name, Program const& program, int32_t unit_) {
	this->unit = unit_;
	this->location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform1i::set(int32_t i) {
	glUniform1i(this->location, i);
}

bwo::Uniform1i::Uniform1i(std::string name, Program const& program) {
	this->location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform2fv::set(glm::vec2 vec) {
	glUniform2fv(this->location, 1, &vec[0]);
}

bwo::Uniform2fv::Uniform2fv(std::string name, Program const& program) {
	this->location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform2iv::set(glm::ivec2 vec) {
	glUniform2iv(this->location, 1, &vec[0]);
}

void bwo::Uniform2iv::set(std::vector<glm::ivec2> const& vecs) {
	glUniform2iv(this->location, static_cast<GLsizei>(vecs.size()), &vecs[0][0]);
}

bwo::Uniform2iv::Uniform2iv(std::string name, Program const& program) {
	this->location = glGetUniformLocation(program.ID, name.c_str());
}

void bwo::Uniform4fv::set(glm::vec4 vec) {
	glUniform4fv(this->location, 1, &vec[0]);
}

bwo::Uniform4fv::Uniform4fv(std::string name, Program const& program) {
	this->location = glGetUniformLocation(program.ID, name.c_str());
}
