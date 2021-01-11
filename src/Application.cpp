#include "Application.h"
#include "Log.h"
#include <chrono>
#include "Texture.h"
#include "FrameBuffer.h"
#include "Model.h";
#include <cmath>
#include <iostream>
#include <btBulletCollisionCommon.h>
#include "Objects/Object.h"
#include <glm/gtx/string_cast.hpp>
#include <algorithm>    // std::max

namespace GEngine {

    Application* Application::s_instance = nullptr;




    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


    Application::Application(const std::string& name){
        GE_CORE_ASSERT(!s_instance, "Application already exists!");
        Application::s_instance = this;
        m_window = new Window(name, SCR_WIDTH, SCR_HEIGHT, true);
        m_camera = new Camera();
        m_window->attach(*m_camera);
        m_projection = new PerspectiveProjection(fov, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        m_renderer = new Renderer();
        m_renderer->init();


    }

    Application::~Application(){


    }

    void Application::launchBall() {
        PhBall* ball = balls.at(selectedBallIndex);
        ball->launch(100.0f, glm::vec3(0.0f, 0.0f, -1.0f));


    }

    void Application::moveBall(std::string direction) {
        PhBall* ball = balls.at(selectedBallIndex);
        ball->move(direction);

    }

    void Application::resetBall() {
        PhBall* ball = balls.at(0);
        ball->reset();
    }

    void Application::resetAll() {
        selectedBallIndex = 0;
        for (auto ball : balls) {
            ball->reset();
        }

        for (auto pin : phConvexObjects) {
            pin->reset();
        }
    }

    void Application::selectedBall(int index) {
        selectedBallIndex = index;
    }


    void Application::run(){

        Emitter* emitter = new Emitter(1000);
        std::string file_base = "";


        using namespace std::chrono;


        Model* sceneModel = new Model(file_base + std::string("./Objects/scene/scene.obj"));
        if (sceneModel->getLoaded() == 1) {  //check if path is correct. If not then end the function
            return; 
        }
        Model* ballModel = new Model(file_base + std::string("./Objects/ball/ball_final.obj"));
        Model* runwayModel = new Model(file_base + std::string("./Objects/runway/untitled.obj"));
        Model* lampModel = new Model(file_base + std::string("./Objects/lamp/lamp_only.obj"));
        Model* lightModel = new Model(file_base + std::string("./Objects/light/light.obj"));
        Model* top_model = new Model(file_base + std::string("./Objects/top/top.obj"));

      
         
         Model* convexHullModel = new Model(file_base + std::string("./Objects/pin/pinConvexhull.obj"), true);
         Model* pin = new Model(file_base + std::string("./Objects/pin/pin_final.obj"));

         ///////set vectors for objects ////////////
         std::vector<Object*> convexPinObjects;
         std::vector<Object*> pinObjects;
         std::vector<glm::vec3> pinPositions{ glm::vec3(-35.0f, 1.81f, -45.0f),
                                              glm::vec3(-33.0f, 1.81f, -46.8f), glm::vec3(-37.0f, 1.81f, -46.8f),
                                              glm::vec3(-31.5f, 1.81f, -48.3f), glm::vec3(-35.0f, 1.81f, -48.3f), glm::vec3(-38.5f, 1.81f, -48.3f),
                                              glm::vec3(-30.0f, 1.81f, -49.8f), glm::vec3(-33.0f, 1.81f, -49.8f), glm::vec3(-37.0f, 1.81f, -49.8f), glm::vec3(-40.0f, 1.81f, -49.8f)};




         Physics* physics = new Physics();
       //  physics->addGroundSurface();

         Object* ballObject1 = new Object(glm::vec3(-35.0f, 7.0f, 47.0f), glm::vec3(0.0f), glm::vec3(1.0f), ballModel);  
         Object* ballObject2 = new Object(glm::vec3(0.0f, 7.0f, 47.0f), glm::vec3(0.0f), glm::vec3(1.0f), ballModel);
         Object* ballObject3 = new Object(glm::vec3(35.0f, 7.0f, 47.0f), glm::vec3(0.0f), glm::vec3(1.0f), ballModel);
         PhBall* phBall1 = new PhBall(physics, ballObject1, 2.0f, 1.15f);
         PhBall* phBall2 = new PhBall(physics, ballObject2, 2.0f, 1.15f);
         PhBall* phBall3 = new PhBall(physics, ballObject3, 2.0f, 1.15f);

         balls.push_back(phBall1);
         balls.push_back(phBall2);
         balls.push_back(phBall3);

         for (glm::vec3 pos : pinPositions) {
             for (int i = 0; i < 3; i++) {
                 Object* convexPinObject = new Object(pos + glm::vec3(i*35.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), convexHullModel);
                 Object* pinObject = new Object(convexPinObject->getPosition(), convexPinObject->getOrientation(), convexPinObject->getScale(), pin);
                 convexPinObject->setChildObject(pinObject);
                 PhObject* phConvexObject = new PhObject(convexPinObject, 1.0f, "pin");
                 convexPinObjects.push_back(convexPinObject);
                 pinObjects.push_back(pinObject);
                 phConvexObjects.push_back(phConvexObject);
                 physics->addDynamicModel(phConvexObject);
             }

         }

         emitter->update(m_camera->getDeltaTime(), ballObject1);

         Object* runwayObject1 = new Object(glm::vec3(-35.0f, 0.5f, -48.0f), glm::vec3(0.0f), glm::vec3(0.8f), runwayModel);
         Object* runwayObject2 = new Object(glm::vec3(0.0f, 0.5f, -48.0f), glm::vec3(0.0f), glm::vec3(0.8f), runwayModel);
         Object* runwayObject3 = new Object(glm::vec3(35.0f, 0.5f, -48.0f), glm::vec3(0.0f), glm::vec3(0.8f), runwayModel);

         PhObject* phRunway1 = new PhObject(runwayObject1, 0.0f);
         PhObject* phRunway2 = new PhObject(runwayObject2, 0.0f);
         PhObject* phRunway3 = new PhObject(runwayObject3, 0.0f);


         Object* room  = new Object(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.5f), sceneModel);
         PhObject* phRoom = new PhObject(room, 0.0f);


         physics->addStaticModel(phRunway1);
         physics->addStaticModel(phRunway2);
         physics->addStaticModel(phRunway3);
         physics->addStaticModel(phRoom);

        const unsigned int FRAME_WIDTH = 1024, FRAME_HEIGHT = 1024;


        ////////////framebuffer for the reflection on the top ////////////////
        FrameBuffer frameBuffer = FrameBuffer(FRAME_WIDTH, FRAME_HEIGHT, "color");


        /////////framebuffers for the shadows calculations. 1 framebuffer per 1 light source///////////
        std::vector<FrameBuffer*> frameBuffers;
        for (int i = 0; i < 6; i++) {
            frameBuffers.push_back(new FrameBuffer(FRAME_WIDTH, FRAME_HEIGHT, "depth"));
        }



        ////////SHADERS/////////

        const std::string cubeMapColorPathVs = file_base + std::string("./Shaders/cubeMap.vs");
        const std::string cubeMapColorPathFs = file_base + std::string("./Shaders/cubeMap.fs");
        const std::string cubeMapColorPathGs = file_base + std::string("./Shaders/cubeMap.gs");

        const std::string modelShadowsShaderVs = file_base + std::string("./Shaders/Shadows/model_shadows.vs");
        const std::string modelShadowsShaderFs = file_base + std::string("./Shaders/Shadows/model_shadows.fs");
        const std::string modelShadowsShaderGs = file_base + std::string("./Shaders/Shadows/model_shadows.gs");

        const std::string lightShaderVs = file_base + std::string("./Shaders/light/lightShader.vs");
        const std::string lightShaderFs = file_base + std::string("./Shaders/light/lightShader.fs");
        const std::string lightShaderGs = file_base + std::string("./Shaders/light/lightShader.gs");


        const std::string shaderParticleVs = file_base + std::string("./Shaders/Particle/particle.vs");
        const std::string shaderParticleFs = file_base + std::string("./Shaders/Particle/particle.fs");

        const std::string modelLoaderShaderVs = file_base + std::string("./Shaders/model_loader.vs");
        const std::string modelLoaderShaderFs = file_base + std::string("./Shaders/model_loader.fs");
        const std::string runwayLoaderShaderGs = file_base + std::string("./Shaders/model_loader.gs");

        const std::string lampLoaderShaderGs = file_base + std::string("./Shaders/lamp_loader.gs");

        const std::string skyboxShaderVs = file_base + std::string("./Shaders/skybox/skybox.vs");
        const std::string skyboxShaderFs = file_base + std::string("./Shaders/skybox/skybox.fs");


        const std::string modelTopShaderVs = file_base + std::string("./Shaders/cube.vs");
        const std::string modelTopShaderFs = file_base + std::string("./Shaders/cube.fs");

        Shader* cubeMapColor = new Shader(cubeMapColorPathVs.c_str(), cubeMapColorPathFs.c_str(), cubeMapColorPathGs.c_str());
        Shader* modelShadowsShader = new Shader(modelShadowsShaderVs.c_str() , modelShadowsShaderFs.c_str(), modelShadowsShaderGs.c_str());
        Shader* lightShader = new Shader(lightShaderVs.c_str(), lightShaderFs.c_str(), lightShaderGs.c_str());
        Shader* shaderParticle = new Shader(shaderParticleVs.c_str(), shaderParticleFs.c_str());

        Shader* modelLoaderShader = new Shader(modelLoaderShaderVs.c_str(), modelLoaderShaderFs.c_str());
        Shader* runwayLoaderShader = new Shader(modelLoaderShaderVs.c_str(), modelLoaderShaderFs.c_str(), runwayLoaderShaderGs.c_str());
        Shader* lampLoaderShader = new Shader(modelLoaderShaderVs.c_str(), modelLoaderShaderFs.c_str(), lampLoaderShaderGs.c_str());


        Shader* skyboxShader = new Shader(skyboxShaderVs.c_str(), skyboxShaderFs.c_str());

        Shader* modelTopShader = new Shader(modelTopShaderVs.c_str(), modelTopShaderFs.c_str());

        /////////////////////////////

        std::vector<Shader*> shaders;
        shaders.push_back(modelLoaderShader);
        shaders.push_back(runwayLoaderShader);
        shaders.push_back(lampLoaderShader);


    
        // lights
        glm::vec3 lightPositions[] = {
            glm::vec3(-25.0f, 20.0f, 40.0f),
            glm::vec3(0.0f, 20.0f, 40.0f),
            glm::vec3(25.0f, 20.0f, 40.0f),
            glm::vec3(-25.0f, 20.0f, -40.0f),
            glm::vec3(0.0f, 20.0f, -40.0f),
            glm::vec3(25.0f, 20.0f, -40.0f)
        };



        glm::vec3 lightColors[] = {
            glm::vec3(255.0f, 255.0f, 255.0f),
            glm::vec3(255.0f, 255.0f, 255.0f),
            glm::vec3(255.0f, 255.0f, 255.0f),
            glm::vec3(255.0f, 255.0f, 255.0f),
            glm::vec3(255.0f, 255.0f, 255.0f),
            glm::vec3(255.0f, 255.0f, 255.0f)

        };

        float near_plane = 1.0f;
        float far_plane = 500.0f;

        for (Shader* shader : shaders) {
            shader->bind();
            shader->setInt("u_DepthMap[0]", 0);
            shader->setInt("u_DepthMap[1]", 1);
            shader->setInt("u_DepthMap[2]", 2);
            shader->setInt("u_DepthMap[3]", 3);
            shader->setInt("u_DepthMap[4]", 4);
            shader->setInt("u_DepthMap[5]", 5);
            shader->setInt("texture_albedo1", 6);
            shader->setInt("texture_normal1", 7);
            shader->setInt("texture_metallic1", 8);
            shader->setInt("texture_roughness1", 9);
            shader->setInt("texture_ao1", 10);
            shader->setOffset(6);

            for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
                shader->setFloat3("u_LightPositions[" + std::to_string(i) + "]", lightPositions[i]);
                shader->setFloat3("u_LightColors[" + std::to_string(i) + "]", lightColors[i]);
            }
            shader->bind();
            shader->setFloat("u_FarPlane", far_plane);
            shader->unbind();
        }



