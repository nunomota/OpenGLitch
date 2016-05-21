#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

/** This class is simply used as a holder
  * of 3D information such as: position,
  * rotation and scale.
  */
class Transform {

    private:
        static const glm::vec3 world_up_vector;
        static const glm::vec3 world_forward_vector;
        static const glm::vec3 world_right_vector;

        Reporter reporter_;
        GlmStrings glm_strings_;

        glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::mat4 model_ = IDENTITY_MATRIX;
        glm::mat4 inverted_model_ = IDENTITY_MATRIX;

        // directional vectors
        glm::vec3 up_vector_ = world_up_vector;
        glm::vec3 forward_vector_ = world_forward_vector;
        glm::vec3 right_vector_ = world_right_vector;

        void calculateTranslation() {
            model_ = glm::translate(model_, position_);
        }

        void calculateRotation() {
            model_ = glm::rotate(model_, glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model_ = glm::rotate(model_, glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model_ = glm::rotate(model_, glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        void calculateScale() {
            model_ = glm::scale(model_, scale_);
        }

        void recalculateModelMatrix() {

            model_ = IDENTITY_MATRIX;
            inverted_model_ = IDENTITY_MATRIX;

            calculateTranslation();
            calculateRotation();
            calculateScale();
            // TODO maybe calculate the inverse matrix step-by-step instead?
            inverted_model_ = inverse(model_);

            updateDirectionalVectors();
        }

        void updateDirectionalVectors() {
            glm::vec4 rotated_up_vector      = glm::vec4(world_up_vector, 0.0f);
            glm::vec4 rotated_forward_vector = glm::vec4(world_forward_vector, 0.0f);
            glm::vec4 rotated_right_vector   = glm::vec4(world_right_vector, 0.0f);
            glm::mat4 rotaion_matrix         = IDENTITY_MATRIX;

            rotaion_matrix = glm::rotate(rotaion_matrix, glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotaion_matrix = glm::rotate(rotaion_matrix, glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
            rotaion_matrix = glm::rotate(rotaion_matrix, glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));

            rotated_up_vector      = rotaion_matrix * rotated_up_vector;
            rotated_forward_vector = rotaion_matrix * rotated_forward_vector;
            rotated_right_vector   = rotaion_matrix * rotated_right_vector;

            up_vector_      = glm::vec3(rotated_up_vector);
            forward_vector_ = glm::vec3(rotated_forward_vector);
            right_vector_   = glm::vec3(rotated_right_vector);
        }

    public:
        Transform() {
            position_ = glm::vec3(0.0f, 0.0f, 0.0f);
            rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);
            scale_ = glm::vec3(1.0f, 1.0f, 1.0f);
            recalculateModelMatrix();
        }

        void setPosition(glm::vec3 new_position) {
            position_ = new_position;
            recalculateModelMatrix();
        }

        void setRotation(glm::vec3 new_rotation) {
            rotation_ = new_rotation;
            recalculateModelMatrix();   
        }

        void setScale(glm::vec3 new_scale) {
            scale_ = new_scale;
            recalculateModelMatrix();
        }

        glm::vec3 getPosition() {
            return position_;
        }

        glm::vec3 getRotation() {
            return rotation_;
        }

        glm::vec3 getScale() {
            return scale_;
        }

        glm::vec3 getUpVector() {
            return up_vector_;
        }

        glm::vec3 getForwardVector() {
            return forward_vector_;
        }

        glm::vec3 getRightVector() {
            return right_vector_;
        }

        glm::mat4 getModelMatrix() {
            return model_;
        }

        glm::mat4 getInvertedModelMatrix() {
            return inverted_model_;
        }

        std::string to_string() {
            std::ostringstream transform_representation;
            transform_representation << std::fixed << std::setprecision(2);
            transform_representation << "┌--- Transform --------------------┐" << std::endl
                                     << "| position: " << glm_strings_.create(position_) << std::endl
                                     << "| rotation: " << glm_strings_.create(rotation_) << std::endl
                                     << "| scale   : " << glm_strings_.create(scale_)    << std::endl
                                     << "└----------------------------------┘";
            return transform_representation.str();
        }
};
const glm::vec3 Transform::world_up_vector      = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 Transform::world_forward_vector = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 Transform::world_right_vector   = glm::vec3(1.0f, 0.0f, 0.0f);