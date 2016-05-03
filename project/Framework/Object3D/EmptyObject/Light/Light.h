#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Light: public EmptyObject {
    private:
        static const glm::vec3 default_color;

    protected:
        glm::vec3 ambience_color;
        glm::vec3 diffuse_color;
        glm::vec3 specular_color;

    public:
        Light () {
            setColor(default_color);
        }

        void setAmbienceColor(glm::vec3 new_color) {
            ambience_color(new_color);
        }

        void setDiffuseColor(glm::vec3 new_color) {
            diffuse_color(new_color);
        }

        void setSpecularColor(glm::vec3 new_color) {
            specular_color(new_color);
        }

        void setColor(glm::vec3 new_color) {
            setAmbienceColor(new_color);
            setDiffuseColor(new_color);
            setSpecularColor(new_color);
        }

        glm::vec3 getAmbienceColor() {
            return ambience_color;
        }

        glm::vec3 getDiffuseColor() {
            return diffuse_color;
        }

        glm::vec3 getSpecularColor() {
            return specular_color;
        }
};
const glm::vec3 default_color = glm::vec3(1.0f, 1.0f, 1.0f);