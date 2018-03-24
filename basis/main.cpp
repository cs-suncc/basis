#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "SOIL.h"

#include <iostream>
#include <fstream>
#include <string>

void onFrameBufferSize(GLFWwindow *window, int w, int h);
void processInput(GLFWwindow *window);
GLuint loadShader(GLenum type, const char *shaderFileSource);

GLfloat vertices[] = {	//color			 //tx
	-0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
	0.8f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
};

GLfloat texcords[] = {
	0.0f, 0.0f,
	0.5f, 1.0f,
	1.0f, 0.0f,
};

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow *wd = glfwCreateWindow(640, 480, "LOGL", nullptr, nullptr);
	if (!wd) {
		std::cerr << "Cannot create GLFW Window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(wd);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to init GLAD" << std::endl;
		return 2;
	}
	glViewport(0, 0, 640, 480);
	glfwSetFramebufferSizeCallback(wd, onFrameBufferSize);

	auto vshader = loadShader(GL_VERTEX_SHADER, "vs.glsl");
	auto fshader = loadShader(GL_FRAGMENT_SHADER, "fs.glsl");

	auto prog = glCreateProgram();
	glAttachShader(prog, vshader);
	glAttachShader(prog, fshader);
	glLinkProgram(prog);


#ifdef _DEBUG
	int success;
	char info[512];
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(prog, 512, nullptr, info);
		std::cerr << "Error on linking shader program\n" << info << std::endl;
		system("pause");
		return -1;
	}
#endif
	glDeleteShader(vshader);
	glDeleteShader(fshader);

	GLuint varray, buffer;
	glGenVertexArrays(1, &varray);
	glBindVertexArray(varray);
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), nullptr);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	int txw, txh, txchan;
	auto txdata = SOIL_load_image("wall.jpg", &txw, &txh, &txchan, SOIL_LOAD_AUTO);
	if (!txdata) {
		std::cerr << "Loading texture wall.jpg failed" << std::endl;
		return 1;
	}
	GLuint tx;
	glGenTextures(1, &tx);
	glBindTexture(GL_TEXTURE_2D, tx);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txw, txh, 0, GL_RGB, GL_UNSIGNED_BYTE, txdata);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(txdata);




	while (!glfwWindowShouldClose(wd)) {
		processInput(wd);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(prog);
		glBindVertexArray(varray);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(wd);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void onFrameBufferSize(GLFWwindow *wd, int w, int h) {
	glViewport(0, 0, w, h);
	std::cout << w << " " << h << std::endl;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

GLuint loadShader(GLenum type, const char *shaderSourceFile) {
	std::ifstream fin(shaderSourceFile);
	if (!fin) {
		std::cerr << "Error on Loding shader " << shaderSourceFile << std::endl;
	}
	std::string source;
	std::string line;
	while (std::getline(fin, line)) {
		line.append("\n");
		source.append(line);
	}
	auto shadersource = source.c_str();
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shadersource, nullptr);
	glCompileShader(shader);

#ifdef _DEBUG
	int success;
	char info[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, nullptr, info);
		std::cerr << "Error on compiling shader " << shaderSourceFile << "\n" << info << std::endl;
		glDeleteShader(shader);
		return GLuint(0);
	}
#endif
	return shader;
}
