#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class WorldInstance: public World {
    private:
        Camera* camera;

        Mirror mirror;
        MinimapContainer minimap;

        Terrain* terrain;
        Water* water;

        Controller controller;

    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            camera = getCamera();
            getLight()->rotate(vec3(-45.0f, 0.0f, 0.0f));

            setupController();
            setupMirror();
            setupMinimap();

            terrain = instantiate(new Terrain(getTime(), getLight(), getCamera()));
            water = instantiate(new Water(mirror.getMirrorTextureID(), getTime(), getLight(), getCamera()));

            camera->getTransform()->setPosition(terrain->getTransform()->getPosition());
            camera->translate(vec3(0.0f, 0.5f, 0.0f));
        }

        // method called every frame
        void Update() {
            move();
            mirror.update();
            minimap.update();
            controller.update();
        }

        void setupController() {
            controller.setTarget(camera->getTransform());
            controller.setTime(getTime());
            controller.setTerrain(terrain);
        }

        void setupMirror() {
            Camera* mirror_camera = instantiate(new Camera());
            enableLiveRenderer(mirror_camera);
            mirror.setMirrorCamera(mirror_camera);
            mirror.setTargetCamera(camera);
            mirror.setup();
        }

        void setupMinimap() {
            Camera* viewer_camera = instantiate(new Camera());
            enableLiveRenderer(viewer_camera);
            minimap.setBackground(instantiate2D(new Minimap()));
            minimap.setViewer(viewer_camera, instantiate2D(new LiveViewer(viewer_camera->getRenderTextureID())));
            minimap.setTargetCamera(camera);
            minimap.setup();
        }

        void move() {
            // pitch up/down
            if (getKeyDown(Keyboard::Q)) {
                controller.translate(vec3(0.0f, 1.0f, 0.0f), true);
            } else if (getKeyDown(Keyboard::E)) {
                controller.translate(vec3(0.0f, -1.0f, 0.0f), true);
            } else {
                controller.translate(vec3(0.0f, 1.0f, 0.0f), false);
            }

            // sideways camera turn
            if (getKeyDown(Keyboard::D)) {
                getCamera()->rotate(vec3(0.0f, -90.0f, 0.0f) * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::A)) {
                getCamera()->rotate(vec3(0.0f, 90.0f, 0.0f) * getTime()->getDeltaTime());
            }

            // front/back camera movement
            if (getKeyDown(Keyboard::W)) {
                controller.translate(getCamera()->getTransform()->getForwardVector(), true);
                //getCamera()->translate(getCamera()->getTransform()->getForwardVector() * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::S)) {
                controller.translate(-getCamera()->getTransform()->getForwardVector(), true);
            } else {
                controller.translate(getCamera()->getTransform()->getForwardVector(), false);   
            }
        }
};