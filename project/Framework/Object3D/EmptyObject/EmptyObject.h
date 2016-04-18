#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class EmptyObject: public Object3D {
    public:
        void Init() {
            is_initialized_ = true;
        }

        void Draw(const glm::mat4 &view = IDENTITY_MATRIX,
                  const glm::mat4 &projection = IDENTITY_MATRIX) {}

        void Cleanup() {}
};