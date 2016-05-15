#pragma once

class Minimap: public Plane {
    protected:
        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("water_vshader.glsl",
                                                  "water_fshader.glsl");
        }

        void SetupUniforms() {
            GLuint texture = Loader::loadTexture("minimap.png");
            addTexture(texture);
        }
};