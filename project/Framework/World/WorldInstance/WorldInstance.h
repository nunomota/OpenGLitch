#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class WorldInstance: public World {
    private:
        Camera* camera;
        Camera* camera2;
        Terrain* terrain;
        Water* water;

        LiveViewer* minimap;

    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            camera = getCamera();
            camera2 = instantiate(new Camera(45.0f, 1.0f, 0.1f, 100.0f));
            enableLiveRenderer(camera2);

            terrain = instantiate(new Terrain());
            water = instantiate(new Water());
            minimap = instantiate(new LiveViewer(camera2->getRenderTextureID()));

            camera->translate(vec3(0.0f, 0.0f, 8.0f));
            camera->scale(vec3(-0.2f, -0.2f, -0.2f));
            
            camera2->rotate(vec3(-90.0f, 0.0f, 0.0f));
            camera2->translate(vec3(0.0f, 0.0f, 3.0f));

            minimap->rotate(vec3(90.0f, 0.0f, 0.0f));
            minimap->translate(vec3(0.0f, 0.0f, 6.0f));
            minimap->scale(vec3(0.0f, 0.5f, 0.0f));
        }

        // method called every frame
        void Update() {
            // forward/backward camera movement
            if (getKeyDown(Keyboard::W)) {
                getCamera()->translate(vec3(0.0f, 0.0f, -1.0f) * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::S)) {
                getCamera()->translate(vec3(0.0f, 0.0f, 1.0f) * getTime()->getDeltaTime());
            }

            // sideways camera movement
            if (getKeyDown(Keyboard::D)) {
                getCamera()->translate(vec3(1.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::A)) {
                getCamera()->translate(vec3(-1.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
            }

            // upwards/downwards camera movement
            if (getKeyDown(Keyboard::P)) {
                getCamera()->translate(vec3(0.0f, 1.0f, 0.0f) * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::L)) {
                getCamera()->translate(vec3(0.0f, -1.0f, 0.0f) * getTime()->getDeltaTime());
            }

            if (getKeyPressed(Keyboard::Z)) {
                setCamera(camera2);
            }
        }    
};