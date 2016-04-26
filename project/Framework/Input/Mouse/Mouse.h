#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Mouse {
    private:
        static const int supportedButtons = 2;
        static const int startingKeyIndex = 1;
        bool buttons[supportedButtons];

        void initButtonValues() {
            for (int i = 0; i < supportedButtons; i++) {
                buttons[i] = false;
            }
        }

        bool isValidKey(int key) {
            return (key >= 0 && key < supportedButtons)? true: false;
        }

    public:
        static const int LEFT_BUTTON = 0;
        static const int RIGHT_BUTTON = 1;

        Mouse() {
            initButtonValues();
        }

        void setState(int key, bool state) {
            int new_key = key-startingKeyIndex;
            if (isValidKey(new_key)) {
                buttons[key] = state;
            }
        }

        bool getState(int key) {
            return (isValidKey(key))? buttons[key]:false;
        }
};