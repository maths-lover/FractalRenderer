#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

namespace FractalRenderer {

	std::string readShaderFile(const std::string& shaderFilePath)
	{
		std::ifstream shaderFile(shaderFilePath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();

		return shaderStream.str();
	}

	GLuint compileShader(GLenum shaderType, const char* shaderSource)
	{
		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderSource, nullptr);
		glCompileShader(shader);

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint logLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<GLchar> log(logLength);
			glGetShaderInfoLog(shader, logLength, nullptr, log.data());
			std::cerr << "Shader compilation failed:\n" << log.data() << std::endl;
			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}

	GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
		GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) {
			GLint logLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<GLchar> log(logLength);
			glGetProgramInfoLog(program, logLength, nullptr, log.data());
			std::cerr << "Shader program linking failed:\n" << log.data() << std::endl;
			glDeleteProgram(program);
			program = 0;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return program;
	}
}
