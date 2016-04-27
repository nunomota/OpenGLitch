#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

#include "../GlmStrings/GlmStrings.h"

/** This class is used to print any message
  * with a specific flag (warning, error, etc).
  */
class Reporter {
    private:
        static GlmStrings glm_strings_;

        static void output(std::string message, std::string flag) {
            std::ostringstream final_message;
            final_message << '[' << flag << ']' << " " << message;
            std::cout << final_message.str() << std::endl;
        }

    public:
        static void println(std::string message, std::string flag) {
            output(message, flag);
        }

        static void println(glm::vec3 vector, std::string flag) {
            output(glm_strings_.create(vector), flag);
        }

        static void println(glm::mat4 matrix, std::string flag) {
            output(glm_strings_.create(matrix), flag);
        }

        static void println(std::string message) {
            output(message, "Info");
        }

        static void println(glm::vec3 vector) {
            output(glm_strings_.create(vector), "Info");
        }

        static void println(glm::mat4 matrix) {
            output(glm_strings_.create(matrix), "Info");
        }

        static void print_special(std::string message) {
            std::cout << message << std::endl;
        }

        static void new_line() {
            std::cout << std::endl;
        };
};