#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Reporter {
    private:
        const string flags[3] = string[]("Info", "Warning", "Error");
        const char flag_delimiters[2] = char[]('[', ']');

        static void output(string message, string flag) {
            string final_message = sprintf("%c %s %c  %s", flag_delimiters[0], flag, flag_delimiters[1], message);
            cout << final_message << endl;
        }

    public:
        static void println(string message, string flag) {
            output(message, flag);
        }

        static void println(string message, int flag_index) {
            int index = (flag_index >= 0 && flag_index < 3) ? flag_index : 0;
            output(message, flags[index]);
        }

        static void println(string message) {
            output(message, flags[0]);
        }
}