#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

#include "Reporter/Reporter.h"

Reporter reporter;

/** The Object3D class is a generic class from
  * which every 3D object instatiated on a scene
  * inherit from.
  */
class Object3D {

    protected:
        GLuint vertex_array_id_;                // vertex array object
        GLuint vertex_buffer_object_position_;  // memory buffer for positions
        GLuint vertex_buffer_object_index_;     // memory buffer for indices
        GLuint program_id_;                     // GLSL shader program ID
        GLuint num_indices_;                    // number of vertices to render

        bool is_initialized_ = false;
        glm::mat4 model_;

        virtual void Init();

    public:
        Transform transform;    // Object-specific Transform

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
                Reporter.println("Cannot draw uninitialized Object3D", 2);
            }
        }

        void CleanUp() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_position_);
            glDeleteBuffers(1, &vertex_buffer_object_index_);
            glDeleteProgram(program_id_);
            glDeleteVertexArrays(1, &vertex_array_id_);
        }
};