#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

typedef struct {
    Terrain* terrain;
    Water* water;
} Chunk;

class WorldInstance: public World {
    private:
        Camera* camera;
        Camera* camera2;

        Minimap* minimapBg;
        LiveViewer* minimap;

        Terrain* terrain;
        Water* water;

    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            camera = getCamera();
            camera2 = instantiate(new Camera(45.0f, 1.0f, 0.1f, 100.0f));

            camera->translate(vec3(0.0f, 0.0f, 8.0f));
            camera->scale(vec3(-0.2f, -0.2f, -0.2f));
            
            camera2->rotate(vec3(-90.0f, 0.0f, 0.0f));
            camera2->getTransform()->setPosition(camera->getTransform()->getPosition());
            camera2->translate(vec3(0.0f, 3.0f, 0.0f));

            terrain = instantiate(new Terrain());
            water = instantiate(new Water());

            setupMinimap();
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

            // make minimap camera follow the main camera
            Transform* camera_transform = camera->getTransform();
            Transform* camera2_transform = camera2->getTransform();
            vec3 camera2_rotation = camera2_transform->getRotation();
            camera2_transform->setPosition(camera_transform->getPosition() + vec3(0.0f, 3.0f, 0.0f));
            camera2_transform->setRotation(vec3(camera2_rotation.x, camera2_rotation.y, camera_transform->getRotation().y));
        }

        void setupMinimap() {
            enableLiveRenderer(camera2);
            minimapBg = instantiate2D(new Minimap());
            minimap = instantiate2D(new LiveViewer(camera2->getRenderTextureID()));

            minimap->rotate(vec3(90.0f, 0.0f, 0.0f));
            minimap->translate(vec3(-0.75f, 0.75f, 0.0f));
            minimap->scale(vec3(-0.79f, 0.0f, -0.79f));

            minimapBg->rotate(vec3(90.0f, 0.0f, 0.0f));
            minimapBg->translate(vec3(-0.75f, 0.75f, 0.1f));
            minimapBg->scale(vec3(-0.75f, 0.0f, -0.75f));
        }
};