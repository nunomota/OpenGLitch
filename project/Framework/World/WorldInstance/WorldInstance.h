#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class WorldInstance: public World {
    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
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