        /////////////////////skybox//////////////////////

        std::vector<std::string> faces{ file_base + std::string("./Objects/skybox/right.jpg") ,
                file_base + std::string("./Objects/skybox/left.jpg"),
                file_base + std::string("./Objects/skybox/top.jpg"),
                file_base + std::string("./Objects/skybox/bottom.jpg"),
                file_base + std::string("./Objects/skybox/front.jpg"),
                file_base + std::string("./Objects/skybox/back.jpg") };
        CubeMapTexture* skybox = new CubeMapTexture(faces);
        


        skyboxShader->bind();
        skyboxShader->setInt("skybox", 0);
        skyboxShader->unbind();

        VertexArray* vertexArray_skybox = new VertexArray();
        VertexBuffer* vertexBuffer_skybox = new VertexBuffer(skyboxVertices, sizeof(skyboxVertices), 36);

        VtxBufLayout bufferlayout_skybox = { {ShaderDataType::Vec3, "a_Position" } };

        vertexBuffer_skybox->setLayout(bufferlayout_skybox);
        vertexArray_skybox->addVertexBuffer(*vertexBuffer_skybox);



        /////////////////////////////////////////////////

        modelTopShader->bind();
        modelTopShader->setInt("u_Room", 0);
        modelTopShader->setOffset(1);
        modelTopShader->unbind();



