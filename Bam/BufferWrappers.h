#pragma once

namespace bwo
{
	class VertexArrayObject
	{
	private:
		GLuint ID;
	public:
		void gen(int32_t count) {
			glGenVertexArrays(1, &ID);
			bind();
			for (int32_t i = 0; i < count; i++) {
				glEnableVertexAttribArray(i);
			}
		};
		void bind() { glBindVertexArray(ID); };
		void unbind() { glBindVertexArray(0); };

		VertexArrayObject() = default;
		~VertexArrayObject();

		NOCOPYMOVE(VertexArrayObject);
	};

	class Buffer
	{
	public:
		GLuint ID;

		Buffer() = default;
		Buffer(GLuint ID_) : ID(ID_) {};
		~Buffer();

		NOCOPYMOVE(Buffer);
	};

	class Texture
	{
	public:
		GLuint ID;

		Texture() = default;
		Texture(GLuint ID_) : ID(ID_) {};
		~Texture();

		NOCOPYMOVE(Texture);
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

		Program() = default;
		Program(GLuint ID_) : ID(ID_) {};
		~Program();

		NOCOPYMOVE(Program);
	};

	class Uniform2iv
	{
	private:
		GLuint location;

	public:
		void set(glm::ivec2 vec);
		void set(std::vector<glm::ivec2> const& vecs);

		Uniform2iv() = default;
		Uniform2iv(std::string name, Program const& program);
		~Uniform2iv() = default;
	};

	class Uniform2fv
	{
	private:
		GLuint location;

	public:
		void set(glm::vec2 vec);

		Uniform2fv() = default;
		Uniform2fv(std::string name, Program const& program);
		~Uniform2fv() = default;
	};

	class UniformMatrix4fv
	{
	private:
		GLuint location;

	public:
		void set(glm::mat4 const& mat);

		UniformMatrix4fv() = default;
		UniformMatrix4fv(std::string name, Program const& program);
		~UniformMatrix4fv() = default;
	};

	class UniformTexture2D
	{
	private:
		int32_t unit;
		GLuint location;

	public:
		void set(GLuint texture);

		UniformTexture2D() = default;
		UniformTexture2D(std::string name, Program const& program, int32_t unit);
		~UniformTexture2D() = default;
	};

	class Uniform3fv
	{
	private:
		GLuint location;

	public:
		void set(glm::vec3 vec);

		Uniform3fv() = default;
		Uniform3fv(std::string name, Program const& program);
		~Uniform3fv() = default;
	};

	class Uniform4fv
	{
	private:
		GLuint location;

	public:
		void set(glm::vec4 vec);

		Uniform4fv() = default;
		Uniform4fv(std::string name, Program const& program);
		~Uniform4fv() = default;
	};

	class Uniform1f
	{
	private:
		GLuint location;

	public:
		void set(float f);

		Uniform1f() = default;
		Uniform1f(std::string name, Program const& program);
		~Uniform1f() = default;
	};

	class UniformTexture2DArray
	{
	private:
		GLuint location;
		int32_t unit;

	public:
		void set(GLuint texture);

		UniformTexture2DArray() = default;
		UniformTexture2DArray(std::string name, Program const& program, int32_t unit);
		~UniformTexture2DArray() = default;
	};

	class Uniform1i
	{
	private:
		GLuint location;

	public:
		void set(int32_t i);

		Uniform1i() = default;
		Uniform1i(std::string name, Program const& program);
		~Uniform1i() = default;
	};
}
