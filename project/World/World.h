#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class World {
    private:
        // TODO linked list of Object3D instantiated

    public:
        void addObject(Object3D new_object) {
            // TODO add new_object to LL
        }

        void removeObject(Object3D target_object) {
            // TODO remove target_object from LL
        }

        void Init() {
            // TODO call the Init method of every Object3D in the LL
        }

        void Draw() {
            // TODO call the Draw method of every Object3D in the LL
        }

        void Cleanup() {
            // TODO call the Cleanup method of every Object3D in the LL
        }
};