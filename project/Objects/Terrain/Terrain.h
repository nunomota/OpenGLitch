#pragma once

class Terrain: public Grid {
    private:
        HeightBuffer heightbuffer;
        HeightMap height_map;
        GLuint height_map_id_;

        DirectionalLight* light;
        Camera* camera;
        Time* time;

        GLuint time_id;
        GLuint light_dir_id;
        GLuint light_pos_id;
        GLuint light_a_id;
        GLuint light_d_id;
        GLuint light_s_id;
        GLuint camera_pos_id;

        static const int height_map_width = 500;
        static const int height_map_height = 500;
        static const int height_map_colors = 3;
        GLfloat height_map_heights[height_map_width * height_map_height * height_map_colors];

        void calculateHeights() {
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, height_map_heights);
        }

    protected:
        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("terrain_vshader.glsl",
                                                  "terrain_fshader.glsl");
        }

        void SetupUniforms() {
            height_map_id_ = Loader::loadTexture("perlin.jpg");
            addTexture(height_map_id_);
            addTexture(Loader::loadTexture("perlinNormal.png"));
            addTexture(Loader::loadTexture("waterNormal.png"));
            time_id = glGetUniformLocation(program_id_, "time");
            calculateHeights();

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
        }

        void UpdateUniforms() {
            glUniform1f(time_id, time->getCurrentTime());
        }

    public:
        Terrain(Time* new_time, DirectionalLight* new_light, Camera* new_camera) {
            time = new_time;
            light = new_light;
            camera = new_camera;
        }

        GLuint getHeightMapID() {
            return height_map_id_;
        }

        float getHeight(float x, float y) {
            int index = (x + y*height_map_width)*height_map_colors;
            return height_map_heights[index];
        }
};