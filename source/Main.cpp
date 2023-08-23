#include <iostream>
#include <vector>
#include <string>

#include "Render/OpenGL.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Application.hpp"

//==============================================================
//						GL Error Handling
//==============================================================

#if _DEBUG

/* Adapted from https://learnopengl.com/In-Practice/Debugging */
void APIENTRY putDebugContextMessage(
	GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	//ignore alignment warnings:
	if (id == 101) return;

	const char* colorStr = "";
	const char* typeStr = "";
	const char* sourceStr = "";
	const char* severityStr = "";

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:				colorStr = "\033[91m", typeStr = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	colorStr = "", typeStr = "Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	colorStr = "", typeStr = "Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:			colorStr = "", typeStr = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:			colorStr = "", typeStr = "Performance"; break;
	case GL_DEBUG_TYPE_MARKER:				colorStr = "", typeStr = "Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:			colorStr = "", typeStr = "Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:			colorStr = "", typeStr = "Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:				colorStr = "", typeStr = "Other"; break;
	}

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             sourceStr = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceStr = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceStr = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     sourceStr = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           sourceStr = "Other"; break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         severityStr = "high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       severityStr = "medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          severityStr = "low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "notification"; break;
	}

	std::cerr << "Debug Message(" << id << "): " << colorStr << message <<
		"\033[0m\n  Type:" << typeStr <<
		"\n  Source: " << sourceStr <<
		"\n  Severity: " << severityStr << std::endl;
}

/* Adapted from https://learnopengl.com/In-Practice/Debugging */
void _internal_checkGlErrors(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		const char* error = "";
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}

		std::cerr << file << '(' << line << "): " << error << '\n' << std::endl;
	}
}

#endif//#if DEBUG

bool keyState[1024];

bool keyDown(int key) 
{
	return keyState[key];
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		keyState[key] = true;
	else if (action == GLFW_RELEASE)
		keyState[key] = false;
}

void updateWindowTitle(GLFWwindow* window, float deltaTime)
{
	static float accumTime = 0.0f;
	static int frameCount = 0;

	accumTime += deltaTime;
	frameCount++;

	if (accumTime >= 0.5) {
		int ms	= (int)(deltaTime * 1000.0f);
		int fps = (int)((float)frameCount / 0.5f);

		std::string title = "Noise Maker    ";
		title += "Fps: " + std::to_string(fps) +  "    ";
		title += "Ms: " + std::to_string(ms);

		glfwSetWindowTitle(window, title.c_str());

		accumTime = 0.0;
		frameCount = 0;
	}
}

int __cdecl main(int argc, char** argv) 
{
	if (!glfwInit()) {
		std::cerr << "Could not initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	GLFWwindow* window = glfwCreateWindow(DEF_WIN_WIDTH, DEF_WIN_HEIGHT, "Noise Maker", nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Could not create a window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);

	glewExperimental = true;
	if (GLenum err = glewInit() != GLEW_OK) {
		std::cerr << "Could not initalize glew." << std::endl;
		return -1;
	}

	//Initialization for debug contexts must be done after GLEW loads all of the
	//OpenGL functionality
	#if _DEBUG && USE_DEBUG_CONTEXT
		int contextFlags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);

		//Use the debug context if it was properly created, otherwise use normal OpenGL error checking
		if (contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT) {
			std::cout << "Using debug context for error logging" << std::endl;
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(putDebugContextMessage, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
		else {
			std::cout << "Debug Contexts Not Available" << std::endl;
		}

	#elif _DEBUG
		std::cout << "Debug Contexts Disabled" << endl;
	#endif //#if DEBUG

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	glfwSetKeyCallback(window, keyCallback);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	if (!init(DEF_WIN_WIDTH, DEF_WIN_HEIGHT)) {
		std::cerr << "Could not initialize application\n" << std::endl;
		return -1;
	}

	double prevTime = glfwGetTime();

 	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		int w, h;
		glfwGetWindowSize(window, &w, &h);

		double curTime = glfwGetTime();
		double deltaTime = curTime - prevTime;
		prevTime = curTime;

		if (!update(w, h, deltaTime))
			glfwSetWindowShouldClose(window, true);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		updateWindowTitle(window, deltaTime);
	}	

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}