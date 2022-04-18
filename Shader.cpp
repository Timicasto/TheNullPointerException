#include "Shader.h"

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath) : program(program) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);

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
		printf("{STDERR} FILE NOT SUCCESFULLY READ\n");
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("{STDERR} VERTEX SHADER COMPILATION FAILED : %s \n", infoLog);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment,512, NULL, infoLog);
		printf("{STDERR} FRAGMENT SHADER COMPILATION FAILED: %s \n", infoLog);
	}

	this -> program = glCreateProgram();
	glAttachShader(this -> program, vertex);
	glAttachShader(this -> program, fragment);
	glLinkProgram(this -> program);
	glGetProgramiv(this -> program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this -> program, 512, NULL, infoLog);
		printf("{STDERR} PROGRAM LINKING FAILED\n");
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(this -> program);
}




