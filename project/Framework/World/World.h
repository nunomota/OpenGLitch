#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class World {
    private:
        Mouse mouse;
        Keyboard keyboard;

        std::vector<Object3D*> uninitialized;
        std::vector<Object3D*> objects;
        Camera* main_camera;

        Time world_time;
        bool are_objects_uninitialized;
        float camera_aspect_ratio;

        void initializeObjects() {
            Reporter::println("Objects being initialized", "World");
            for (std::vector<Object3D*>::iterator it = uninitialized.begin(); it != uninitialized.end(); ++it) {
                Object3D* object = (*it);
                if(object) {
                    object->Init();
                    objects.push_back(object);
                }
            }
            uninitialized.clear();
            are_objects_uninitialized = false;
            Reporter::println("All objects initialized", "World");
        }

        void drawObjects() {
            for (std::vector<Object3D*>::iterator it = objects.begin(); it != objects.end(); ++it) {
                Object3D* object = (*it);
                if(object && main_camera) {
                    if (object->getRenderer()->getState()) {
                        object->Draw(main_camera->getViewMatrix(), main_camera->getProjectionMatrix());
                    }
                }
            }
        }

        void cleanupObjects() {
            Reporter::println("Cleaning up objects", "World");
            for (std::vector<Object3D*>::iterator it = uninitialized.begin(); it != uninitialized.end(); ++it) {
                Object3D* object = (*it);
                if (object) {
                    delete object;
                }
            }
            for (std::vector<Object3D*>::iterator it = objects.begin(); it != objects.end(); ++it) {
                Object3D* object = (*it);
                if (object) {
                    object->Cleanup();
                    delete object;
                }
            }
            uninitialized.clear();
            objects.clear();
            are_objects_uninitialized = false;
            Reporter::println("All objects cleaned up", "World");
        }
    protected:
        template <typename Object>
        Object* instantiate(Object* new_object) {
            if (new_object) {
                uninitialized.push_back(new_object);
                are_objects_uninitialized = true;
            }
            return new_object;
        }

        void destroy(Object3D* target_object) {
            std::vector<Object3D*>::iterator uninitialized_delete_index = std::remove(uninitialized.begin(), uninitialized.end(), target_object);
            for (std::vector<Object3D*>::iterator it = uninitialized_delete_index; it != uninitialized.end(); ++it) {
                Object3D* object = (*it);
                if (object) {
                    object->Cleanup();
                    delete object;
                }
            }
            uninitialized.erase(uninitialized_delete_index, uninitialized.end());

            std::vector<Object3D*>::iterator objects_delete_index = std::remove(objects.begin(), objects.end(), target_object);
            for (std::vector<Object3D*>::iterator it = objects_delete_index; it != objects.end(); ++it) {
                Object3D* object = (*it);
                if (object) {
                    object->Cleanup();
                    delete object;
                }
            }
            objects.erase(objects_delete_index, objects.end());

            if (uninitialized.empty()) are_objects_uninitialized = false;
        }

        void setCamera(Camera* camera) {
            if (camera) {
                main_camera = camera;
                main_camera->setAspect(camera_aspect_ratio);
            }
        }

        Camera* getCamera() {
            return main_camera;
        }

        Time* getTime() {
            return &world_time;
        }

        bool getMouseButtonDown(int key) {
            return mouse.getDownState(key);
        }

        bool getMouseButtonPressed(int key) {
            return mouse.getPressedState(key, world_time.getCurrentTime());
        }

        bool getKeyDown(int key) {
            return keyboard.getDownState(key);
        }

        bool getKeyPressed(int key) {
            return keyboard.getPressedState(key, world_time.getCurrentTime());;
        }

        virtual void Start() {};
        virtual void Update() {};

    public:
        void Init() {
            are_objects_uninitialized = false;
            camera_aspect_ratio = 1.0f;
            world_time.Init();
            Start();
        }

        void Display() {
            if (are_objects_uninitialized) initializeObjects();
            drawObjects();
            world_time.Update();
            Update();
        }

        void Terminate() {
            cleanupObjects();
        }

        void setKeyState(int key, bool state) {
            keyboard.setState(key, state);
        }

        void setMouseButtonState(int key, bool state) {
            mouse.setState(key, state);
        }

        void setAspectRatio(float new_aspect) {
            if (new_aspect > 0.0f) {
                camera_aspect_ratio = new_aspect;
                if (main_camera) main_camera->setAspect(new_aspect);
            }
        }
};