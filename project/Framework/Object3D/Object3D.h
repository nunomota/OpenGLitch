#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

/** The Object3D class is a generic class from
  * which every 3D object instatiated on a scene
  * inherit from.
  */
class Object3D {
    protected:
        Reporter reporter_;

        GLuint vertex_array_id_;                  // vertex array object
        GLuint vertex_buffer_object_position_;    // memory buffer for positions
        GLuint vertex_buffer_object_index_;       // memory buffer for indices
        GLuint vertex_buffer_object_uv_;          // memory buffer for uv coordinates
        GLuint program_id_;                       // GLSL shader program ID
        GLuint num_indices_;                      // number of vertices to render
        GLuint MVP_id_;                           // model, view, proj matrix ID
        GLuint clip_plane_id_;                    // clip plane ID

        std::vector<GLuint> texture_ids_;         // Optional texture IDs

        bool is_initialized_ = false;

        Transform transform;    // Object-specific Transform
        Renderer renderer;      // Object-specific Renderer
        int tag = 0;            // default tag for every object

    public:
        // constructor
        Object3D () {
            //World.addObject(this);
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

        void setTag(int new_tag) {
            tag = new_tag;
        }

        int getTag() {
            return tag;
        }

        Transform* getTransform() {
            return &transform;
        }

        Renderer* getRenderer() {
            return &renderer;
        }

        virtual void Init() {}

        virtual void Draw(const glm::vec4 &clipping_plane = glm::vec4(1.0f),
                          const glm::mat4 &view = IDENTITY_MATRIX,
                          const glm::mat4 &projection = IDENTITY_MATRIX) {}

        virtual void Cleanup() {}
};