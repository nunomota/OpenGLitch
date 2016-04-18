#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

#include "../GlmStrings/GlmStrings.h"

/** This class is used to print any message
  * with a specific flag (warning, error, etc).
  */
class Reporter {
    private:
        GlmStrings glm_strings_;

        const std::string default_flag_ = "Info";
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

        void println(glm::vec3 vector, std::string flag) {
            output(glm_strings_.create(vector), flag);
        }

        void println(glm::mat4 matrix, std::string flag) {
            output(glm_strings_.create(matrix), flag);
        }

        void println(std::string message) {
            output(message, default_flag_);
        }

        void println(glm::vec3 vector) {
            output(glm_strings_.create(vector), default_flag_);
        }

        void println(glm::mat4 matrix) {
            output(glm_strings_.create(matrix), default_flag_);
        }

        void print_special(std::string message) {
            std::cout << message << std::endl;
        }

        void new_line() {
            std::cout << std::endl;
        };
};