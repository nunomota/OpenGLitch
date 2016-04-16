#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Reporter {
    private:
        const std::string flags[3]{"Info", "Warning", "Error"};
        const char flag_delimiters[2]{'[', ']'};

        void output(std::string message, std::string flag) {
            std::ostringstream final_message;
            final_message << flag_delimiters[0] << flag << flag_delimiters[1] << " " << message;
            std::cout << final_message.str() << std::endl;
        }

    public:
        void println(std::string message, std::string flag) {
            output(message, flag);
        }

        void println(std::string message, int flag_index) {
            int index = (flag_index >= 0 && flag_index < 3) ? flag_index : 0;
            output(message, flags[index]);
        }

        void println(std::string message) {
            output(message, flags[0]);
        }
};