        while (m_running) {
           // m_renderer->setClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
            m_renderer->setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
            m_renderer->clear();
            m_renderer->beginScene(*m_camera, *m_projection);
            m_renderer->setDeltaTime();

            for (Shader* shader : shaders) {
                shader->bind();
                shader->setInt("u_ShadowsEnable", shadowsEnabled);
            }
        glm::vec3 cubeMapPos(ceil(m_camera->getPosition().x), 44.5f, ceil(m_camera->getPosition().z));
       // std::cout << m_camera->getPosition().x << " " << m_camera->getPosition().z << std::endl;
        PerspectiveProjection projection = PerspectiveProjection(90.0f, (float)FRAME_WIDTH / (float)FRAME_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 cubeMapProj = projection.getProjectionMatrix();
        std::vector<glm::mat4> cubeMap;
        cubeMap.push_back(cubeMapProj* glm::lookAt(cubeMapPos, cubeMapPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        cubeMap.push_back(cubeMapProj* glm::lookAt(cubeMapPos, cubeMapPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        cubeMap.push_back(cubeMapProj* glm::lookAt(cubeMapPos, cubeMapPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        cubeMap.push_back(cubeMapProj* glm::lookAt(cubeMapPos, cubeMapPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
        cubeMap.push_back(cubeMapProj* glm::lookAt(cubeMapPos, cubeMapPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
        cubeMap.push_back(cubeMapProj* glm::lookAt(cubeMapPos, cubeMapPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down

        glm::mat4 modelTopPos = glm::mat4(1.0f);
        modelTopPos = glm::translate(modelTopPos, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        modelTopPos = glm::scale(modelTopPos, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
 
        glm::mat4 lampPos= glm::mat4(1.0f);
        lampPos = glm::translate(lampPos, glm::vec3(-25.0f, 35.0f, 40.0f)); // translate it down so it's at the center of the scene
        lampPos = glm::scale(lampPos, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down

        glm::mat4 model_runway = glm::mat4(1.0f);
        model_runway = glm::translate(model_runway, glm::vec3(-35.0f, 0.5f, -45.0f)); // translate it down so it's at the center of the scene
        model_runway = glm::scale(model_runway, glm::vec3(10.0f, 10.0f, 10.0f));	// it's a bit too big for our scene, so scale it down


        for (int j = 0; j < 6; j++) {
            glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)FRAME_WIDTH / (float)FRAME_HEIGHT, near_plane, far_plane);
            std::vector<glm::mat4> shadowTransforms;
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[j], lightPositions[j] + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[j], lightPositions[j] + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[j], lightPositions[j] + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[j], lightPositions[j] + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[j], lightPositions[j] + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
            shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositions[j], lightPositions[j] + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

            FrameBuffer* frameBuffer = frameBuffers[j];
            m_renderer->setViewPort(0, 0, FRAME_WIDTH, FRAME_HEIGHT);

            frameBuffer->bind();
            m_renderer->clear();
            modelShadowsShader->bind();
            for (unsigned int i = 0; i < 6; ++i) {
                modelShadowsShader->setMat4("u_ShadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
            }
            modelShadowsShader->setFloat("f_FarPlane", far_plane);
            modelShadowsShader->setFloat3("v_LightPos", lightPositions[j]);

            m_renderer->drawModel(*modelShadowsShader, *sceneModel, room->getObjectMat());
            m_renderer->drawModel(*modelShadowsShader, *runwayModel, runwayObject1->getObjectMat());
            m_renderer->drawModel(*modelShadowsShader, *runwayModel, runwayObject2->getObjectMat());
            m_renderer->drawModel(*modelShadowsShader, *runwayModel, runwayObject3->getObjectMat());
            frameBuffer->unbind();
        }

       /* m_renderer->submit(*shader_light, *vertexArray_light, lampPos);
        m_renderer->indexDraw(*vertexArray_light);
        showFPS();*/


        /////// render to the framebuffer whcih will be used for the reflection///////////
        m_renderer->setViewPort(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
        m_renderer->clear();

        glm::mat4 model_runway_frame = runwayObject1->getObjectMat();
        frameBuffer.bind();
        cubeMapColor->bind();
        for (unsigned int i = 0; i < 6; ++i) {
            cubeMapColor->setMat4("u_CubeMap[" + std::to_string(i) + "]", cubeMap[i]);
        }
        m_renderer->drawModel(*cubeMapColor, *sceneModel, model);
        m_renderer->drawModel(*cubeMapColor, *runwayModel, model_runway_frame);
        model_runway_frame = glm::translate(model_runway_frame, glm::vec3(44.0f, 0.0f, 0.0f));
        m_renderer->drawModel(*cubeMapColor, *runwayModel, model_runway_frame);
        model_runway_frame = glm::translate(model_runway_frame, glm::vec3(44.0f, 0.0f, 0.0f));
        m_renderer->drawModel(*cubeMapColor, *runwayModel, model_runway_frame);


        for (Object* pinObject : pinObjects) {
            m_renderer->drawModel(*cubeMapColor, *pin, pinObject->getObjectMat());
        }

        m_renderer->drawModel(*cubeMapColor, *ballModel, ballObject1->getObjectMat());
        m_renderer->drawModel(*cubeMapColor, *ballModel, ballObject2->getObjectMat());
        m_renderer->drawModel(*cubeMapColor, *ballModel, ballObject3->getObjectMat());

        frameBuffer.unbind();
        ////////////////////////////////////////////////////////

        m_renderer->setViewPort(0, 0, SCR_WIDTH, SCR_HEIGHT);
        m_renderer->clear();



        for (int i = 0; i < 6; i++) {
            frameBuffers[i]->getCubeMapTexture()->bind(i);
        }

        m_renderer->drawModel(*modelLoaderShader, *sceneModel, room->getObjectMat());
        m_renderer->drawModel(*runwayLoaderShader, *runwayModel, runwayObject1->getObjectMat());

        
        glm::mat4 particleModel = glm::mat4(1.0f);


        for (Object* pinObject : pinObjects) {
            m_renderer->drawModel(*modelLoaderShader, *pin, pinObject->getObjectMat());
        }

        m_renderer->drawModel(*lampLoaderShader, *lampModel, lampPos);
        m_renderer->drawModel(*lightShader, *lightModel, lampPos);
     
        m_renderer->drawModel(*modelLoaderShader, *ballModel, ballObject1->getObjectMat());
        m_renderer->drawModel(*modelLoaderShader, *ballModel, ballObject2->getObjectMat());
        m_renderer->drawModel(*modelLoaderShader, *ballModel, ballObject3->getObjectMat());



        ////////top model drawing/////////
        frameBuffer.getCubeMapTexture()->bind();
        m_renderer->drawModel(*modelTopShader, *top_model, modelTopPos);
        ////////////////////////////////////

        emitter->update(m_camera->getDeltaTime(), ballObject1);
        m_renderer->drawParticles(*shaderParticle, *emitter, *ballObject1, particleModel);
        physics->update(m_camera->getDeltaTime());
         


        // draw skybox as last
        m_renderer->drawSkyBox(*skyboxShader, *vertexArray_skybox, *skybox);
        m_running = !(m_window->onUpdate());
        }
    }

} 
