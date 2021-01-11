#include "Log.h"

#include <iostream>

#include "btBulletDynamicsCommon.h"
#include "Application.h"
#include "Physics/physics.h"



int main() {

    GEngine::Log::init();

    GE_CORE_WARN("Initilized Log");

    GEngine::Application* app = new GEngine::Application();
    app->run();

}