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
        static const bool is_enabled = true;

        static void output(std::string message, std::string flag) {
            if (is_enabled) {
                std::ostringstream final_message;
                final_message << '[' << flag << ']' << " " << message;
                std::cout << final_message.str() << std::endl;
            }
        }

    public:
        static void println(std::string message, std::string flag) {
            Reporter::output(message, flag);
        }

        static void println(glm::vec3 vector, std::string flag) {
            Reporter::output(glm_strings_.create(vector), flag);
        }

        static void println(glm::mat4 matrix, std::string flag) {
            Reporter::output(glm_strings_.create(matrix), flag);
        }

        static void println(std::string message) {
            Reporter::output(message, "Info");
        }

        static void println(glm::vec3 vector) {
            Reporter::output(glm_strings_.create(vector), "Info");
        }

        static void println(glm::mat4 matrix) {
            Reporter::output(glm_strings_.create(matrix), "Info");
        }

        static void print_special(std::string message) {
            if (is_enabled) std::cout << message << std::endl;
        }

        static void new_line() {
            if (is_enabled) std::cout << std::endl;
        }
};