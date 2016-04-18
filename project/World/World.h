#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class World {
    private:
        std::vector<Object3D> uninitialized;
        std::vector<Object3D> objects;
        Camera main_camera;

    public:
        void addObject(Object3D new_object) {
            if (new_object != NULL) uninitialized.pushBack(new_object);
        }

        void removeObject(Object3D target_object) {
            // TODO remove target_object from LL
        }

        void setCamera(Camera camera) {
            if (camera != NULL) main_camera = camera;
        }

        Camera getCamera() {
            return main_camera;
        }

        void Init() {
            for (std::vector<Object3D>::iterator it = uninitialized.begin(); it != uninitialized.end(); ++it) {
                it->Init();
                // TODO remove it from 'uninitialized' and add it to 'objects'
            }
        }

        void Draw() {
            for (std::vector<Object3D>::iterator it = objects.begin(); it != objects.end(); ++it) {
                it->Draw(main_camera.getViewMatrix(), main_camera.getProjectionMatrix());
            }
        }

        void Cleanup() {
            for (std::vector<Object3D>::iterator it = objects.begin(); it != objects.end(); ++it) {
                it->Cleanup();
            }
            main_camera.Cleanup();
        }
};