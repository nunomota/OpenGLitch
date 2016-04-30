#pragma once
#include "icg_helper.h"

// setup 1D color texture
#define NB_COLOR 2
GLfloat tex[NB_COLOR] = {0.0, 2.0 * 3.14};



struct PASS {
        enum ENUM {
            FIRST=1, SECOND=2
        };
};

class ScreenQuad {

    private:
        GLuint vertex_array_id_;        // vertex array object
        GLuint program_id_;             // GLSL shader program ID
        GLuint vertex_buffer_object_;   // memory buffer
        GLuint texture_1_id_;           // texture ID 1
        GLuint texture_1d_id_;          // texture ID

        float std_dev = 2;
        float num_of_tiles = 4.0;
        float amplitude = 1.0;
        int octaves = 3;
        float gain = 1.0;
        float offset = 1.0;
        int fractal_algo = 0; // 0 = fBm, 1=

        int efficient_gaussian_;

        int screenquad_width_;
        int screenquad_height_;

    public:
        void Init(int width, int height, GLuint texture) {

            // set screenquad size
            this->screenquad_width_ = width;
            this->screenquad_height_ = height;

            // compile the shaders
            program_id_ = icg_helper::LoadShaders("screenquad_vshader.glsl",
                                                  "screenquad_fshader.glsl");
            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex Array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // vertex coordinates
            {
                const GLfloat vertex_point[] = { /*V1*/ -1.0f, -1.0f, 0.0f,
                                                 /*V2*/ +1.0f, -1.0f, 0.0f,
                                                 /*V3*/ -1.0f, +1.0f, 0.0f,
                                                 /*V4*/ +1.0f, +1.0f, 0.0f
            };
                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_point),
                             vertex_point, GL_STATIC_DRAW);

                // attribute
                GLuint vertex_point_id = glGetAttribLocation(program_id_, "vpoint");
                glEnableVertexAttribArray(vertex_point_id);
                glVertexAttribPointer(vertex_point_id, 3, GL_FLOAT, DONT_NORMALIZE,
                                      ZERO_STRIDE, ZERO_BUFFER_OFFSET);
            }

            // texture coordinates
            {
                const GLfloat vertex_texture_coordinates[] = { /*V1*/ 0.0f, 0.0f,
                                                               /*V2*/ 1.0f, 0.0f,
                                                               /*V3*/ 0.0f, 1.0f,
                                                               /*V4*/ 1.0f, 1.0f

            };

                // buffer
                glGenBuffers(1, &vertex_buffer_object_);
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_texture_coordinates),
                             vertex_texture_coordinates, GL_STATIC_DRAW);

                // attribute
                GLuint vertex_texture_coord_id = glGetAttribLocation(program_id_,
                                                                     "vtexcoord");
                glEnableVertexAttribArray(vertex_texture_coord_id);
                glVertexAttribPointer(vertex_texture_coord_id, 2, GL_FLOAT,
                                      DONT_NORMALIZE, ZERO_STRIDE,
                                      ZERO_BUFFER_OFFSET);
            }

            // load/Assign texture
            this->texture_1_id_ = texture;
            glBindTexture(GL_TEXTURE_2D, texture_1_id_);
            float * data = new float[screenquad_width_*screenquad_height_];
            glTexImage2D(GL_TEXTURE_2D,0,GL_R32F,screenquad_width_,screenquad_height_,0,GL_DEPTH_COMPONENT,GL_FLOAT,data);

            
            // 1D texture for gradients shader
            glGenTextures(1, &texture_1d_id_);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_1D, texture_1d_id_);
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, NB_COLOR, 0, GL_RED, GL_FLOAT, tex);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glBindTexture(GL_TEXTURE_1D, texture_1d_id_);
            
            glUniform1i(glGetUniformLocation(program_id_, "tex_PASS1"),
                        0 /*GL_TEXTURE0*/);

            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_);
            glDeleteProgram(program_id_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            glDeleteTextures(1, &texture_1_id_);
        }

        void UpdateSize(int screenquad_width, int screenquad_height) {
            this->screenquad_width_ = screenquad_width;
            this->screenquad_height_ = screenquad_height;
        }


        void UpdateNumberOfTiles(float tiles){
            this->num_of_tiles += tiles;
            std::cout << "new num of tiles : " << num_of_tiles << std::endl;
        }
        
        void UpdateFractalAmplitude(float increment){
            this->amplitude += increment;
            std::cout << "new amplitude : " << amplitude << std::endl;
        }

        void UpdateNumberOfOctaves(int increment){
            this->octaves += increment;
            std::cout << "new num of octaves : " << octaves << std::endl;
        }

        void UpdateGain(float increment){
            this->gain += increment;
            std::cout << "new gain : " << gain << std::endl;
        }

        void UpdateOffset(float increment){
            this->offset += increment;
            std::cout << "new offset : " << offset << std::endl;
        }

        void UseFractalAlgorithm(int value){
            this->fractal_algo = value;
            if( value == 0){
                std::cout << "using fBm : " << offset << std::endl;
            }else if( value == 1){
                std::cout << "using Ridged Multifractal : " << offset << std::endl;
            }else{
                std::cout << "using hybrid Multifractal : " << offset << std::endl;
            }

        }

        void Draw() {
            glUseProgram(program_id_);
            glBindVertexArray(vertex_array_id_);

            glUniform1f(glGetUniformLocation(program_id_, "num_tiles"), num_of_tiles);
            glUniform1f(glGetUniformLocation(program_id_, "H"), amplitude);
            glUniform1i(glGetUniformLocation(program_id_, "octaves"), octaves);
            glUniform1f(glGetUniformLocation(program_id_, "gain"), gain);
            glUniform1f(glGetUniformLocation(program_id_, "offset"), offset);
            glUniform1i(glGetUniformLocation(program_id_, "fractal_algorithm"), fractal_algo);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_1D, texture_1_id_);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glBindVertexArray(0);
            glUseProgram(0);
        }
};
