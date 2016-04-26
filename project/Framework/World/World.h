#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class World {
    private:
        static std::vector<Object3D*> uninitialized;
        static std::vector<Object3D*> objects;
        static Camera* main_camera;

        static bool areObjectsUninitialized;

        static void initializeObjects() {
            for (std::vector<Object3D*>::iterator it = uninitialized.begin(); it != uninitialized.end(); ++it) {
                Object3D* object = (*it);
                if(object) {
                    object->Init();
                    objects.push_back(object);
                }
            }
            uninitialized.clear();
            areObjectsUninitialized = false;
        }

        static void drawObjects() {
            for (std::vector<Object3D*>::iterator it = objects.begin(); it != objects.end(); ++it) {
                Object3D* object = (*it);
                if(object) {
                    object->Draw(main_camera->getViewMatrix(), main_camera->getProjectionMatrix());
                }
            }
        }

        static void cleanupObjects() {
            for (std::vector<Object3D*>::iterator it = objects.begin(); it != objects.end(); ++it) {
                Object3D* object = (*it);
                if (object) object->Cleanup();
            }
            if (main_camera) main_camera->Cleanup();
            uninitialized.clear();
            objects.clear();
        }

    public:
        static void addObject(Object3D* new_object) {
            if (new_object) {
                uninitialized.push_back(new_object);
                areObjectsUninitialized = true;
            }
        }

        static void removeObject(Object3D* target_object) {
            // TODO remove target_object from LL
        }

        static void setCamera(Camera* camera) {
            if (camera) main_camera = camera;
        }

        static Camera* getCamera() {
            return main_camera;
        }

        static void Start() {
            areObjectsUninitialized = false;
        }

        static void Update() {
            if (areObjectsUninitialized) initializeObjects(); 
        }

        static void Terminate() {
            cleanupObjects();
        }
};