#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

/** This class is simply used as a holder
  * of 3D information such as: position,
  * rotation and scale.
  */
class Transform {

    private:
        glm::vec3 position_(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation_(0.0f, 0.0f, 0.0f);
        glm::vec3 scale_(1.0f, 1.0f, 1.0f);

    public:
        void setPosition(glm::vec3 new_position) {
            position_ = new_position;
        }

        void setRotation(glm::vec3 new_rotation) {
            rotation_ = new_rotation;
        }

        void setScale(glm::vec3 new_scale) {
            scale_ = new_scale;
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
};