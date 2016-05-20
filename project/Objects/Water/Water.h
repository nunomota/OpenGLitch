#pragma once

class Water: public Grid {
    private:
        GLuint reflection_texture_id;
        GLuint time_id;

        Time* time;
        DirectionalLight* light;

        GLuint light_dir_id;
        GLuint light_pos_id;
        GLuint light_a_id;
        GLuint light_d_id;
        GLuint light_s_id;

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
            GLuint texture_dudv = Loader::loadTexture("waterDUDV.png");
            GLuint texture_normalmap = Loader::loadTexture("waterNormal.png");
            addTexture(texture_dudv);
            addTexture(reflection_texture_id);
            addTexture(texture_normalmap);
            time_id = glGetUniformLocation(program_id_, "time");

            light_dir_id = glGetUniformLocation(program_id_, "lightDirection");
            light_pos_id = glGetUniformLocation(program_id_, "lightPosition");
            light_a_id = glGetUniformLocation(program_id_, "La");
            light_d_id = glGetUniformLocation(program_id_, "Ld");
            light_s_id = glGetUniformLocation(program_id_, "Ls");

            glUniform3f(light_dir_id, light->getDirection());
            glUniform3f(light_pos_id, light->getTransform()->getPosition());
            glUniform3f(light_a_id, light->getAmbienceColor());
            glUniform3f(light_d_id, light->getDiffuseColor());
            glUniform3f(light_s_id, light->getSpecularColor());
            // TODO setup time for animation
        }

        void UpdateUniforms() {
            glUniform1f(time_id, time->getCurrentTime());
            // TODO update time for animation
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    public:
        Water(GLuint mirror_texture_id, Time* new_time, DirectionalLight* new_light) {
            reflection_texture_id = mirror_texture_id;
            time = new_time;
            light = new_light;
        }
};