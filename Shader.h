#ifndef OPENGLC___SHADER_H
#define OPENGLC___SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader {
public:
	GLuint program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use();
};


#endif //OPENGLC___SHADER_H
