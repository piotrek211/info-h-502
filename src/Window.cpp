#include "Window.h"
#include "Log.h"
#include <iostream>
#include "Application.h"
namespace GEngine {
	Window::Window(std::string name, int width, int height, bool vsync) :
		 m_name(name), m_width(width), m_height(height), m_vsync(vsync) {


		GE_CORE_INFO("Creating window {0} ({1}, {2})", name, width, height);

		if(!windowInit){
			int success = glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			GE_CORE_ASSERT(success, "Failed to initialize the GLFW!");
		}

		GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		GE_CORE_ASSERT(window, "Failed to create the window!");
		if (window == NULL) {
			glfwTerminate();
		}

		glWindow = window;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_context = new Context(window);
		m_context->init();
		setVSync(true);


	}

	void Window::attach(Camera& camera) {
		this->camera = &camera;
		this->info.camera = &camera;
		this->info.lastMousePos = glm::vec2(m_width / 2.0f, m_height / 2.0f);
		glfwSetWindowUserPointer(glWindow, &info);
		glfwSetKeyCallback(glWindow, [](GLFWwindow* glWindow, int key, int scancode, int action, int mods) {
			if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
				glfwSetWindowShouldClose(glWindow, true);
			}
			else if (Input::isKeyPressed(GLFW_KEY_DOWN)) {
				(Application::getApplication()).launchBall();

			}
			else if (Input::isKeyPressed(GLFW_KEY_RIGHT)) {
				(Application::getApplication()).moveBall("right");

			}
			else if (Input::isKeyPressed(GLFW_KEY_LEFT)) {
				(Application::getApplication()).moveBall("left");

			}
			else if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
				(Application::getApplication()).resetAll();

			}
			else if (Input::isKeyPressed(GLFW_KEY_F)) {
				(Application::getApplication()).enableShadows();

			}
			else if (Input::isKeyPressed(GLFW_KEY_1)) {
				(Application::getApplication()).selectedBall(0);

			}
			else if (Input::isKeyPressed(GLFW_KEY_2)) {
				(Application::getApplication()).selectedBall(1);

			}
			else if (Input::isKeyPressed(GLFW_KEY_3)) {
				(Application::getApplication()).selectedBall(2);

			}

			else {
				Camera* camera = (Camera*)((windowInfo*)glfwGetWindowUserPointer(glWindow))->camera;
				Camera_Movement camera_direction = Input::getMovement(key);
				if (camera_direction != Camera_Movement::NONE) {
					camera->onKeyInput(camera_direction);
				}
			}
			});

		glfwSetCursorPosCallback(glWindow, [](GLFWwindow* glWindow, double xPos, double yPos){
			windowInfo* info = (windowInfo*)glfwGetWindowUserPointer(glWindow);
			Camera* camera = info->camera;
			if (info->firstMouse) {
				info->lastMousePos = glm::vec2((float)xPos, (float)yPos);
				info->firstMouse = false;
			}
			float xOff = (float)xPos - info->lastMousePos.x;
			float yOff = (float)yPos - info->lastMousePos.y;
			info->lastMousePos.x = (float)xPos;
			info->lastMousePos.y = (float)yPos;
			camera->onMouseMovment(xOff, yOff);
			});

		glfwSetScrollCallback(glWindow, [](GLFWwindow* glWindow, double xOffset, double yOffset){
			float fov = (Application::getApplication()).getFov();
			if (fov >= 1.0f && fov <= 45.0f)
				fov -= yOffset;
			if (fov <= 1.0f)
				fov = 1.0f;
			if (fov >= 45.0f)
				fov = 45.0f;
			(Application::getApplication()).setFov(fov);
		});

		glfwSetFramebufferSizeCallback(glWindow, [](GLFWwindow* glWindow, int width, int height) {
			glViewport(0, 0, width, height);
			});

	}


	void Window::processInput(){
		if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(glWindow, true);
		}
	}

	bool Window::onUpdate() {
		glm::vec2 pos = Input::getMousePosition();
		bool close = glfwWindowShouldClose(glWindow);
		glfwPollEvents();
		m_context->swapBuffers();
		return close;
	}

	void Window::setVSync(bool endis) {
		m_vsync = endis;
	}
	bool Window::isVSync() const {
		return m_vsync;
	}

	GLFWwindow* Window::getGLWindow() {
		return glWindow;

	}
	Window::~Window() {
		delete m_context;
	}
}