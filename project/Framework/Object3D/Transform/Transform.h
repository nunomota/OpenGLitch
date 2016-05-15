#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

/** This class is simply used as a holder
  * of 3D information such as: position,
  * rotation and scale.
  */
class Transform {

    private:
        Reporter reporter_;
        GlmStrings glm_strings_;

        glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::mat4 model_ = IDENTITY_MATRIX;
        glm::mat4 inverted_model_ = IDENTITY_MATRIX;

        void calculateTranslation() {
            model_ = glm::translate(model_, position_);
            inverted_model_ = glm::translate(inverted_model_, -position_);
        }

        void calculateRotation() {
            model_ = glm::rotate(model_, glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model_ = glm::rotate(model_, glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model_ = glm::rotate(model_, glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));
            inverted_model_ = glm::rotate(inverted_model_, glm::radians(-rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
            inverted_model_ = glm::rotate(inverted_model_, glm::radians(-rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
            inverted_model_ = glm::rotate(inverted_model_, glm::radians(-rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        void calculateScale() {
            model_ = glm::scale(model_, scale_);
            inverted_model_ = glm::scale(inverted_model_, scale_); // TODO contrain scale to be > 0 and use 1/scale instead of scale
        }

        void recalculateModelMatrix() {

            model_ = IDENTITY_MATRIX;
            inverted_model_ = IDENTITY_MATRIX;

            calculateTranslation();
            calculateRotation();
            calculateScale();
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