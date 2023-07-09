#pragma once

#include <string>

namespace FractalRenderer
{
	std::string readShaderFile(const std::string& shaderFilePath);
	GLuint compileShader(GLenum shaderType, const char* shaderSource);
	GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
}
