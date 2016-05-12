#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class WorldInstance: public World {
    private:
        Camera* camera;
        Camera* camera2;
        Terrain* terrain;
        Water* water;

    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            camera = getCamera();
            camera2 = instantiate(new Camera(45.0f, 1.0f, 0.1f, 100.0f));

            terrain = instantiate(new Terrain());
            water = instantiate(new Water());

            camera->translate(vec3(0.0f, 0.0f, 8.0f));
            camera->scale(vec3(-0.2f, -0.2f, -0.2f));
            
            //camera2->translate(vec3(0.0f, 3.0f, 0.0f));
            camera2->rotate(vec3(-90.0f, 0.0f, 0.0f));

            //terrain->rotate(vec3(90.0f, 0.0f, 0.0f));
            //water->translate(vec3(0.5f, 0.0f, 0.0f));
            //water->rotate(vec3(90.0f, 0.0f, 0.0f));
        }

        // method called every frame
        void Update() {
            // forward/backward camera movement
            if (getKeyDown(Keyboard::W)) {
                camera->translate(vec3(0.0f, 0.0f, -1.0f) * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::S)) {
                camera->translate(vec3(0.0f, 0.0f, 1.0f) * getTime()->getDeltaTime());
            }

            // sideways camera movement
            if (getKeyDown(Keyboard::D)) {
                camera->translate(vec3(1.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::A)) {
                camera->translate(vec3(-1.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
            }

            // upwards/downwards camera movement
            if (getKeyDown(Keyboard::P)) {
                camera->translate(vec3(0.0f, 1.0f, 0.0f) * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::L)) {
                camera->translate(vec3(0.0f, -1.0f, 0.0f) * getTime()->getDeltaTime());
            }

            if (getKeyPressed(Keyboard::Z)) {
                setCamera(camera2);
            }

            if (getKeyPressed(Keyboard::X)) {
                setCamera(camera);
            }
        }    
};