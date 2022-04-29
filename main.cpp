#include "GL/glew.h"
#define GLEW_STATIC

#include "ext.hpp"

#include "GLFW/glfw3.h"

#include "font.h"
#include "ext/matrix_clip_space.hpp"
#include "gtc/type_ptr.hpp"
#include "fields.h"
#include <cstdlib>

#include "Profile.h"

#include <map>
#include <cstdio>

const GLuint WIDTH = 854;
const GLuint HEIGHT = 480;

int selector = 0;
int selectorForProfileCreating = 0;
int page = 0;

std::string input[6] = {"", "", "", "", "", ""};
std::string loadString;
std::string name = "root";
std::string username;

Profile curr = Profile("Default", "root", UNKNOWN, 0, 0, 0);

std::map<GLchar, Character> charactersJBMono;
std::map<GLchar, Character> charactersTransformer;

GLuint textVao, textVbo;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

void renderText(std::map<GLchar, Character> map, Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale,
                glm::vec3 color) {
	shader.use();
	glUniform3f(glGetUniformLocation(shader.program, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVao);
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); ++c) {
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
				{xPos + w, yPos,     1, 1},
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
	glfwSwapInterval(0);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "TheNullPointerException", nullptr, nullptr);
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
	if (FT_New_Face(ft, "./type style.ttf", 0, &face)) {
		printf("{STDERR} Failed to Load font\n");
	}
	FT_Set_Pixel_Sizes(face, 0, 40);
	genASCII(&charactersTransformer, face);
	FT_Face jbMono;
	if (FT_New_Face(ft, "./JetBrainsMono-Regular.ttf", 0, &jbMono)) {
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
		if (page == 0) {
			renderText(charactersTransformer, textShader, "The NullPointerException", 160, 400, 1, glm::vec3(1, 1, 1));
			renderText(charactersJBMono, textShader, "New start", 160, 232, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, "Load Save", 160, 216, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, "Create Profile", 160, 200, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, "Settings", 160, 184, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, "Load Profile", 160, 168, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, "Quit", 160, 152, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, ">", 140, 232 - 16 * selector, 1, glm::vec3(1, 1, 1));
			renderText(charactersJBMono, textShader, username, 120, 100, 1, glm::vec3(0, 1, 0));
		} else if (page == 2) {
			if (selectorForProfileCreating == 6) {
				renderText(charactersJBMono, textShader, ">", 140, 344 - 16 * selectorForProfileCreating, 1,glm::vec3(1, 1, 1));
			} else {
				renderText(charactersJBMono, textShader, ">               _", 140, 344 - 16 * selectorForProfileCreating, 1,glm::vec3(1, 1, 1));
			}
			renderText(charactersJBMono, textShader, "Profile Name", 160, 344, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, "Player Name", 160, 328, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, "Gender", 160, 312, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, "Age", 160, 296, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, "Height", 160, 280, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, "Weight", 160, 264, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, "Save", 160, 248, 1, glm::vec3(0, 1, 0));
			for (int i = 0 ; i < 6 ; ++i) {
				renderText(charactersJBMono, textShader, input[i], 160 + 16 * 9, 344 - 16 * i, 1, glm::vec3(1, 1, 1));
			}
		} else if (page == 3) {

		} else if (page == 4) {
			renderText(charactersJBMono, textShader, "Profile Name", 160, 248, 1, glm::vec3(0, 1, 0));
			renderText(charactersJBMono, textShader, ">                      _", 140, 248, 1,glm::vec3(1, 1, 1));
			renderText(charactersJBMono, textShader, loadString, 372, 248, 1, glm::vec3(1, 1, 1));
		}
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void updateName() {
	username = name.append("@TheNullPointerException ~ $_");
}

void enterCharforCreatingProfile(int selectorCurr, int key) {
	if (key == GLFW_KEY_BACKSPACE && !input[selectorCurr].empty()) {
		input[selectorCurr].erase(input[selectorCurr].end() - 1);
	} else {
		input[selectorCurr].append(&keyMap[key]);
	}
}

void enterChar(int key) {
	if (key == GLFW_KEY_BACKSPACE && !loadString.empty()) {
		loadString.erase(loadString.end() - 1);
	} else {
		loadString.append(&keyMap[key]);
	}
}

Gender parseGender(const std::string& str) {
	if (str == "M" || str == "MALE") {
		return MALE;
	} else if (str == "F" || str == "FEMALE") {
		return FEMALE;
	} else if (str == "INTERSEX") {
		return INTERSEX;
	} else if (str == "TRANS" || str == "TRANSGENDER" || str == "FTM" || str == "MTF") {
		return TRANSGENDER;
	} else if (str == "NON-BINARY" || str == "MTX" || str == "FTX") {
		return NON_BINARY;
	} else {
		return UNKNOWN;
	}
}

void saveProfile() {
	Profile profile = Profile(input[0], input[1], parseGender(input[2]), atoi(input[3].c_str()), atoi(input[4].c_str()), atoi(input[5].c_str()));
	profile.saveToFile();
	for (auto & i : input) {
		i = "";
	}
}

void loadProfile(std::string profileName) {
	curr = Profile::readFromFile(std::to_string(std::hash<std::string>()(profileName)));
	name = curr.playerName;
	loadString = "";
	updateName();
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (page == 0) {
		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			if (selector < 5) {
				++selector;
			}
		} else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			if (selector > 0) {
				--selector;
			}
		} else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
			switch (selector) {
				case 5:
					glfwTerminate();
					exit(0);
				case 4:
					page = 4;
				case 3:
					return;
				case 2:
					page = 2;
					return;
				case 1:
					return;
				case 0:
					return;
				default:
					return;
			}
		}
	} else if (page == 2) {
		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && GLFW_REPEAT) {
			if (selectorForProfileCreating < 6) {
				++selectorForProfileCreating;
			}
		} else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			if (selectorForProfileCreating > 0) {
				--selectorForProfileCreating;
			}
		} else if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			page = 0;
		} else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
			if (selectorForProfileCreating == 6) {
				saveProfile();
			}
		}
		if (selectorForProfileCreating != 6 && action == GLFW_PRESS ) {
			enterCharforCreatingProfile(selectorForProfileCreating, key);
		}
	} else if (page == 4) {
		if (action == GLFW_PRESS) {
			enterChar(key);
		}
		if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
			loadProfile(loadString);
			page = 0;
		}
	}
}
