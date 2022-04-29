#include <ft2build.h>
#include FT_FREETYPE_H

#define GLEW_STATIC

#include "font.h"


void loadJBMono(FT_Face* face, FT_Library ft) {
    if (FT_New_Face(ft, "./JetBrainsMono-Regular.ttf", 0, *&face)) {
        printf("{STDERR} Failed to load font.\n");
    }
}

void loadTransformer(FT_Face* face, FT_Library ft) {
    if (FT_New_Face(ft, "./type style.ttf", 0, *&face)) {
        printf("{STDERR} Failed to load font.\n");
    }
}

void genASCII(std::map<GLchar, Character> *map, FT_Face face) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (GLubyte c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			printf("{STDERR} Failed to load Glyph\n");
			continue;
		}
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED,
		             GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<GLuint>(face->glyph->advance.x)
		};
		map->insert(std::pair<GLchar, Character>(c, character));
	}
}

/*void renderText(std::map<GLchar, Character> map, Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
	shader.use();
	glUniform3f(glGetUniformLocation(shader.program, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVao);
	std::string::const_iterator c;
	for (c = text.begin() ; c != text.end() ; ++c) {
		Character ch = map[*c];
		GLfloat xPos = x + ch.bearing.x * scale;
		GLfloat yPos = y - (ch.size.y - ch.bearing.y) * scale;
		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		GLfloat vertices[6][4] = {
				{xPos,     yPos + h, 0, 0},
				{xPos,     yPos,     0, 1},
				{xPos + w, yPos,     1, 1},

				{xPos,     yPos + h, 0, 0},
				{xPos + w, yPos, 1, 1},
				{xPos + w, yPos + h, 1, 0}
		};
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		glBindBuffer(GL_ARRAY_BUFFER, textVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}*/

