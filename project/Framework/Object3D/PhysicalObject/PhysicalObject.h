#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class PhysicalObject: public Object3D {
    private:
        GLenum draw_mode_ = GL_TRIANGLE_STRIP;    // used after as the 1st parameter of glDrawElements
        bool is_texture_defined = false;

    protected:
        virtual void InitialCalculations() {};    // Called once, before any OpenGL operations take place
        virtual void LoadShaders() {};            // Called once, to compile the object's shaders
        virtual void SetupVertices() {};          // Called once, to setup the mesh's vertices
        virtual void SetupIndexBuffer() {};       // Called once, to setup the vertices' indices
        virtual void SetupUniforms() {};          // Called once, to setup new uniforms for the shader
        virtual void UpdateUniforms() {};         // Called every Draw call, to update the uniforms' values

        /** Called to set the drawing mode to either:
          * 0     - GL_TRIANGLES
          * 1     - GL_TRIANGLE_STRIP
          * Other - GL_TRIANGLE_STRIP
          */
        void setDrawMode(int new_mode) {
            if (new_mode == 0) {
                draw_mode_ = GL_TRIANGLES;
            } else if (new_mode == 1) {
                draw_mode_ = GL_TRIANGLE_STRIP;
            } else {
                Reporter::println("Invalid draw mode provided. Reverting to default (GL_TRIANGLE_STRIP)");
                draw_mode_ = GL_TRIANGLE_STRIP;
            }
        }

        /** This method can be called from any of
          * the virtual methods defined above except 
          * 'InitialCalculations'. 
          */
        void setTexture(string texture_name) {
            int width;
            int height;
            int nb_component;

            is_texture_defined = true;

            // set stb_image to have the same coordinates as OpenGL
            stbi_set_flip_vertically_on_load(1);
            unsigned char* image = stbi_load(texture_name.c_str(), &width,
                                             &height, &nb_component, 0);

            if(image == nullptr) {
                is_texture_defined = false;
                throw(string("Failed to load texture"));
            }

            glGenTextures(1, &texture_id_);
            glBindTexture(GL_TEXTURE_2D, texture_id_);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            if(nb_component == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                             GL_RGB, GL_UNSIGNED_BYTE, image);
            } else if(nb_component == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, image);
            }

            GLuint tex_id = glGetUniformLocation(program_id_, "texture");
            glUniform1i(tex_id, 0 /*GL_TEXTURE0*/);

            // cleanup
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(image);
        }

    public:
        void Init() {
            InitialCalculations();

            // call to the sub-class' method to get the shaders' name
            LoadShaders();

            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // call to sub-class' method to setup the vertices correctly
            SetupVertices();

            // position shader attribute
            // fetch attribute ID for vertex positions
            GLuint loc_position = glGetAttribLocation(program_id_, "position");
            glEnableVertexAttribArray(loc_position); // Enable it
            glVertexAttribPointer(loc_position, 3, GL_FLOAT, DONT_NORMALIZE,
                                  ZERO_STRIDE, ZERO_BUFFER_OFFSET);

            // call to sub-class' method to setup the index buffer correctly
            SetupIndexBuffer();

            // other uniforms
            MVP_id_ = glGetUniformLocation(program_id_, "MVP");
            SetupUniforms();

            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);

            is_initialized_ = true;
        }

        void Draw(const glm::mat4 &view = IDENTITY_MATRIX,
                  const glm::mat4 &projection = IDENTITY_MATRIX) {
            if (is_initialized_) {
                glUseProgram(program_id_);
                glBindVertexArray(vertex_array_id_);

                // bind texture
                if (is_texture_defined) {
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, texture_id_);
                }

                // setup MVP
                glm::mat4 model = transform.getModelMatrix();
                glm::mat4 MVP = projection*view*model;
                glUniformMatrix4fv(MVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(MVP));

                UpdateUniforms();

                // draw
                glDrawElements(draw_mode_, num_indices_, GL_UNSIGNED_INT, 0);

                glBindVertexArray(0);
                glUseProgram(0);
            } else {
                reporter_.println("Cannot draw uninitialized Object3D!", "Warning");
            }
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_position_);
            glDeleteBuffers(1, &vertex_buffer_object_index_);
            glDeleteProgram(program_id_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            if (is_texture_defined) glDeleteTextures(1, &texture_id_);
        }
};