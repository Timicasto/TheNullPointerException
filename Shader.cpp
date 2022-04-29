#include "Shader.h"

#define GLEW_STATIC

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath) : program(program) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	fShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (std::ifstream::failure e) {
		printf("%s %d %s\n", e.what(), e.code().value(), e.code().message().c_str());
		printf("{STDERR} FILE NOT SUCCESSFULLY READ\n");
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		printf("{STDERR} VERTEX SHADER COMPILATION FAILED : %s \n", infoLog);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment,512, nullptr, infoLog);
		printf("{STDERR} FRAGMENT SHADER COMPILATION FAILED: %s \n", infoLog);
	}

	this -> program = glCreateProgram();
	glAttachShader(this -> program, vertex);
	glAttachShader(this -> program, fragment);
	glLinkProgram(this -> program);
	glGetProgramiv(this -> program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this -> program, 512, NULL, infoLog);
		printf("{STDERR} PROGRAM LINKING FAILED: %s\n", infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() const {
	glUseProgram(this -> program);
}




