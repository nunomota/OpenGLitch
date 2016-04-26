#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Mouse {
    private:
        const int supportedButtons = 2;
        bool buttons[supportedButtons];

        void initButtonValues() {
            for (int i = 0; i < supportedButtons; i++) {
                buttons[i] = false;
            }
        }

    public:
        static const int LEFT_BUTTON = 0;
        static const int RIGHT_BUTTON = 1;

        Mouse() {
            initButtonValues();
        }

        void setState(int key, bool state) {
            int new_key = key-1;
            if (new_key >= 0 && new_key < supportedButtons) {
                buttons[key] = state;
            }
        }
};