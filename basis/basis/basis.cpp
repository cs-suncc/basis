#include "basis.h"
#include <utility>

namespace bs
{
void init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

BsInt createWindow(const char * title, BsInt width, BsInt height) {
	GLFWwindow *wd = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!wd) {
		return 1;
		//ERROR 1
	}
	glfwMakeContextCurrent(wd);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return 2;
	}
	glViewport(0, 0, width, height);
	return 0;
}
}