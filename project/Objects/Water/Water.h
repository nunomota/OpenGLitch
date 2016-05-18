#pragma once

class Water: public Grid {
    private:
        GLuint reflection_texture_id;

    protected:
        void InitialCalculations() {
            Grid::InitialCalculations();
            setDimension(16);
            setTag(1);
        }

        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("water_vshader.glsl",
                                                  "water_fshader.glsl");
        }

        void SetupUniforms() {
            GLuint texture = Loader::loadTexture("water.tga");
            addTexture(texture);
            addTexture(reflection_texture_id);
            // TODO setup time for animation
        }

        void UpdateUniforms() {
            // TODO update time for animation
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    public:
        Water(GLuint mirror_texture_id) {
            reflection_texture_id = mirror_texture_id;
        }
};