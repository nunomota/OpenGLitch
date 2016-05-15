#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class WorldInstance: public World {
    private:
        Camera* camera;
        Camera* camera2;

        MinimapContainer minimap;
        InfiniteTerrain infinite_terrain;

    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            camera = getCamera();
            camera2 = instantiate(new Camera());

            infinite_terrain.setTarget(camera);

            camera->translate(vec3(0.0f, 1.0f, 0.0f));
            camera->scale(vec3(-0.2f, -0.2f, -0.2f));
            
            camera2->rotate(vec3(-90.0f, 0.0f, 0.0f));
            camera2->getTransform()->setPosition(camera->getTransform()->getPosition());
            camera2->translate(vec3(0.0f, 3.0f, 0.0f));

            setupMinimap();
            setupInfiniteTerrain();
        }

        // method called every frame
        void Update() {
            // upward/downward camera turn
            if (getKeyDown(Keyboard::W)) {
                getCamera()->rotate(vec3(90.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::S)) {
                getCamera()->rotate(vec3(-90.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
            }

            // sideways camera turn
            if (getKeyDown(Keyboard::D)) {
                getCamera()->rotate(vec3(0.0f, -90.0f, 0.0f) * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::A)) {
                getCamera()->rotate(vec3(0.0f, 90.0f, 0.0f) * getTime()->getDeltaTime());
            }

            // front/back camera movement
            if (getKeyDown(Keyboard::P)) {
                getCamera()->translate(getCamera()->getTransform()->getForwardVector() * getTime()->getDeltaTime());
            } else if (getKeyDown(Keyboard::L)) {
                getCamera()->translate(-getCamera()->getTransform()->getForwardVector() * getTime()->getDeltaTime());
            }

            minimap.update();
            infinite_terrain.update();
        }

        void setupMinimap() {
            enableLiveRenderer(camera2);
            minimap.setBackground(instantiate2D(new Minimap()));
            minimap.setViewer(camera2, instantiate2D(new LiveViewer(camera2->getRenderTextureID())));
            minimap.setTargetCamera(camera);
            minimap.setup();
        }

        void setupInfiniteTerrain() {
            for (int i = 0; i < 4; i++) {
                infinite_terrain.setChunk(i, Chunk(instantiate(new Terrain()), instantiate(new Water())));
            }
            infinite_terrain.initialize();
        }
};