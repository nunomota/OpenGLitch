#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class WorldInstance: public World {
    private:
        Camera* camera;

        Mirror mirror;
        Mirror refraction;
        MinimapContainer minimap;

        Terrain* terrain;
        Water* water;
        Sky* sky;

        LiveViewer* reflection_texture;
        LiveViewer* refraction_texture;

        Controller controller;

        CustomObject* cube;

    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            camera = getCamera();
            getLight()->rotate(vec3(-45.0f, 0.0f, 0.0f));

            terrain = instantiate(new Terrain(getTime(), getLight(), getCamera()));
            camera->getTransform()->setPosition(terrain->getTransform()->getPosition());
            
            sky = instantiate(new Sky());
            sky->rotate(vec3(180.0f,0.0f,0.0f));
            sky->scale(vec3(40.0f,40.0f,40.0f));
            sky->getTransform()->setPosition(camera->getTransform()->getPosition());

            setupController();
            setupMirror();
            setupRefraction();
            setupMinimap();

            water = instantiate(new Water(mirror.getMirrorTextureID(), refraction.getMirrorTextureID(), getTime(), getLight(), getCamera()));

            reflection_texture = instantiate2D(new LiveViewer(mirror.getMirrorTextureID()));
            reflection_texture->rotate(vec3(90.0f, 0.0f, 0.0f));
            reflection_texture->translate(vec3(0.75f, 0.75f, 0.0f));
            reflection_texture->scale(vec3(-0.79f, 0.0f, -0.79f));

            refraction_texture = instantiate2D(new LiveViewer(refraction.getMirrorTextureID()));
            refraction_texture->rotate(vec3(90.0f, 0.0f, 0.0f));
            refraction_texture->translate(vec3(0.75f, 0.0f, 0.0f));
            refraction_texture->scale(vec3(-0.79f, 0.0f, -0.79f));

            cube = instantiate(new CustomObject("Cube.obj"));
            cube->getTransform()->setPosition(camera->getTransform()->getPosition());
        }

        // method called every frame
        void Update() {
            move();
            mirror.update();
            refraction.update();
            minimap.update();
            controller.update();

            vec3 camera_position = camera->getTransform()->getPosition();
            terrain->getTransform()->setPosition(vec3(camera_position.x, 0.0f, camera_position.z));
            water->getTransform()->setPosition(vec3(camera_position.x, 0.0f, camera_position.z));
            sky->getTransform()->setPosition(camera_position);
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
            mirror.setClipPlane(vec4(0.0f, 1.0f, 0.0f, 0.0f));
            mirror.setup();
        }

        void setupRefraction() {
            Camera* refraction_camera = instantiate(new Camera());
            enableLiveRenderer(refraction_camera);
            refraction.setMirrorCamera(refraction_camera);
            refraction.setTargetCamera(mirror.getMirrorCamera());
            refraction.setClipPlane(vec4(0.0f, -1.0f, 0.0f, 0.0f));
            refraction.setup();
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
            } else if (getKeyDown(Keyboard::S)) {
                controller.translate(-getCamera()->getTransform()->getForwardVector(), true);
            } else {
                controller.translate(getCamera()->getTransform()->getForwardVector(), false);   
            }
        }
};