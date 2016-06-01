#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class WorldInstance: public World {
    private:
        Camera* camera;
        Camera* camera2;
        DirectionalLight* light;

        LiveViewer* live_viewer;
        LiveViewer* live_viewer2;

        Terrain* terra;
        Water* water;

        float multiplier = 1.0f;
        MinimapContainer minimap;
        //InfiniteTerrain infinite_terrain;

    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            camera = getCamera();
            light = getLight();
            camera2 = instantiate(new Camera());

            light->translate(vec3(0.5f,0.0f,1.0f));

            //infinite_terrain.setTarget(camera);
            Camera* shadow_camera = light->getShadowCamera();
            setupShadow();
            terra = instantiate(new Terrain(shadow_camera->getShadowTextureID())); 
            water = instantiate(new Water());

            camera->translate(vec3(0.0f, 1.0f, 0.0f));
            camera->scale(vec3(-0.2f, -0.2f, -0.2f));

            camera2->rotate(vec3(-90.0f, 0.0f, 0.0f));
            camera2->getTransform()->setPosition(camera->getTransform()->getPosition());
            camera2->translate(vec3(0.0f, 3.0f, 0.0f));

            setupMinimap();
            //setupInfiniteTerrain();
        }

        // method called every frame
        void Update() {
            // upward/downward camera turn
            if (getKeyDown(Keyboard::W)) {
                getCamera()->rotate(vec3(90.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::S)) {
                getCamera()->rotate(vec3(-90.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
            }

            // sideways camera turn
            if (getKeyDown(Keyboard::D)) {
                getCamera()->rotate(vec3(0.0f, -90.0f, 0.0f) * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::A)) {
                getCamera()->rotate(vec3(0.0f, 90.0f, 0.0f) * getTime()->getDeltaTime());
            }

            // front/back camera movement
            if (getKeyDown(Keyboard::P)) {
                getCamera()->translate(getCamera()->getTransform()->getForwardVector() * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::L)) {
                getCamera()->translate(-getCamera()->getTransform()->getForwardVector() * getTime()->getDeltaTime());
            }

            //light->translate(vec3(0.0f,0.0f,-0.2f) * getTime()->getDeltaTime());
            //vec3 light_position = light->getTransform()->getPosition();
            //light->getTransform()->setPosition(vec3(light_position.x, light_position.y + 0.001f/*cos(light_position.z * 5.0f) * 0.05f*/, light_position.z - 0.001f));
            updateShadow();
            minimap.update();
            //infinite_terrain.update();
        }

        void setupShadow() {
            Camera* shadow_camera = light->getShadowCamera();
            Transform* shadow_camera_transform =  shadow_camera->getTransform();
            shadow_camera_transform->setPosition(light->getTransform()->getPosition());
            shadow_camera_transform->setRotation(light->getTransform()->getRotation());
            enableLiveRenderer(shadow_camera);
            live_viewer = instantiate2D(new LiveViewer(shadow_camera->getRenderTextureID()));
            live_viewer->rotate(vec3(90.0f, 0.0f, 0.0f));
            live_viewer->translate(vec3(0.75f, 0.75f, 0.0f));
            live_viewer->scale(vec3(-0.79f, 0.0f, -0.79f));

            live_viewer2 = instantiate2D(new LiveViewer(shadow_camera->getShadowTextureID()));
            printf("%d\n",shadow_camera->getShadowTextureID());
            live_viewer2->rotate(vec3(90.0f, 0.0f, 0.0f));
            live_viewer2->translate(vec3(0.75f, 0.0f, 0.0f));
            live_viewer2->scale(vec3(-0.79f, 0.0f, -0.79f));
        }

        void setupMinimap() {
            enableLiveRenderer(camera2);
            minimap.setBackground(instantiate2D(new Minimap()));
            minimap.setViewer(camera2, instantiate2D(new LiveViewer(camera2->getRenderTextureID())));
            minimap.setTargetCamera(camera);
            minimap.setup();
        }

        void updateShadow() {
            vec3 light_position = light->getTransform()->getPosition();
            printf("first: %f, %f, %f \n", light_position.x, light_position.y, light_position.z);

            //light->translate(vec3(0.1f,0.0f,0.0f));

            light->getTransform()->setPosition(vec3(light_position.x, light_position.y + 0.01f/*cos(light_position.z * 5.0f) * 0.05f*/, light_position.z - 0.01f));

            Camera* shadow_camera = light->getShadowCamera();
            GlmStrings astring;
            cout<< "before: \n" << astring.create(shadow_camera->getViewMatrix()) << "\n" << endl;
            Transform* shadow_camera_transform =  shadow_camera->getTransform();


            vec3 cam_pos = shadow_camera_transform->getPosition();
            printf("first cam: %f, %f, %f \n", cam_pos.x, cam_pos.y, cam_pos.z);
            cout<< "cam before: \n" << astring.create(shadow_camera->getViewMatrix()) << "\n" << endl;

            shadow_camera_transform->setPosition(light->getTransform()->getPosition() * multiplier);
            //light->translate(vec3(0.0f,-0.01f * multiplier,-0.01f * multiplier));
            vec3 new_cam_pos =  shadow_camera_transform->getPosition();
            printf("second cam: %f, %f, %f \n", new_cam_pos.x, new_cam_pos.y, new_cam_pos.z);
            multiplier = multiplier + 0.001f;
            cout<< "cam after: \n" << astring.create(shadow_camera->getViewMatrix()) << "\n" << endl;

            //std::cout<<glm::to_string(light->getTransform()->getPosition())<<std::endl;
            vec3 pos = light->getTransform()->getPosition();
            printf("second: %f, %f, %f \n", pos.x, pos.y, pos.z);

            shadow_camera_transform->setRotation(light->getTransform()->getRotation());
        }
};