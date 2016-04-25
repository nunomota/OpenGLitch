#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class DirectionalLight: public Light {
    private:
        glm::vec3 default_direction_ = glm::vec3(0.0f, 0.0f, 1.0f);

        glm::mat4 getRotationMatrix() {
            glm::mat4 rotation_matrix = IDENTITY_MATRIX;

            rotation_matrix = glm::rotate(rotation_matrix, glm::radians(transform.getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
            rotation_matrix = glm::rotate(rotation_matrix, glm::radians(transform.getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotation_matrix = glm::rotate(rotation_matrix, glm::radians(transform.getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

            return rotation_matrix;
        }

    public:
        glm::vec3 getDirection() {
            glm::mat4 rotation_matrix = getRotationMatrix();
            return (rotation_matrix * glm::vec4(default_direction_, 1.0f)).xyz;
        }
};