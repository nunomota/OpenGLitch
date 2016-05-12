#pragma once

class Terrain: public Grid {
    private:
        FrameBuffer framebuffer;
        HeightMap height_map;
        GLuint height_map_id_;

    protected:
        void InitialCalculations() {
            Grid::InitialCalculations();
            height_map_id_ = framebuffer.Init(getDimension(), getDimension());

            // Draw the heightmap and store id
            framebuffer.Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            height_map.Draw();
            framebuffer.Unbind();

            int pixel[4];
            std::cout << "(0, 0) " << pixel[0] << std::endl;
            glReadPixels(0.5f, 0.5f, 1, 1, GL_RGB, GL_INT, &pixel[0]);
            std::cout << "(0, 0) " << pixel[0] << ", " << pixel[1] << ", " << pixel[2] << ", " << pixel[3] << std::endl;
        }

        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("terrain_vshader.glsl",
                                                  "terrain_fshader.glsl");
        }

        void SetupUniforms() {
            addTexture(height_map_id_);
        }

        void FinalOperations() {
            framebuffer.Cleanup();
        }
};