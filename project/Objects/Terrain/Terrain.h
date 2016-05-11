#pragma once

class Terrain: public Grid {
    private:
        FrameBuffer framebuffer;
        GLuint height_map_id_;

    protected:
        void InitialCalculations() {
            Grid::InitialCalculations();
            height_map_id_ = framebuffer.Init(getDimension(), getDimension());
        }

        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("grid_vshader.glsl",
                                                  "grid_fshader.glsl");
        }

        void FinalOperations() {
            framebuffer.Cleanup();
        }
};