#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class DirectionalLight: public Light {
    private:
        glm::vec3 default_direction_ = glm::vec3(0.0f, 0.0f, 1.0f);
        Camera* shadow_camera;

        glm::mat4 getRotationMatrix() {
            glm::mat4 rotation_matrix = IDENTITY_MATRIX;

            rotation_matrix = glm::rotate(rotation_matrix, glm::radians(transform.getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
            rotation_matrix = glm::rotate(rotation_matrix, glm::radians(transform.getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotation_matrix = glm::rotate(rotation_matrix, glm::radians(transform.getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

            return rotation_matrix;
        }

    public:
        void setShadowCamera(Camera* new_shadow_camera) {
            shadow_camera = new_shadow_camera;
        }

        glm::vec3 getDirection() {
            glm::mat4 rotation_matrix = getRotationMatrix();
            glm::vec4 transformed_direction = rotation_matrix * glm::vec4(default_direction_, 1.0f);
            return glm::vec3(transformed_direction);
        }

        Camera* getShadowCamera() {
            return shadow_camera;
        }

        glm::mat4 getDepthMVP() {
            if(shadow_camera){
                glm::mat4 view = shadow_camera->getViewMatrix();
                glm::mat4 projection = shadow_camera->getProjectionMatrix();
                glm::mat4 model = getTransform()->getModelMatrix();
                return projection * view * model;
            } else{
                Reporter::println("NULL shadow camera");
                return IDENTITY_MATRIX; 
            }
        }

};