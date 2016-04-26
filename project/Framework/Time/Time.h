#pragma once
#include "icg_helper.h"

class Time {
    private:
        double cur_time;
        double prev_time;
        double deltaTime;

    public:
        static void Init() {
            cur_time = glfwGetTime();
            prev_time = glfwGetTime();
            deltaTime = 0.0f;
        }

        static void Update() {
            prev_time = cur_time;
            cur_time = glfwGetTime();
            deltaTime = cur_time - prev_time;
        }

        static float DeltaTime() {
            return (float)deltaTime;
        }
};