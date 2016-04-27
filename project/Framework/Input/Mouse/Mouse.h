#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Mouse {
    private:
        static const int clickYieldMiliseconds = 250;
        static const int supportedButtons = 2;
        static const int startingKeyIndex = 1;
        bool buttons[supportedButtons];
        float buttonDownTimeStamp[supportedButtons];

        void initButtonValues() {
            for (int i = 0; i < supportedButtons; i++) {
                buttons[i] = false;
                buttonDownTimeStamp[i] = 0.0f;
            }
        }

        bool isValidKey(int key) {
            return (key >= 0 && key < supportedButtons)? true: false;
        }

        bool isClickAvailable(int key, float timeStamp) {
            if (isValidKey(key)) {
                float minimum_time_diff = (float)clickYieldMiliseconds/1000.0f;
                float time_diff = timeStamp - buttonDownTimeStamp[key];
                return (time_diff >= minimum_time_diff && buttons[key])?true:false;
            }
            return false;
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
                buttons[new_key] = state;
            }
        }

        bool getDownState(int key) {
            return (isValidKey(key))? buttons[key]:false;
        }

        bool getPressedState(int key, float timeStamp) {
            if (isClickAvailable(key, timeStamp)) {
                buttonDownTimeStamp[key] = timeStamp;
                return buttons[key];
            }
            return false;
        }
};