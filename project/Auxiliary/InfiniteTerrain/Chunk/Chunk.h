#pragma once

class Chunk {
    public:
        Terrain* terrain;
        Water* water;

        Chunk(Terrain* new_terrain = NULL, Water* new_water = NULL) {
            terrain = new_terrain;
            water = new_water;
        }

        void translate(glm::vec3 t_vec) {
            terrain->translate(t_vec);
            water->translate(t_vec);
        }
};