#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class World {
    private:
        static std::vector<Object3D*> uninitialized;
        static std::vector<Object3D*> objects;
        static Camera* main_camera;

    public:
        static void addObject(Object3D* new_object) {
            if (new_object != NULL) uninitialized.push_back(new_object);
        }

        static void removeObject(Object3D* target_object) {
            // TODO remove target_object from LL
        }

        static void setCamera(Camera* camera) {
            if (camera != NULL) main_camera = camera;
        }

        static Camera* getCamera() {
            return main_camera;
        }

        static void Init() {
            for (std::vector<Object3D*>::iterator it = uninitialized.begin(); it != uninitialized.end(); ++it) {
                Object3D* object = (*it);
                if(object != NULL) {
                    object->Init();
                    objects.push_back(object);
                }
            }
            uninitialized.clear();
        }

        static void Draw() {
            for (std::vector<Object3D*>::iterator it = objects.begin(); it != objects.end(); ++it) {
                Object3D* object = (*it);
                if(object != NULL) {
                    object->Draw(main_camera->getViewMatrix(), main_camera->getProjectionMatrix());
                }
            }
        }

        static void Cleanup() {
            for (std::vector<Object3D*>::iterator it = objects.begin(); it != objects.end(); ++it) {
                Object3D* object = (*it);
                if (object != NULL) object->Cleanup();
            }
            if (main_camera != NULL) main_camera->Cleanup();
        }
};