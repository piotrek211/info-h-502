#pragma once


#include <glm/glm.hpp>
#include <string>
#include "cameraInput.h"

namespace GEngine {

	using inputCode = uint16_t;


	namespace Key {
		enum : inputCode {
			A = 65,
			D = 68,
			Q = 81,
			S = 83,
			W = 87,
			Z = 90
		};
	} 
	namespace Mouse {
		enum : inputCode {
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7,

			ButtonLast = Button7,
			ButtonLeft = Button0,
			ButtonRight = Button1,
			ButtonMiddle = Button2
		};
	}

	namespace QWERTY {
		enum : inputCode {
			FORWARD = Key::W,
			BACKWARD = Key::S,
			LEFT = Key::A,
			RIGHT = Key::D
		};
	}


	class Input {
		public:
			static bool isKeyPressed(const inputCode key);
			static bool isMouseButtonPressed(const inputCode button);
			static glm::vec2 getMousePosition();
			static float getMouseX();
			static float getMouseY();
			static inputCode getKey(const Camera_Movement mouvement);
			static Camera_Movement getMovement(int key);
			static bool escapePressed();
	};

}