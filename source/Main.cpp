#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>



int main(int argc, char** argv) 
{
	if (!glfwInit()) {
		std::cerr << "Could not initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(720, 480, "Noise Maker", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Could not create a window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Could not load OpenGL" << std::endl;
		return -1;
	}

	if (!ImGui_ImplOpenGL3_Init("3.3")) {
		std::cerr << "Could not initialize DearImGUI" << std::endl;
		return -1;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwWaitEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}