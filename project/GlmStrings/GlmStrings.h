#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class GlmStrings {
    private:
        std::string pretty_float(float number) {
            std::ostringstream number_representation;
            number_representation << std::fixed << std::setprecision(2);
            if (number >= 0) number_representation << "+" << number;
            else number_representation << number;
            return number_representation.str();
        }

    public:
        std::string create(glm::vec3 vector) {
            std::ostringstream string_representation;
            string_representation << "(" << std::fixed << std::setprecision(2)
                                  << vector.x << ", "
                                  << vector.y << ", "
                                  << vector.z 
                                  << ")";
            return string_representation.str();
        }

        std::string create_single(glm::mat4 matrix) {
            std::ostringstream string_representation;
            string_representation << std::fixed << std::setprecision(2);
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

        std::string* create_lines(glm::mat4 matrix) {
            std::string* lines = new std::string[4];
            for (int x = 0; x < 4; x++) {
                std::ostringstream line_representation;
                line_representation << std::fixed << std::setprecision(2);
                line_representation << "[";
                for (int y = 0; y < 4; y++) {
                    float element = float(matrix[x][y]);
                    if (y < 3) line_representation << pretty_float(element) << ", ";
                    else line_representation << pretty_float(element);
                }
                if (x < 3) line_representation << "]" << std::endl;
                else line_representation << "]";
                lines[x] = line_representation.str();
            }
            return lines;
        }
};