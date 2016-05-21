#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Plane: public PhysicalObject {
    private:
        // Position buffer
        const GLfloat position[12] = {-1.0f, 0.0f,  +1.0f,
                                      +1.0f, 0.0f,  +1.0f,
                                      -1.0f, 0.0f,  -1.0f,
                                      +1.0f, 0.0f,  -1.0f};

        // index buffer
        const GLuint index[4] = {0, 1, 2, 3};

    protected:
        void InitialCalculations() {
            setDrawMode(1);
        }

        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("plain_vshader.glsl",
                                                  "plain_fshader.glsl");
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