#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Plane: public PhysicalObject {
    private:
        const int sub_division = 512;

        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;

        void addVertex(float x, float y, std::vector<GLfloat>* vertices) {
            (*vertices).push_back(x); (*vertices).push_back(y);
        }

        void useVertex(int index, std::vector<GLuint>* indices) {
            (*indices).push_back(index);
        }

        void calculateVertices() {
            float step = 2.0f / (float)sub_division;
            for (int i = 0, mult = 1; i < sub_division+1; i++, mult = -mult) {
                float x_coord = -1.0f + i*step;
                for (int j = 0; j < sub_division+1; j++) {
                    float y_coord = (float)mult * (-1.0f + j*step);
                    if (i == 0) {
                        addVertex(x_coord, y_coord, &vertices); 
                    } else {
                        int index = (int)vertices.size()/2;

                        // use the previously calculated vertex, in the same line
                        useVertex(index - (j*2+1), &indices);
                        // create a new vertex
                        addVertex(x_coord, y_coord, &vertices);
                        useVertex(index, &indices);
                    }
                }
            }
        }

    protected:
        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("plane_vshader.glsl",
                                                  "plane_fshader.glsl");
        }

        void SetupVertices() {
            calculateVertices();

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
};