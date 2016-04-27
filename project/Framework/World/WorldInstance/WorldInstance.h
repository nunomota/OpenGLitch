#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class WorldInstance: public World {
    protected:

        // method called only once
        void Start() {

        }

        // method called every frame
        void Update() {
            if (getKeyPressed(0)) {
                Reporter::println("A was pressed", "Keyboard");
            }
        }    
};