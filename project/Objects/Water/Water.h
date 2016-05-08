#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Water: public Grid {
    protected:
        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("water_vshader.glsl",
                                                  "water_fshader.glsl");
        }

        void SetupUniforms() {
            setTexture("water.tga");
            // TODO setup time for animation
        }

        void UpdateUniforms() {
            // TODO update time for animation
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
};