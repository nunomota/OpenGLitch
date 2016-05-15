#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Cube: public PhysicalObject {
    private:
        // Position buffer
        const GLfloat position[24] = {-1.0f, -1.0f,  1.0f, // left, bottom, front
                                    1.0f, -1.0f,  1.0f,  // right, bottom, front
                                    1.0f,  1.0f,  1.0f,  // right, top, front
                                    -1.0f,  1.0f,  1.0f, // left, top, front
                                    -1.0f, -1.0f, -1.0f, // left, bottom, back
                                    1.0f, -1.0f, -1.0f,  // right, bottom, back
                                    1.0f,  1.0f, -1.0f,  // right, top, back
                                    -1.0f,  1.0f, -1.0f};// left, top, back

        // index buffer
        const GLuint index[36] = {0, 1, 2,
                                  0, 2, 3,
                                  1, 5, 6,
                                  1, 6, 2,
                                  5, 4, 7,
                                  5, 7, 6,
                                  4, 0, 3,
                                  4, 3, 7,
                                  3, 2, 6,
                                  3, 6, 7,
                                  1, 0, 4,
                                  1, 4, 5};

    protected:
        void InitialCalculations() {
            setDrawMode(0);
        }

        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("cube_vshader.glsl",
                                                  "cube_fshader.glsl");
        }

        void SetupVertices() {
            glGenBuffers(1, &vertex_buffer_object_position_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_position_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);
        }

        void SetupIndexBuffer() {
            num_indices_ = sizeof(index) / sizeof(GLuint);

            glGenBuffers(1, &vertex_buffer_object_index_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_index_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
        }
};