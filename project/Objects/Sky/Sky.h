#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Sky: public Cube {
    private:
        GLuint texture_id_;

    protected:
        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("sky_vshader.glsl",
                                                  "sky_fshader.glsl");
        }

        void SetupUniforms() {

            // make stbi use same coordinates as opengl
            stbi_set_flip_vertically_on_load(1);

            glGenTextures(1, &texture_id_);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
            int nb_component;

            vector<string> textures_faces;
            textures_faces.push_back("miramar_ft.tga");
            textures_faces.push_back("miramar_bk.tga");
            textures_faces.push_back("miramar_dn.tga");
            textures_faces.push_back("miramar_up.tga");
            textures_faces.push_back("miramar_rt.tga"); // front  - rt
            textures_faces.push_back("miramar_lf.tga"); // back - lf

            int width,height;
            unsigned char* image;  
            for(GLuint i = 0; i < textures_faces.size(); i++)
            {

                image = stbi_load(textures_faces[i].c_str(), &width,
                                                 &height, &nb_component, 0);

                if(image == nullptr) {
                   printf("failed to load texture");
                }
                if(nb_component == 3) {
                    glTexImage2D(
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);                
                } else if(nb_component == 4) {
                    glTexImage2D(
                        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                        0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
                }
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            GLuint tex_id = glGetUniformLocation(program_id_, "tex");
            glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);

            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }

        void UpdateUniforms(){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
        }

};