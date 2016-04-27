#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Renderer {
    private:
        bool is_enabled;

    public:
        Renderer() {
            is_enabled = true;
        }

        bool getState() {
            return is_enabled;
        }

        void setState(bool new_state) {
            is_enabled = new_state;
        }
}