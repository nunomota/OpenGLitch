#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class WorldInstance: public World {
    private:
        Camera* camera;
        Terrain* terrain;

    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            camera = getCamera();

            terrain = instantiate(new Terrain());

            camera->translate(vec3(0.0f, 0.0f, 8.0f));
            camera->scale(vec3(-0.2f, -0.2f, -0.2f));
        }

        // method called every frame
        void Update() {
            if (getKeyDown(Keyboard::W)) {
                camera->translate(vec3(0.0f, 0.0f, -1.0f) * getTime()->getDeltaTime());
            }

            if (getKeyDown(Keyboard::S)) {
                camera->translate(vec3(0.0f, 0.0f, 1.0f) * getTime()->getDeltaTime());
            }
        }    
};