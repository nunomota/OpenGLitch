#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

#include "../Object3D/Object3D.h"
#include "../Camera/Camera.h"

class World {
    private:
        std::vector<Object3D> objects;
        Camera camera;

    public:
        static void addObject(Object3D new_object) {
            if (new_object != NULL) objects3D.pushBack(new_object);
        }

        static void removeObject(Object3D target_object) {
            // TODO remove target_object from LL
        }

        static void Init() {
            for (std::vector<Object3D>::iterator it = objects.begin(); it != objects.end(); ++it) {
                it->Init();
            }
        }

        static void Draw() {
            for (std::vector<Object3D>::iterator it = objects.begin(); it != objects.end(); ++it) {
                // TODO get projection view matrix from cameraand send as parameters
                it->Draw();
            }
        }

        static void Cleanup() {
            for (std::vector<Object3D>::iterator it = objects.begin(); it != objects.end(); ++it) {
                it->Cleanup();
            }
        }
};