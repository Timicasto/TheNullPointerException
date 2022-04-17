#ifndef OPENGLC___FONT_RENDERER_H
#define OPENGLC___FONT_RENDERER_H

#include <map>
#include <string>
#include "GL/glew.h"
#include "Shader.h"
#include "freetype/freetype.h"
#include "vec3.hpp"
#include "ext/vector_int2.hpp"
#include <ft2build.h>

GLuint textVao, textVbo;

typedef struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
}Character;

void loadJBMono(FT_Face* face, FT_Library ft);
void loadTransformer(FT_Face* face, FT_Library ft);
void genASCII(std::map<GLchar, Character> map, FT_Face face);
void renderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

#endif //OPENGLC___FONT_RENDERER_H
