#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Camera: public Object3D {
    private:
        float fovy_;
        float aspect_;
        float near_;
        float far_;

        glm::mat4 view_;
        glm::mat4 projection_;

    public:
        Camera(float fovy, float aspect, float near, float far) {
            fovy_ = fovy;
            aspect_ = aspect;
            near_ = near;
            far_ = (far > near)? far : near;
        }

        void setFov(float new_fov) {
            fovy_ = new_fov;
        }

        void setAspect(float new_aspect) {
            aspect_ = new_aspect;
        }

        void setNear(float new_near_plane) {
            if (new_near_plane < far_) near_ = new_near_plane;
        }

        void setFar(float new_far_plane) {
            if (new_far_plane > near_) far_ = new_far_plane;
        }

        glm::mat4 getViewMatrix() {
            return view_;
        }

        glm::mat4 getProjectionMatrix() {
            return projection_;
        }
};