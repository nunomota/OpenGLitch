#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

#include "Reporter/Reporter.h"

class Object3D {

    protected:
        GLuint vertex_array_id_;                // vertex array object
        GLuint vertex_buffer_object_position_;  // memory buffer for positions
        GLuint vertex_buffer_object_index_;     // memory buffer for indices
        GLuint program_id_;                     // GLSL shader program ID
        GLuint num_indices_;                    // number of vertices to render

        bool is_initialized = false;

        virtual void Init();

    public:
        Transform transform;    // Object-specific Transform

        void rotate() {
            return 0;
        }

        void translate() {
            return 0;
        }

        void scale() {
            return 0;
        }

        void Draw(const glm::mat4 &view = IDENTITY_MATRIX,
                  const glm::mat4 &projection = IDENTITY_MATRIX) {

            if (is_initialized) {
                glUseProgram(program_id_);
                glBindVertexArray(vertex_array_id_);

                // setup MVP
                glm::mat4 MVP = projection*view*model;
                glUniformMatrix4fv(MVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(MVP));

                // draw
                glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_INT, 0);

                glBindVertexArray(0);
                glUseProgram(0);
            } else {
                Reporter.println("Cannot draw uninitialized Object3D", 2);
            }
        }

        void CleanUp() {
            return 0;
        }
};