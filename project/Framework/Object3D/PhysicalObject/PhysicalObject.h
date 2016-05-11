#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class PhysicalObject: public Object3D {
    private:
        GLenum draw_mode_ = GL_TRIANGLE_STRIP;    // used after as the 1st parameter of glDrawElements
        GLuint Ma_id_, Md_id_, Ms_id_, alpha_id_;

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
        void addTexture(GLuint texture_id) {
            std::ostringstream tex_name;
            tex_name << "tex" << texture_ids_.size();

            GLuint tex_id = glGetUniformLocation(program_id_, tex_name.str().c_str());
            glUniform1i(tex_id, texture_ids_.size());

            texture_ids_.push_back(texture_id);
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
            Ma_id_ = glGetUniformLocation(program_id_, "Ma");
            Md_id_ = glGetUniformLocation(program_id_, "Md");
            Ms_id_ = glGetUniformLocation(program_id_, "Ms");
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

                // bind textures
                for(std::vector<GLuint>::iterator it = texture_ids_.begin(); it != texture_ids_.end(); ++it) {
                    GLuint cur_id = *it;
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, cur_id);
                }

                // setup MVP
                glm::mat4 model = transform.getModelMatrix();
                glm::mat4 MVP = projection*view*model;
                glUniformMatrix4fv(MVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(MVP));

                // setup Material uniforms
                // TODO carefull, mayve getMaterial will have to return a pointer
                Material material = renderer.getMaterial();
                glUniform3fv(Ma_id_, ONE, glm::value_ptr(material.getAmbienceColor()));
                glUniform3fv(Md_id_, ONE, glm::value_ptr(material.getDiffuseColor()));
                glUniform3fv(Ms_id_, ONE, glm::value_ptr(material.getSpecularColor()));
                glUniform1f(alpha_id_, material.getAlpha());

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
            // TODO turn boolean value into boolean vector
            for (std::vector<GLuint>::iterator it = texture_ids_.begin(); it != texture_ids_.end(); ++it) {
                GLuint cur_id = *it;
                glDeleteTextures(1, &cur_id);
            }
        }
};