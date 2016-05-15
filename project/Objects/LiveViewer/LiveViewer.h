#pragma once

class LiveViewer: public Grid {
    private:
        GLuint live_texture_id_;
    public:
        LiveViewer(GLuint live_texture_id) {
            live_texture_id_ = live_texture_id;
        }

        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("textured_vshader.glsl",
                                                  "textured_fshader.glsl");
        }

        void SetupUniforms() {
            addTexture(live_texture_id_);
        }
};