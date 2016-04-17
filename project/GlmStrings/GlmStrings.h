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
            string_representation << "vec3(" << std::fixed << std::setprecision(2)
                                  << vector.x << ", "
                                  << vector.y << ", "
                                  << vector.z 
                                  << ")";
            return string_representation.str();
        }

        std::string create(glm::mat4 matrix) {
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
};