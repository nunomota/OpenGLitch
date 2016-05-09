#pragma once

class Water: public Grid {
    protected:
        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("water_vshader.glsl",
                                                  "water_fshader.glsl");
        }

        void SetupUniforms() {
            addTexture("water.tga");
            // TODO setup time for animation
        }

        void UpdateUniforms() {
            // TODO update time for animation
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
};