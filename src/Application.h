#pragma once

#include "Window.h"
#include <string>
#include "Renderer.h"
#include "Camera.h"
#include "Projection.h"

#include "Buffer.h"
#include "VertexArray.h"
#include "Physics/phBall.h"
#include "Particle/Emitter.h"
namespace GEngine {
    // settings
    const unsigned int SCR_WIDTH = 1600;
    const unsigned int SCR_HEIGHT = 900;
    class Application {
        public:
            Application(const std::string& name = "GEngine");
            virtual ~Application();
            void run();
            Window* getWindow() { return m_window; }
            static Application& getApplication() { return *s_instance; }
            void onWindowClose() {};
            void onWindowResize() {};
            void setFov(float fov) {
                this->fov = fov;
                m_projection = new PerspectiveProjection(fov, (float) SCR_WIDTH /(float)SCR_HEIGHT, 0.1f, 1000.0f);
            }
            float getFov() { return fov; }

            void moveBall(std::string direction);
            void resetBall();
            void resetAll();
            void launchBall();
            void selectedBall(int index);
            void showFPS(void) {
                static double lastTime = glfwGetTime();
                static int nbFrames = 0;

                // Measure speed
                double currentTime = glfwGetTime();
                nbFrames++;
                if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
                                                     // printf and reset timer
                    std::cout << 1000.0 / double(nbFrames) << " ms/frame -> " << nbFrames << " frames/sec" << std::endl;
                    nbFrames = 0;
                    lastTime += 1.0;
                }
            }

            void enableShadows() {
                shadowsEnabled = shadowsEnabled ? 0.0 : 1.0;

            }

        private:
            bool m_running = true;
            static Application* s_instance;
            Window* m_window;
            Renderer* m_renderer;
            Camera* m_camera;
            Projection* m_projection;
            float fov = 45.0f;
            std::vector<PhBall*> balls;
            std::vector<PhObject*> phConvexObjects;
            int selectedBallIndex = 0;
            int shadowsEnabled = 1;
    };


}

