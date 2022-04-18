#define GLEW_STATIC
#include "GL/glew.h"

#include "GLFW/glfw3.h"

#include "font.h"
#include "ext/matrix_clip_space.hpp"
#include "gtc/type_ptr.hpp"

#include <map>
#include <cstdio>

const GLuint WIDTH = 854;
const GLuint HEIGHT = 480;

int selector = 0;

std::map<GLchar, Character> charactersJBMono;
std::map<GLchar, Character> charactersTransformer;

GLuint textVao, textVbo;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void renderText(std::map<GLchar, Character> map, Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
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
}

int main() {

    printf("Starting GLFW context, OpenGL 3.3\n");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "TheNullPointerException", nullptr, nullptr);
    if (window == nullptr) {
        printf("Failed to initialize GLFW.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW.\n");
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader textShader("text.vsh", "text.fsh");
	glm::mat4 projection = glm::ortho(0.0F, static_cast<GLfloat>(WIDTH), 0.0F, static_cast<GLfloat>(HEIGHT));
	textShader.use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		printf("{STDERR} Could not initialize FreeType Library\n");
	}
	FT_Face face;
	if (FT_New_Face(ft, "type style.ttf", 0, &face)) {
		printf("{STDERR} Failed to Load font\n");
	}
	FT_Set_Pixel_Sizes(face, 0, 40);
	genASCII(&charactersTransformer, face);
	FT_Face jbMono;
	if (FT_New_Face(ft, "JetBrainsMono-Regular.ttf", 0,&jbMono)) {
		printf("{STDERR} Failed to Load font\n");
	}
	FT_Set_Pixel_Sizes(jbMono, 0, 16);
	genASCII(&charactersJBMono, jbMono);
	FT_Done_Face(face);
	FT_Done_Face(jbMono);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &textVao);
	glGenBuffers(1, &textVbo);
	glBindVertexArray(textVao);
	glBindBuffer(GL_ARRAY_BUFFER, textVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
	    glClear(GL_COLOR_BUFFER_BIT);
	    renderText(charactersTransformer, textShader, "The NullPointerException", 160, 400, 1, glm::vec3(1, 1, 1));
	    renderText(charactersJBMono, textShader, "New start", 160, 232, 1, glm::vec3(0, 1, 0));
	    renderText(charactersJBMono, textShader, "Load Save", 160, 216, 1, glm::vec3(0, 1, 0));
	    renderText(charactersJBMono, textShader, "Create Profile", 160, 200, 1, glm::vec3(0, 1, 0));
	    renderText(charactersJBMono, textShader, "Settings", 160, 184, 1, glm::vec3(0, 1, 0));
	    renderText(charactersJBMono, textShader, "Quit", 160, 168, 1, glm::vec3(0, 1, 0));
	    renderText(charactersJBMono, textShader, ">", 140, 232 - 16 * selector, 1, glm::vec3(1, 1, 1));
		glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		if (selector < 4) {
			++selector;
		}
    } else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		if (selector > 0) {
			--selector;
		}
	}
}