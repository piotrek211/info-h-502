#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Context.h"
#include "Camera.h"
namespace GEngine {

	class Window {
		public:
			Window() = default;
			~Window();
			Window(std::string name, int width, int height, bool vsync = false);
			int getWidth() const { return m_width; }
			int getHeight() const { return m_height; }
			void setVSync(bool enabled);
			bool isVSync() const;
			bool onUpdate();
			GLFWwindow* getGLWindow();
			void attach(Camera& camera);
			void processInput();

		private:
			struct windowInfo {
				Camera* camera;
				bool firstMouse = true;
				glm::vec2 lastMousePos;
			};
			std::string m_name;
			struct windowInfo info;
			int m_width = 0;
			int m_height = 0;
			bool m_vsync = false;
			bool windowInit = false;
			GLFWwindow* glWindow = nullptr;
			Context* m_context = nullptr;
			Camera* camera = nullptr;
	
	};

}