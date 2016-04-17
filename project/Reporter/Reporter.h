#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

/** This class is used to print any message
  * with a specific flag (warning, error, etc).
  */
class Reporter {
    private:
        const std::string default_flag_ = "Info";
        const char flag_delimiters[2]{'[', ']'};

        void output(std::string message, std::string flag) {
            std::ostringstream final_message;
            final_message << flag_delimiters[0] << flag << flag_delimiters[1] << " " << message;
            std::cout << final_message.str() << std::endl;
        }

        std::string glm_to_string(glm::vec3 vector) {
            std::ostringstream string_representation;
            string_representation << "vec3(" << std::fixed << std::setprecision(2)
                                  << vector.x << ", "
                                  << vector.y << ", "
                                  << vector.z 
                                  << ")";
            return string_representation.str();
        }

        std::string glm_to_string(glm::mat4 matrix) {
            std::ostringstream string_representation;
            string_representation << "Mat4" << std::endl << std::fixed << std::setprecision(2);
            for (int x = 0; x < 4; x++) {
                string_representation << "[";
                for (int y = 0; y < 4; y++) {
                    float element = float(matrix[x][y]);
                    if (y < 3) string_representation << pretty_float(element) << ", ";
                    else string_representation << pretty_float(element);
                }
                if (x < 3) string_representation << "]" << std::endl;
                else string_representation << "]";
            }
            return string_representation.str();
        }

        std::string pretty_float(float number) {
            std::ostringstream number_representation;
            number_representation << std::fixed << std::setprecision(2);
            if (number >= 0) number_representation << "+" << number;
            else number_representation << number;
            return number_representation.str();
        }

    public:
        void println(std::string message, std::string flag) {
            output(message, flag);
        }

        void println(glm::vec3 vector, std::string flag) {
            output(glm_to_string(vector), flag);
        }

        void println(glm::mat4 matrix, std::string flag) {
            output(glm_to_string(matrix), flag);
        }

        void println(std::string message) {
            output(message, default_flag_);
        }

        void println(glm::vec3 vector) {
            output(glm_to_string(vector), default_flag_);
        }

        void println(glm::mat4 matrix) {
            output(glm_to_string(matrix), default_flag_);
        }

        void print_special(std::string message) {
            std::cout << message << std::endl;
        }

        void new_line() {
            std::cout << std::endl;
        };
};