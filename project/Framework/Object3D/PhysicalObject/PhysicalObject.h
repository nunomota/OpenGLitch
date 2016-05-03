#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class PhysicalObject: public Object3D {
    protected:
        virtual void InitialCalculations() {};
        virtual void LoadShaders() {};
        virtual void SetupVertices() {};
        virtual void SetupIndexBuffer() {};
        virtual void SetupUniforms() {};
        virtual void UpdateUniforms() {};

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

                // setup MVP
                glm::mat4 model = transform.getModelMatrix();
                glm::mat4 MVP = projection*view*model;
                glUniformMatrix4fv(MVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(MVP));

                UpdateUniforms();

                // draw
                glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, 0);

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
        }
};