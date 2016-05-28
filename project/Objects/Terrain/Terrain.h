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
            glBindTexture(GL_TEXTURE_2D, height_map_id_);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, height_map_heights);
        }

        bool inBounds(glm::vec2 target_coords, float right, float left, float top, float bottom) {
            if ((target_coords.x > left   && target_coords.x < right) &&
                (target_coords.y > bottom && target_coords.y < top)) {
                return true;
            }
            return false;
        }

        glm::vec2 worldToTexCoords(glm::vec2 worldCoords) {
            GlmStrings glmStrings;
            glm::vec3 terrain_coords_3d = getTransform()->getPosition();
            glm::vec2 terrain_coords_2d = glm::vec2(terrain_coords_3d.x, terrain_coords_3d.z);
            glm::vec3 terrain_scale_3d  = getTransform()->getScale();
            glm::vec2 terrain_scale_2d  = glm::vec2(terrain_scale_3d.x, terrain_scale_3d.z);

            float terrain_width  = 2.0f * terrain_scale_2d.x;
            float terrain_height = 2.0f * terrain_scale_2d.y;
            float right_border   = terrain_coords_2d.x + 1.0f * terrain_scale_2d.x;
            float left_border    = terrain_coords_2d.x - 1.0f * terrain_scale_2d.x;
            float top_border     = terrain_coords_2d.y + 1.0f * terrain_scale_2d.y;
            float bottom_border  = terrain_coords_2d.y - 1.0f * terrain_scale_2d.y;

            if (inBounds(worldCoords, right_border, left_border, top_border, bottom_border)) {
                glm::vec2 bl_corner = glm::vec2(left_border, bottom_border);
                glm::vec2 local_vector = worldCoords - bl_corner;
                local_vector = glm::vec2(local_vector.x / terrain_width, local_vector.y / terrain_height);
                cout << "[W] " << glmStrings.create(glm::vec3(worldCoords, 0.0f)) << " [L] " << glmStrings.create(glm::vec3(local_vector, 0.0f)) << endl;
                return local_vector;
            }
            cout << "Out of bounds" << endl;
            return glm::vec2(0, 0);
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

        float getHeight(glm::vec3 camera_coords_3d) {
            GlmStrings glmStrings;
            glm::vec2 texCoords = worldToTexCoords(glm::vec2(camera_coords_3d.x, camera_coords_3d.z));
            int pixel_coord_x = texCoords.x * height_map_width;
            int pixel_coord_y = texCoords.y * height_map_height;
            int index = (pixel_coord_x + pixel_coord_y*height_map_width)*height_map_colors;
            cout << "[P] " << glmStrings.create(glm::vec3(pixel_coord_x, pixel_coord_y, 0.0f)) << " [I] " << index << " [H] " << height_map_heights[index] << endl;
            return height_map_heights[index]*2.0f - 1.0f;
        }
};