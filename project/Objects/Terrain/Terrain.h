#pragma once

class Terrain: public Grid {
    private:
        HeightBuffer heightbuffer;
        HeightMap height_map;
        GLuint height_map_id_;
        GLuint shadow_texture_id;
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
            addTexture(shadow_texture_id);
        }

        void FinalOperations() {
            heightbuffer.Cleanup();
        }

    public:
        Terrain(GLuint new_shadow_texture_id){
            shadow_texture_id = new_shadow_texture_id;
        }


};