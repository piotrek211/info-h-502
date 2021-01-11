#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace GEngine {
	class Context {
		public:
			Context(GLFWwindow* window): glWindow(window) {};
			void init();
			void swapBuffers();
		private:
			GLFWwindow* glWindow;

	};




}