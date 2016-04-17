#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

#include "../Object3D/Object3D.h"

class Cube: Object3D {
    private:
        // Position buffer
        const GLfloat position[] = {-1.0f, -1.0f,  1.0f, // left, bottom, front
                                    1.0f, -1.0f,  1.0f,  // right, bottom, front
                                    1.0f,  1.0f,  1.0f,  // right, top, front
                                    -1.0f,  1.0f,  1.0f, // left, top, front
                                    -1.0f, -1.0f, -1.0f, // left, bottom, back
                                    1.0f, -1.0f, -1.0f,  // right, bottom, back
                                    1.0f,  1.0f, -1.0f,  // right, top, back
                                    -1.0f,  1.0f, -1.0f};// left, top, back

        // index buffer
        const GLuint index[] = {0, 1, 2,  // front face triangle 1
                                0, 2, 3,  // front face triangle 2
                                1, 5, 6,  // right face triangle 1
                                1, 6, 2,  // right face triangle 2
                                5, 4, 7,  // back face triangle 1
                                5, 7, 6,  // back face triangle 2
                                4, 0, 3,  // left face triangle 1
                                4, 3, 7,  // left face triangle 2
                                3, 2, 6,  // top face triangle 1
                                3, 6, 7,  // top face triangle 2
                                1, 0, 4,  // bottom face triangle 1
                                1, 4, 5}; // bottom face triangle 2

    protected:
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