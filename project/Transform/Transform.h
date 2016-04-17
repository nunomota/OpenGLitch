#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

/** This class is simply used as a holder
  * of 3D information such as: position,
  * rotation and scale.
  */
class Transform {

    private:
        glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 scale_ = glm::vec3(1.0f, 1.0f, 1.0f);

        glm::mat4 model_ = IDENTITY_MATRIX;

        void recalculateModelMatrix() {
            // apply scale
            model_ = glm::scale(IDENTITY_MATRIX, scale_);

            // apply rotation to the 3 axys (x, y, z)
            model_ = glm::rotate(model_, rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f));
            model_ = glm::rotate(model_, rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f));
            model_ = glm::rotate(model_, rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f));

            // apply translation
            model_ = glm::translate(model_, position_);
        }

    public:
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
};