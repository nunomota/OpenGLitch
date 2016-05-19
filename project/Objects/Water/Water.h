#pragma once

class Water: public Grid {
    private:
        GLuint reflection_texture_id;
        GLuint time_id;
        Time* time;

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
            GLuint texture = Loader::loadTexture("waterDUDV.png");
            addTexture(texture);
            addTexture(reflection_texture_id);
            time_id = glGetUniformLocation(program_id_, "time");
            // TODO setup time for animation
        }

        void UpdateUniforms() {
            glUniform1f(time_id, time->getCurrentTime());
            // TODO update time for animation
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    public:
        Water(GLuint mirror_texture_id, Time* new_time) {
            reflection_texture_id = mirror_texture_id;
            time = new_time;
        }
};