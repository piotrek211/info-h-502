
#include "Input.h"
//#include <GLFW/glfw3.h>
#include "Application.h"
#include <iostream>
namespace GEngine {

	bool Input::isKeyPressed(const inputCode key){
		GLFWwindow* window = ((Application::getApplication()).getWindow())->getGLWindow();
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::isMouseButtonPressed(const inputCode button) {
		return false;

	}
	bool Input::escapePressed() {
		return false;

	}

	Camera_Movement Input::getMovement(int key) {
		switch (key) {
			case(QWERTY::FORWARD):
				return Camera_Movement::FORWARD;
				break;
			case(QWERTY::BACKWARD):
				return Camera_Movement::BACKWARD;
				break;
			case(QWERTY::LEFT):
				return Camera_Movement::LEFT;
				break;
			case(QWERTY::RIGHT):
				return Camera_Movement::RIGHT;
				break;
			default:
				return Camera_Movement::NONE;
				break;
		}

	}


	glm::vec2 Input::getMousePosition() {
		GLFWwindow* window = ((Application::getApplication()).getWindow())->getGLWindow();
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}


	float Input::getMouseX() {
		return getMousePosition().x;

	}

	float Input::getMouseY() {

		return getMousePosition().y;
	}

	inputCode Input::getKey(const Camera_Movement mouvement) {
			if (mouvement == Camera_Movement::FORWARD) {
				return QWERTY::FORWARD;
			}
			else if (mouvement == Camera_Movement::BACKWARD) {
				return QWERTY::BACKWARD;
			} else if (mouvement == Camera_Movement::LEFT) {
				return QWERTY::LEFT;

			} else if (mouvement == Camera_Movement::RIGHT) {
				return QWERTY::RIGHT;
			}
	}
}