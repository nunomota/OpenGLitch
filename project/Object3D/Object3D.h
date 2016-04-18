#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

/** The Object3D class is a generic class from
  * which every 3D object instatiated on a scene
  * inherit from.
  */
class Object3D {
    private:
        Reporter reporter_;

    protected:
        GLuint vertex_array_id_;                // vertex array object
        GLuint vertex_buffer_object_position_;  // memory buffer for positions
        GLuint vertex_buffer_object_index_;     // memory buffer for indices
        GLuint program_id_;                     // GLSL shader program ID
        GLuint num_indices_;                    // number of vertices to render
        GLuint MVP_id_;                         // model, view, proj matrix ID

        bool is_initialized_ = false;

        Transform transform;    // Object-specific Transform

        virtual void LoadShaders() {};
        virtual void SetupVertices() {};
        virtual void SetupIndexBuffer() {};

    public:
        // constructor
        Object3D () {
            // TODO add this object to World
        }

        // method called to rotate an Object3D along the specified axys
        void rotate(glm::vec3 rotation_vector) {
            glm::vec3 current_rotation = transform.getRotation();
            // TODO keep every single coordinate between 0 and 360
            transform.setRotation(current_rotation + rotation_vector);
        }

        // method called to translate an Object3D according to a vector
        void translate(glm::vec3 translation_vector) {
            glm::vec3 current_position = transform.getPosition();
            transform.setPosition(current_position + translation_vector);
        }

        // method called to scale an Object3D along the specified axys
        void scale(glm::vec3 scaling_vector) {
            glm::vec3 current_scale = transform.getScale();
            transform.setScale(current_scale + scaling_vector);
        }

        Transform getTransform() {
            return transform;
        }

        void Init() {
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