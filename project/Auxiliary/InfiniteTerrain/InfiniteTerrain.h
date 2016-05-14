#pragma once

using namespace glm;

class InfiniteTerrain {
    private:
        Camera* camera;
        float chunk_width;
        int visible_chunk;
        int last_quadrant;

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
        Chunk chunks[4];

        void initializeChunks() {
            vec3 camera_position = camera->getTransform()->getPosition();
            vec3 bl_chunk_position = vec3(camera_position.x, 0.0f, camera_position.z);
            vec3 forward_vector = vec3(0.0f, 0.0f, -1.0f) * chunk_width;
            vec3 right_vector = vec3(1.0f, 0.0f, 0.0f) * chunk_width;

            // organize
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    Terrain* terrain = chunks[i*2 + j].terrain;
                    Transform* terrain_transform = terrain->getTransform();
                    Water* water = chunks[i*2 + j].water;

                    terrain_transform->setPosition(bl_chunk_position + (float)i*forward_vector + (float)j*right_vector);
                    water->getTransform()->setPosition(terrain_transform->getPosition());
                }
            }
            visible_chunk = 0;
            last_quadrant = getQuadrant();
        }

        int getQuadrant() {
            vec3 camera_position = camera->getTransform()->getPosition();
            vec3 chunk_position  = chunks[visible_chunk].terrain->getTransform()->getPosition();
            
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
            if (target >= min && target < max) {
                return true;
            }
            return false;
        }


        void regroupChunks(int last, int current) {
            vec2 mov_vector = (quadToCoords(current) - quadToCoords(last));
            GlmStrings glm_strings;
            cout << glm_strings.create(vec3(mov_vector, 0.0f)) << endl;
            
            if (mov_vector.x == 1) {
                // right
                if (visible_chunk == 1 || visible_chunk == 3) {
                    chunks[0].translate(vec3(chunk_width*2.0f, 0.0f, 0.0f));
                    chunks[2].translate(vec3(chunk_width*2.0f, 0.0f, 0.0f));
                    chunkSwap(0, 1);
                    chunkSwap(2, 3);
                }
            } else if (mov_vector.x == -1) {
                // left
                if (visible_chunk == 0 || visible_chunk == 2) {
                    chunks[1].translate(vec3(chunk_width*-2.0f, 0.0f, 0.0f));
                    chunks[3].translate(vec3(chunk_width*-2.0f, 0.0f, 0.0f));
                    chunkSwap(0, 1);
                    chunkSwap(2, 3);
                }
            }

            if (mov_vector.y == 1) {
                // up
                if (visible_chunk == 2 || visible_chunk == 3) {
                    chunks[0].translate(vec3(0.0f, 0.0f, chunk_width*-2.0f));
                    chunks[1].translate(vec3(0.0f, 0.0f, chunk_width*-2.0f));
                    chunkSwap(0, 2);
                    chunkSwap(1, 3);
                }
            } else if (mov_vector.y == -1) {
                // down
                if (visible_chunk == 0 || visible_chunk == 1) {
                    chunks[2].translate(vec3(0.0f, 0.0f, chunk_width*2.0f));
                    chunks[3].translate(vec3(0.0f, 0.0f, chunk_width*2.0f));
                    chunkSwap(0, 2);
                    chunkSwap(1, 3);
                }
            }
        }

        void chunkSwap(int index1, int index2) {
            // save pointers at index1
            Terrain* temp_terrain = chunks[index1].terrain;
            Water* temp_water     = chunks[index1].water;

            // overwrite pointers at index1
            chunks[index1].terrain = chunks[index2].terrain;
            chunks[index1].water   = chunks[index2].water;

            // write pointers to index2
            chunks[index2].terrain = temp_terrain;
            chunks[index2].water   = temp_water;

            if (visible_chunk == index1) visible_chunk = index2;
            else if (visible_chunk == index2) visible_chunk = index1;
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

    public:
        InfiniteTerrain() {
            chunk_width = 2.0f;
        }

        void setTarget(Camera* target) {
            camera = target;
        }

        void setChunk(int index, Chunk chunk) {
            chunks[index] = chunk;
        }

        void initialize() {
            initializeChunks();
            cout << "In quadrant " << last_quadrant << endl;
        }

        void update() {
            int cur_quadrant = getQuadrant();
            if (cur_quadrant != last_quadrant) {
                cout << "Changed to quadrant " << cur_quadrant << endl;
                if (cur_quadrant == -1) {
                    // TODO assign new visible chunk
                } else {
                    regroupChunks(last_quadrant, cur_quadrant);
                }
                last_quadrant = cur_quadrant;
            }
        }
};