#pragma once

#include <glad/glad.h>
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "cameraInput.h"
namespace GEngine {

	// Default camera values
	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 100.0f;
	const GLfloat SENSITIVTY = 0.25f;

	class Camera {
		public:

			// Constructor with vectors
			Camera(glm::vec3 position = glm::vec3(0.0f, 20.0f, 45.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY){
				this->position = position;
				this->worldUp = up;
				this->yaw = yaw;
				this->pitch = pitch;
				this->updateCameraVectors();
			}
			// Constructor with scalar values
			Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY){
				this->position = glm::vec3(posX, posY, posZ);
				this->worldUp = glm::vec3(upX, upY, upZ);
				this->yaw = yaw;
				this->pitch = pitch;
				this->updateCameraVectors();
			}


			void onKeyInput(Camera_Movement direction);
			void onMouseMovment(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
			void updateCameraVectors();
			void setDeltaTime(GLfloat deltaTime) {
				this->deltaTime = deltaTime;
			}
			GLfloat getDeltaTime() const {
				return deltaTime;
			}
			
			const glm::mat4 GetViewMatrix(){
				return glm::lookAt(this->position, this->position + this->front, this->up);  //position, target = position - direction (of the camera), up  // front = -direction
			}

			const glm::vec3& getPosition() {
				return position;
			}

		private:
			glm::mat4 viewMatrix;
			glm::vec3 position;
			glm::vec3 front;
			glm::vec3 up;
			glm::vec3 right;
			glm::vec3 worldUp;
			GLfloat yaw;
			GLfloat pitch;
			GLfloat roll;
			GLfloat movementSpeed;
			GLfloat mouseSensitivity;
			GLfloat deltaTime;





	};









}