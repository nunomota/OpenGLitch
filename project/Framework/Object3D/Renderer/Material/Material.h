#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Material {
    protected:
        static const glm::vec3 default_color;
        static const float default_alpha;

        glm::vec3 ambience_color;
        glm::vec3 diffuse_color;
        glm::vec3 specular_color;

        float alpha;

    public:
        Material() {
            setColor(default_color);
            setAlpha(default_alpha);
        }

        void setAmbienceColor(glm::vec3 new_color) {
            ambience_color = new_color;
        }

        void setDiffuseColor(glm::vec3 new_color) {
            diffuse_color = new_color;
        }

        void setSpecularColor(glm::vec3 new_color) {
            specular_color = new_color;
        }

        void setColor(glm::vec3 new_color) {
            setAmbienceColor(new_color);
            setDiffuseColor(new_color);
            setSpecularColor(new_color);
        }

        void setAlpha(float new_alpha) {
            alpha = new_alpha;
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

        float getAlpha() {
            return alpha;
        }
};
const glm::vec3 Material::default_color = glm::vec3(1.0f, 1.0f, 1.0f);
const float Material::default_alpha = 60.0f;