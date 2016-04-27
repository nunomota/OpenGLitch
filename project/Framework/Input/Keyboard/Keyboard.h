#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Keyboard {
    private:
        static const int supportedKeys = 26;
        static const int startingKeyIndex = 65;
        bool keys[supportedKeys];

        void initKeyValues() {
            for (int i = 0; i < supportedKeys; i++) {
                keys[i] = false;
            }
        }

        bool isValidKey(int key) {
            return (key >= 0 && key < supportedKeys)? true: false;
        }

    public:
        static const int A = 0;
        static const int B = 1;
        static const int C = 2;
        static const int D = 3;
        static const int E = 4;
        static const int F = 5;
        static const int G = 6;
        static const int H = 7;
        static const int I = 8;
        static const int J = 9;
        static const int K = 10;
        static const int L = 11;
        static const int M = 12;
        static const int N = 13;
        static const int O = 14;
        static const int P = 15;
        static const int Q = 16;
        static const int R = 17;
        static const int S = 18;
        static const int T = 19;
        static const int U = 20;
        static const int V = 21;
        static const int W = 22;
        static const int X = 23;
        static const int Y = 24;
        static const int Z = 25;

        Keyboard() {
            initKeyValues();
        }

        void setState(int key, bool state) {
            int new_key = key-startingKeyIndex;
            if (isValidKey(new_key)) {
                keys[key] = state;
            }
        }

        bool getState(int key) {
            return (isValidKey(key))? keys[key]:false;
        }
};