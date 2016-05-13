#pragma once

class Chunk {
    public:
        Terrain* terrain;
        Water* water;

        void translate(glm::vec3 t_vec) {
            terrain->translate(t_vec);
            water->translate(t_vec);
        }
};