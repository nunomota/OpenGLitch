#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class DirectionalLight: public Light {
    private:
        glm::vec3 default_direction_ = glm::vec3(0.0f, 0.0f, 1.0f);
        Camera* shadow_camera;
        int count = 1;
        float multiplier = 1; // TODO REMOVE

        glm::mat4 getRotationMatrix() {
            glm::mat4 rotation_matrix = IDENTITY_MATRIX;

            rotation_matrix = glm::rotate(rotation_matrix, glm::radians(transform.getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
            rotation_matrix = glm::rotate(rotation_matrix, glm::radians(transform.getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
            rotation_matrix = glm::rotate(rotation_matrix, glm::radians(transform.getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));

            return rotation_matrix;
        }

    public:
        void setShadowCamera(Camera* new_shadow_camera) {
            shadow_camera = new_shadow_camera;
        }

        glm::vec3 getDirection() {
            glm::mat4 rotation_matrix = getRotationMatrix();
            glm::vec4 transformed_direction = rotation_matrix * glm::vec4(default_direction_, 1.0f);
            return glm::vec3(transformed_direction);
        }

        Camera* getShadowCamera() {
            return shadow_camera;
        }

        glm::mat4 getDepthMVP() {
            if(shadow_camera){


                glm::mat4 view = shadow_camera->getViewMatrix();
                GlmStrings bstring;
                //cout << bstring.create(view) << "\n" << endl;

                glm::mat4 projection = shadow_camera->getProjectionMatrix();
                projection = glm::ortho(-1.0f,1.0f,-1.0f,1.0f,-1.0f,2.0f);
                //GlmStrings astring;
                //cout<< "projection: \n" << astring.create(projection) << "\n" << endl;

                glm::mat4 model = getTransform()->getModelMatrix();
                //model = glm::translate(view * model, glm::vec3(0.0f, 0.1f, 0.1f));
                //cout << astring.create(model) << "\n" << endl;

                //return projection * model;
                multiplier = multiplier + 1.0f;
                //return glm::translate(IDENTITY_MATRIX, glm::vec3(0.0001f * multiplier, -0.0001f * multiplier,0.0001f * multiplier));

                return projection * view * model;


                if (count == 0){


                    count = 1;
                    //Reporter::println("DEPTH 0");

                    return IDENTITY_MATRIX;



                } else{

                    count = 0;
                    //Reporter::println("DEPTH 1");

                    glm::mat4 view = shadow_camera->getViewMatrix();
                    GlmStrings bstring;
                    //cout << bstring.create(view) << "\n" << endl;

                    glm::mat4 projection = shadow_camera->getProjectionMatrix();
                    projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,-10.0f,20.0f);
                    //GlmStrings astring;
                    //cout<< "projection: \n" << astring.create(projection) << "\n" << endl;

                    glm::mat4 model = getTransform()->getModelMatrix();
                    //model = glm::translate(view * model, glm::vec3(0.0f, 0.1f, 0.1f));
                    //cout << astring.create(model) << "\n" << endl;

                    //return projection * model;
                    multiplier = multiplier + 1.0f;
                    //return glm::translate(IDENTITY_MATRIX, glm::vec3(0.0001f * multiplier, -0.0001f * multiplier,0.0001f * multiplier));

                    return projection * view * model;
                }


            } else{
                Reporter::println("NULL shadow camera");
                return IDENTITY_MATRIX; 
            }
        }

};