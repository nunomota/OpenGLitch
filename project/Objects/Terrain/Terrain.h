#pragma once

class Terrain: public Grid {
    private:
        HeightBuffer heightbuffer;
        HeightMap height_map;
        GLuint height_map_id_;

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
            height_map_id_ = heightbuffer.Init(getDimension(), getDimension());

            // Draw the heightmap and store id
            heightbuffer.Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            height_map.Draw();
            heightbuffer.Unbind();
        }

        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("terrain_vshader.glsl",
                                                  "terrain_fshader.glsl");
        }

        void SetupUniforms() {
            addTexture(Loader::loadTexture("perlin.jpg"));
            addTexture(Loader::loadTexture("perlinNormal.png"));

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

        void FinalOperations() {
            heightbuffer.Cleanup();
        }

    public:
        Terrain(DirectionalLight* new_light, Camera* new_camera) {
            light = new_light;
            camera = new_camera;
        }
};