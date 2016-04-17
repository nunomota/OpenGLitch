#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

#include "../GlmStrings/GlmStrings.h"

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

        void recalculateModelMatrix() {

            model_ = IDENTITY_MATRIX;

            // apply translation
            model_ = glm::translate(model_, position_);

            // apply rotation to the 3 axys (x, y, z)
            model_ = glm::rotate(model_, rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f));
            model_ = glm::rotate(model_, rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f));
            model_ = glm::rotate(model_, rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f));

            // apply scale
            model_ = glm::scale(model_, scale_);

            reporter_.new_line();
            reporter_.print_special(to_string());
            reporter_.new_line();
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

        std::string to_string() {
            std::ostringstream transform_representation;
            transform_representation << std::fixed << std::setprecision(2);
            transform_representation << "┌--- Transform --------------------┐" << std::endl
                                     << "| position: " << glm_strings_.create(position_) << std::endl
                                     << "| rotation: " << glm_strings_.create(rotation_) << std::endl
                                     << "| scale   : " << glm_strings_.create(scale_)    << std::endl
                                     << "| "                                             << std::endl
                                     <<                   glm_strings_.create(model_)    << std::endl
                                     << "└----------------------------------┘";
            return transform_representation.str();
        }
};