#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class PointLight: public Light {
    private:
        float radius_ = 1.0f;

    public:
        void setRadius(float new_radius) {
            radius_ = new_radius;
        }

        float getRadius() {
            return radius_;
        }
};