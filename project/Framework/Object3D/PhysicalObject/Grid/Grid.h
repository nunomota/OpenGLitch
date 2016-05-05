#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Grid: public PhysicalObject {
    private:
        const int grid_dim = 512;
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;

        void addVertex(float x, float y, float z) {
            vertices.push_back(x); vertices.push_back(y); vertices.push_back(z);
        }

        void useVertex(int index) {
            indices.push_back(index);
        }

    protected:
        void InitialCalculations() {
            float step = 2.0f / (float)grid_dim;
            for (int i = 0, mult = 1; i < grid_dim+1; i++, mult = -mult) {
                float x_coord = -1.0f + i*step;
                for (int j = 0; j < grid_dim+1; j++) {
                    float y_coord = (float)mult * (-1.0f + j*step);
                    if (i == 0) {
                        addVertex(x_coord, 0.0f, y_coord);   // create a new vertex
                    } else {
                        int index = (int)vertices.size()/2;
                        useVertex(index - (j*2+1));          // use the previously calculated vertex, in the same line
                        addVertex(x_coord, 0.0f, y_coord);   // create a new vertex
                        useVertex(index);                    // use the newly created vertex
                    }
                }
            }
        }

        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("cube_vshader.glsl",
                                                  "cube_fshader.glsl");
        }

        void SetupVertices() {
            glGenBuffers(1, &vertex_buffer_object_position_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_position_);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                         &vertices[0], GL_STATIC_DRAW);
        }

        void SetupIndexBuffer() {
            num_indices_ = indices.size();

            glGenBuffers(1, &vertex_buffer_object_index_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_index_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                         &indices[0], GL_STATIC_DRAW);
        }

        void SetupUniforms() {
            // TODO declare the texture
        }

        void UpdateUniforms() {
            // TODO bind the texture
        }
};