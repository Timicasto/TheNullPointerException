#ifndef OPENGLC___SHADER_H
#define OPENGLC___SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include "freetype/freetype.h"
#include "ext/vector_int2.hpp"
#include <ft2build.h>

typedef struct Character {
	GLuint textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
}Character;

class Shader {
public:
	GLuint program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use() const;
};



#endif //OPENGLC___SHADER_H
