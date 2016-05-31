#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class WorldInstance: public World {
    private:
        Camera* camera;
        float camera_speed = 0.5f;

        Mirror mirror;
        Mirror refraction;
        //MinimapContainer minimap;

        Terrain* terrain;
        Water* water;
        Sky* sky;

        //LiveViewer* reflection_texture;
        //LiveViewer* refraction_texture;

        Controller controller;

        bool free_camera = true;

    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            camera = getCamera();
            getLight()->rotate(vec3(-45.0f, 0.0f, 0.0f));

            terrain = instantiate(new Terrain(getTime(), getLight(), getCamera()));
            //camera->getTransform()->setPosition(terrain->getTransform()->getPosition() + vec3(0.0f, 0.3f, 0.0f));
            camera->getTransform()->setPosition(vec3(0.12f, 0.04f, -0.04f));
            camera->getTransform()->setRotation(vec3(-4.8f, -874.54f, 0.0f));
            
            sky = instantiate(new Sky());
            sky->rotate(vec3(180.0f,0.0f,0.0f));
            sky->scale(vec3(40.0f,40.0f,40.0f));
            sky->getTransform()->setPosition(camera->getTransform()->getPosition());

            setupController();
            setupMirror();
            setupRefraction();
            //setupMinimap();

            water = instantiate(new Water(mirror.getMirrorTextureID(), refraction.getMirrorTextureID(), getTime(), getLight(), getCamera()));
            terrain->scale(terrain->getTransform()->getScale() * vec3(2.0f, 0.0f, 2.0f));
            water->scale(water->getTransform()->getScale() * vec3(2.0f, 0.0f, 2.0f));

            //reflection_texture = instantiate2D(new LiveViewer(mirror.getMirrorTextureID()));
            //reflection_texture->rotate(vec3(90.0f, 0.0f, 0.0f));
            //reflection_texture->translate(vec3(0.75f, 0.75f, 0.0f));
            //reflection_texture->scale(vec3(-0.79f, 0.0f, -0.79f));

            //refraction_texture = instantiate2D(new LiveViewer(refraction.getMirrorTextureID()));
            //refraction_texture->rotate(vec3(90.0f, 0.0f, 0.0f));
            //refraction_texture->translate(vec3(0.75f, 0.0f, 0.0f));
            //refraction_texture->scale(vec3(-0.79f, 0.0f, -0.79f));
        }

        // method called every frame
        void Update() {
            move();
            mirror.update();
            refraction.update();
            //minimap.update();
            if (!free_camera) controller.update();

            vec3 camera_position = camera->getTransform()->getPosition();
            terrain->getTransform()->setPosition(vec3(camera_position.x, 0.0f, camera_position.z));
            water->getTransform()->setPosition(vec3(camera_position.x, 0.0f, camera_position.z));
            sky->getTransform()->setPosition(camera_position);

            //getLight()->rotate(vec3(-90.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
            //getLight()->setColor(vec3(cos(getLight()->getTransform()->getRotation().x), 0.0f, 0.0f));
        }

        void setupController() {
            controller.setTarget(camera->getTransform());
            controller.setTime(getTime());
            controller.setTerrain(terrain);
            controller.setVelocity(camera_speed);
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

        /*void setupMinimap() {
            Camera* viewer_camera = instantiate(new Camera());
            enableLiveRenderer(viewer_camera);
            minimap.setBackground(instantiate2D(new Minimap()));
            minimap.setViewer(viewer_camera, instantiate2D(new LiveViewer(viewer_camera->getRenderTextureID())));
            minimap.setTargetCamera(camera);
            minimap.setup();
        }*/

        void move() {
            if (!free_camera) {
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

                if (getKeyDown(Keyboard::W)) {
                    getCamera()->rotate(vec3(-90.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
                } else if (getKeyDown(Keyboard::S)) {
                    getCamera()->rotate(vec3(90.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
                }

                // front/back camera movement
                if (getKeyDown(Keyboard::P)) {
                    controller.translate(getCamera()->getTransform()->getForwardVector(), true);
                } else if (getKeyDown(Keyboard::L)) {
                    controller.translate(-getCamera()->getTransform()->getForwardVector(), true);
                } else {
                    controller.translate(getCamera()->getTransform()->getForwardVector(), false);   
                }
            } else {
                // upwards camera turn
                if (getKeyDown(Keyboard::W)) {
                    getCamera()->rotate(vec3(-90.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
                } else if (getKeyDown(Keyboard::S)) {
                    getCamera()->rotate(vec3(90.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
                }

                // sideways camera turn
                if (getKeyDown(Keyboard::D)) {
                    getCamera()->rotate(vec3(0.0f, -90.0f, 0.0f) * getTime()->getDeltaTime());
                } else if (getKeyDown(Keyboard::A)) {
                    getCamera()->rotate(vec3(0.0f, 90.0f, 0.0f) * getTime()->getDeltaTime());
                }

                // front/back camera movement
                if (getKeyDown(Keyboard::P)) {
                    camera->translate(camera->getTransform()->getForwardVector() * camera_speed * getTime()->getDeltaTime());
                } else if (getKeyDown(Keyboard::L)) {
                    camera->translate(-camera->getTransform()->getForwardVector() * camera_speed * getTime()->getDeltaTime());
                }

                if (getKeyDown(Keyboard::O)) {
                    camera->translate(vec3(0.0f, 1.0f, 0.0f) * camera_speed * getTime()->getDeltaTime());
                } else if (getKeyDown(Keyboard::K)) {
                    camera->translate(vec3(0.0f, -1.0f, 0.0f) * camera_speed * getTime()->getDeltaTime());
                }
            }

            if (getKeyPressed(Keyboard::B)) {
                free_camera = !free_camera;
            }

            if (getKeyPressed(Keyboard::V)) {
                cout << getCamera()->getTransform()->to_string() << endl;
            }
        }
};