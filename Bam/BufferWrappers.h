#pragma once

#include "common.h"

namespace bwo
{
	class VertexArrayObject
	{
	private:
		GLuint ID;
	public:
		void gen(int count) {
			glGenVertexArrays(1, &ID);
			bind();
			for (int i = 0; i < count; i++) {
				glEnableVertexAttribArray(i);
			}
		};
		void bind() { glBindVertexArray(ID); };
		void unbind() { glBindVertexArray(0); };

		deleteDefaults(VertexArrayObject);
		VertexArrayObject() = default;
		~VertexArrayObject();
	};

	class Buffer
	{
	public:
		GLuint ID;

		deleteDefaults(Buffer);
		Buffer() = default;
		Buffer(GLuint ID_) : ID(ID_) {};
		~Buffer();
	};

	class Texture
	{
	public:
		GLuint ID;
		deleteDefaults(Texture);
		Texture() = default;
		Texture(GLuint ID_) : ID(ID_) {};
		~Texture();
	};

	class Program
	{
	private:
		friend class Uniform1f;
		friend class Uniform1i;
		friend class Uniform2fv;
		friend class Uniform2iv;
		friend class Uniform3fv;
		friend class Uniform3iv;
		friend class Uniform4fv;
		friend class Uniform4iv;
		friend class UniformMatrix4fv;
		friend class UniformTexture2D;
		friend class UniformTexture2DArray;

		GLuint ID;
	public:
		void use();

		deleteDefaults(Program);
		Program() = default;
		Program(GLuint ID_) : ID(ID_) {};
		~Program();
	};

	class Uniform2iv
	{
	private:
		GLuint location;
	public:
		void set(glm::ivec2 vec);
		Uniform2iv() = default;
		Uniform2iv(std::string name, Program& program);
		~Uniform2iv() = default;
	};

	class Uniform2fv
	{
	private:
		GLuint location;
	public:
		void set(glm::vec2 vec);
		Uniform2fv() = default;
		Uniform2fv(std::string name, Program& program);
		~Uniform2fv() = default;
	};

	class UniformMatrix4fv
	{
	private:
		GLuint location;
	public:
		void set(glm::mat4& mat);

		UniformMatrix4fv() = default;
		UniformMatrix4fv(std::string name, Program& program);
		~UniformMatrix4fv() = default;
	};

	class UniformTexture2D
	{
	private:
		int unit;
		GLuint location;
	public:
		void set(GLuint texture);
		UniformTexture2D() = default;
		UniformTexture2D(std::string name, Program& program, int unit);
		~UniformTexture2D() = default;
	};

	class Uniform3fv
	{
	private:
		GLuint location;
	public:
		void set(glm::vec3 vec);
		Uniform3fv() = default;
		Uniform3fv(std::string name, Program& program);
		~Uniform3fv() = default;
	};

	class Uniform4fv
	{
	private:
		GLuint location;
	public:
		void set(glm::vec4 vec);
		Uniform4fv() = default;
		Uniform4fv(std::string name, Program& program);
		~Uniform4fv() = default;
	};

	class Uniform1f
	{
	private:
		GLuint location;
	public:
		void set(float f);
		Uniform1f() = default;
		Uniform1f(std::string name, Program& program);
		~Uniform1f() = default;
	};

	class UniformTexture2DArray
	{
	private:
		GLuint location;
		int unit;

	public:
		void set(GLuint texture);
		UniformTexture2DArray() = default;
		UniformTexture2DArray(std::string name, Program& program, int unit);
		~UniformTexture2DArray() = default;
	};

	class Uniform1i
	{
	private:
		GLuint location;

	public:
		void set(int i);
		Uniform1i() = default;
		Uniform1i(std::string name, Program& program);
		~Uniform1i() = default;
	};
}
