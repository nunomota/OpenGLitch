#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class World {
    private:
        Mouse mouse;
        Keyboard keyboard;

        std::vector<Object3D*> uninitialized;
        std::vector<Object3D*> objects;
        std::vector<Camera*> texture_rendering_cameras;
        Camera* main_camera;
        DirectionalLight* main_light;

        Time world_time;
        bool are_objects_uninitialized;
        int window_width, window_height;

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

        void drawRenderTextures() {
            for (std::vector<Camera*>::iterator c_it = texture_rendering_cameras.begin(); c_it != texture_rendering_cameras.end(); ++c_it) {
                Camera* camera = (*c_it);
                if (camera) {
                    camera->bindRenderBuffer();
                    for (std::vector<Object3D*>::iterator o_it = objects.begin(); o_it != objects.end(); ++o_it) {
                        Object3D* object = (*o_it);
                        if(object) {
                            if (object->getRenderer()->getState()) {
                                object->Draw(camera->getViewMatrix(), camera->getProjectionMatrix());
                            }
                        }
                    }
                    camera->unbindRenderBuffer();
                }
            }
        }

        /** WARNING!! This method does not eliminate the objects
          * inside the 'texture_rendering_cameras' vector. If
          * the cameras are instantiated like every other object
          * (AS THEY SHOULD) they will also be inside the 'objects'
          * vector and their memory will be freed.
          */
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
            texture_rendering_cameras.clear();
            are_objects_uninitialized = false;
            Reporter::println("All objects cleaned up", "World");
        }
    protected:
        /*--------------------
        | Core World methods |
        --------------------*/

        /** Method called to instantiate a new object
          * in the current World instance.
          *
          * Usage:       instantiate(new T(args))
          * Returns:     T* obj_ptr
          * Assumptions: T <: Object3D AND args represents
          *              arguments for class instantialization
          */
        template <typename Object>
        Object* instantiate(Object* new_object) {
            if (new_object) {
                uninitialized.push_back(new_object);
                are_objects_uninitialized = true;
            }
            return new_object;
        }

        /** Method called to start rendering a Camera's
          * view to a live texture.
          *
          * Usage:       enableLiveRenderer(camera_ptr)
          */
        void enableLiveRenderer(Camera* camera) {
            texture_rendering_cameras.push_back(camera);
            camera->setScreenDimensions(window_width, window_height);
        }

        /** Method called to destroy a previously instantiated
          * object in the current world instance.
          *
          * Usage:       destroy(obj_ptr)
          */
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

        /** Method called to set the main camera
          * in the current World instance.
          *
          * Usage:       setCamera(camera_ptr)
          */
        void setCamera(Camera* camera) {
            if (camera) {
                main_camera = camera;
                main_camera->setScreenDimensions(window_width, window_height);
            }
        }

        /** Method called to set the main light source
          * in the current World instance.
          *
          * Usage:       setLight(light_ptr)
          */
        void setLight(DirectionalLight* light) {
            if (light) {
                main_light = light;
            }
        }

        /*--------------------
        |   Input getters    |
        --------------------*/

        /** Method called to get state of
          * held mouse keys.
          * 
          * Usage:       getMouseButtonDown(Mouse::RIGHT)
          * Returns:     true, while Mouse::RIGHT is being held down
          */
        bool getMouseButtonDown(int key) {
            return mouse.getDownState(key);
        }

        /** Method called to get state of
          * pressed mouse keys.
          * 
          * Usage:       getMouseButtonPressed(Mouse::RIGHT)
          * Returns:     true, if Mouse::RIGHT is pressed
          */
        bool getMouseButtonPressed(int key) {
            return mouse.getPressedState(key, world_time.getCurrentTime());
        }

        /** Method called to get state of
          * held keyboard keys.
          * 
          * Usage:       getKeyDown(Keyboard::A)
          * Returns:     true, while Keyboard::A is being held down
          */
        bool getKeyDown(int key) {
            return keyboard.getDownState(key);
        }

        /** Method called to get state of
          * pressed keyboard keys.
          * 
          * Usage:       getKeyPressed(Keyboard::A)
          * Returns:     true, if Keyboard::A is pressed
          */
        bool getKeyPressed(int key) {
            return keyboard.getPressedState(key, world_time.getCurrentTime());;
        }

        /*--------------------
        |   Other getters    |
        --------------------*/
        DirectionalLight* getLight() {
            return main_light;
        }

        Camera* getCamera() {
            return main_camera;
        }

        Time* getTime() {
            return &world_time;
        }

        /*--------------------
        | life-cycle methods |
        --------------------*/
        virtual void Start() {};    // Called once, at the beggining of a world's life
        virtual void Update() {};   // Called every frame

    public:
        void Init() {
            are_objects_uninitialized = false;
            window_width = 600;
            window_height = 400;
            world_time.Init();
            Start();
        }

        void Display() {
            if (are_objects_uninitialized) initializeObjects();
            drawRenderTextures();
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

        void setWindowDimensions(int new_width, int new_height) {
            float new_aspect = (float)new_width / (float)new_height;
            if (new_width > 0 && new_height > 0) {
                window_width = new_width;
                window_height = new_height;
                if (main_camera) main_camera->setScreenDimensions(new_width, new_height);
                for (std::vector<Camera*>::iterator c_it = texture_rendering_cameras.begin(); c_it != texture_rendering_cameras.end(); ++c_it) {
                    Camera* camera = (*c_it);
                    if (camera) camera->setScreenDimensions(window_width, window_height);
                }
            }
        }
};