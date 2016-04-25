#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Light: public Light {
    protected:
        glm::vec3 color_ = glm::vec3(0.0f, 0.0f, 0.0f);
        float intensity_ = 1.0f;
    public:
        void setColor(glm::vec3 new_color) {
            color_ = new_color;
        }

        void setIntensity(float new_intesity) {
            intensity_ = new_intesity;
        }

        glm::vec3 getColor() {
            return color_;
        }

        float getIntensity() {
            return intensity_;
        }
};