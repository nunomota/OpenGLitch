#pragma once

class Water: public Grid {
    private:
        GLuint reflection_texture_id;
        GLuint time_id;

        Time* time;
        DirectionalLight* light;
        Camera* camera;

        GLuint light_dir_id;
        GLuint light_pos_id;
        GLuint light_a_id;
        GLuint light_d_id;
        GLuint light_s_id;
        GLuint camera_pos_id;

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
            camera_pos_id = glGetUniformLocation(program_id_, "cameraPosition");

            glUniform3fv(light_dir_id, 1, glm::value_ptr(light->getDirection()));
            glUniform3fv(light_pos_id, 1, glm::value_ptr(light->getTransform()->getPosition()));
            glUniform3fv(light_a_id, 1, glm::value_ptr(light->getAmbienceColor()));
            glUniform3fv(light_d_id, 1, glm::value_ptr(light->getDiffuseColor()));
            glUniform3fv(light_s_id, 1, glm::value_ptr(light->getSpecularColor()));
            glUniform3fv(camera_pos_id, 1, glm::value_ptr(camera->getTransform()->getPosition()));
            // TODO setup time for animation
        }

        void UpdateUniforms() {
            glUniform1f(time_id, time->getCurrentTime());
            // TODO update time for animation
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glUniform3fv(camera_pos_id, 1, glm::value_ptr(camera->getTransform()->getPosition()));
        }
    public:
        Water(GLuint mirror_texture_id, Time* new_time, DirectionalLight* new_light, Camera* new_camera) {
            reflection_texture_id = mirror_texture_id;
            time = new_time;
            light = new_light;
            camera = new_camera;
        }
};