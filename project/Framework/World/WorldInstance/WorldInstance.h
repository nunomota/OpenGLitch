#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class WorldInstance: public World {
    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            Cube* cube = instantiate(new Cube());
            (*cube).translate(glm::vec3(0.0f, 0.0f, -4.0f));
            (*cube).rotate(glm::vec3(0.0f, 45.0f, 45.0f));
        }

        // method called every frame
        void Update() {
            if (getKeyDown(Keyboard::A)) {
                Reporter::println("A is being pressed");
            }

            if (getKeyPressed(Keyboard::B)) {
                Reporter::println("B was pressed");
            }
        }    
};