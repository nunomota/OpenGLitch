#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

typedef struct {
    Terrain* terrain;
    Water* water;
} Chunk;

/** The index of a chunk in the array
  * also represents its relative
  * position according to all others:
  *
  *    [2, 3]
  *    [0, 1]
  *
  * This means that their indexes will 
  * change based on camera movement.
  */
struct {
    float chunk_width = 2.0f;
    Chunk chunks[4];
    int visible_chunk;
    int last_quadrant;
} infinite_terrain;

class WorldInstance: public World {
    private:
        Camera* camera;
        Camera* camera2;

        Minimap* minimapBg;
        LiveViewer* minimap;

    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            camera = getCamera();
            camera2 = instantiate(new Camera(45.0f, 1.0f, 0.1f, 100.0f));

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

            updateInfiniteTerrain();

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

        void setupInfiniteTerrain() {

            vec3 camera_position = camera->getTransform()->getPosition();
            vec3 bl_chunk_position = vec3(camera_position.x, 0.0f, camera_position.z);
            vec3 forward_vector = vec3(0.0f, 0.0f, -1.0f) * infinite_terrain.chunk_width;
            vec3 right_vector = vec3(1.0f, 0.0f, 0.0f) * infinite_terrain.chunk_width;

            // instantiate all chunks
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    Terrain* new_terrain = instantiate(new Terrain());
                    Transform* terrain_transform = new_terrain->getTransform();
                    Water* new_water = instantiate(new Water());

                    terrain_transform->setPosition(bl_chunk_position + (float)i*forward_vector + (float)j*right_vector);
                    new_water->getTransform()->setPosition(terrain_transform->getPosition());

                    infinite_terrain.chunks[i*2 + j].terrain = new_terrain;
                    infinite_terrain.chunks[i*2 + j].water   = new_water;
                }
            }
            infinite_terrain.visible_chunk = 0;
            infinite_terrain.last_quadrant = getQuadrant();
        }

        void updateInfiniteTerrain() {
            int cur_quadrant = getQuadrant();
            if (cur_quadrant != -1) {
                if (cur_quadrant != infinite_terrain.last_quadrant) {
                    regroupChunks(infinite_terrain.last_quadrant, cur_quadrant);
                    infinite_terrain.last_quadrant = cur_quadrant;
                }
            } else {
                // TODO assign new visible chunk
            }
        }

        int getQuadrant() {
            float chunk_width = infinite_terrain.chunk_width;
            vec3 camera_position = camera->getTransform()->getPosition();
            vec3 chunk_position  = infinite_terrain.chunks[infinite_terrain.visible_chunk].terrain->getTransform()->getPosition();
            
            if (inBounds(camera_position.x, chunk_position.x, chunk_position.x + chunk_width/2.0f) &&
                inBounds(camera_position.z, chunk_position.z, chunk_position.z + chunk_width/2.0f)) {
                return 1;
            } else if (inBounds(camera_position.x, chunk_position.x - chunk_width/2.0f, chunk_position.x) &&
                       inBounds(camera_position.z, chunk_position.z, chunk_position.z + chunk_width/2.0f)) {
                return 0;
            } else if (inBounds(camera_position.x, chunk_position.x, chunk_position.x + chunk_width/2.0f) &&
                       inBounds(camera_position.z, chunk_position.z - chunk_width/2.0f, chunk_position.z)) {
                return 3;
            } else if (inBounds(camera_position.x, chunk_position.x - chunk_width/2.0f, chunk_position.x) &&
                       inBounds(camera_position.z, chunk_position.z - chunk_width/2.0f, chunk_position.z)) {
                return 2;
            } else {
                return -1;
            }
        }

        bool inBounds(float target, float min, float max) {
            if (target >= min && target <= max) {
                return true;
            }
            return false;
        }

        void regroupChunks(int last, int current) {
            vec2 mov_vector = (quadToCoords(current) - quadToCoords(last));
        }

        vec2 quadToCoords(int quadrant) {
            if (quadrant == 0) {
                return vec2(0, 0);
            } else if (quadrant == 1) {
                return vec2(1, 0);
            } else if (quadrant == 2) {
                return vec2(0, 1);
            } else {
                return vec2(1, 1);
            }
        }
};