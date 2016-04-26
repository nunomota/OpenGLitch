#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class World {
    private:
        static std::vector<Object3D*> uninitialized;
        static std::vector<Object3D*> objects;
        static Camera* main_camera;

        static Time world_time;
        static bool are_objects_uninitialized;

        static void initializeObjects() {
            for (std::vector<Object3D*>::iterator it = uninitialized.begin(); it != uninitialized.end(); ++it) {
                Object3D* object = (*it);
                if(object) {
                    object->Init();
                    objects.push_back(object);
                }
            }
            uninitialized.clear();
            are_objects_uninitialized = false;
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
            are_objects_uninitialized = false;
        }
    protected:
        Object3D* instantiate(Object3D* new_object) {
            if (new_object) {
                uninitialized.push_back(new_object);
                are_objects_uninitialized = true;
            }
            return new_object;
        }

        void destroy(Object3D* target_object) {
            // TODO make sure memory is being 'destroyed', avoiding memory leaks
            uninitialized.erase(std::remove(uninitialized.begin(), uninitialized.end(), target_object), uninitialized.end());
            objects.erase(std::remove(objects.begin(), objects.end(), target_object), objects.end());
            if (uninitialized.empty()) are_objects_uninitialized = false;
        }

        void setCamera(Camera* camera) {
            if (camera) main_camera = camera;
        }

        Camera* getCamera() {
            return main_camera;
        }

        Time getTime() {
            return world_time;
        }

        virtual void Start() {};
        virtual void Update() {};

    public:
        void Init() {
            are_objects_uninitialized = false;
            world_time.Init();
            Start();
        }

        void Display() {
            if (are_objects_uninitialized) initializeObjects();
            world_time.Update();
            Update();
        }

        void Terminate() {
            cleanupObjects();
        }
};