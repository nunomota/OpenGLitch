#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

#define PI 3.14f

class Camera: public Object3D {
    private:
        Reporter reporter;

        const float default_fovy = 45.0f;
        const float default_aspect = 16.0f/9.0f;
        const float default_far_distance = 10.0f;

        float fovy_;
        float aspect_;
        float near_;
        float far_;

        glm::mat4 view_;
        glm::mat4 projection_;

        void recalculateViewMatrix() {
            view_ = IDENTITY_MATRIX;
            view_[0][0] = -transform.getPosition().x;
            view_[1][1] = -transform.getPosition().y;
            view_[2][2] = -transform.getPosition().z;
            reporter.println("Recalculating view matrix");
            reporter.println(view_);
        }

        void recalculateProjectionMatrix() {
            float top = near_ * tan((PI/180.0f) * (fovy_/2.0f));
            float bottom = -top;
            float right = top * aspect_;
            float left = -right;

            projection_ = IDENTITY_MATRIX;
            projection_[0][0] = (2.0f * near_) / (right-left);
            projection_[1][1] = (2.0f * near_) / (top-bottom);
            projection_[2][2] = -1.0f * ((far_ + near_) / (far_ - near_));
            projection_[3][3] = 0.0f;
            projection_[2][0] = (right + left) / (right - left);
            projection_[2][1] = (top + bottom) / (top - bottom);
            projection_[3][2] = (-2.0f * far_ * near_) / (far_ - near_);
            projection_[2][3] = -1.0f;
        }

    public:
        Camera(float fovy, float aspect, float near, float far) {
            fovy_ = (fovy > 0)? fovy : default_fovy;
            aspect_ = (aspect > 0)? aspect : default_aspect;
            near_ = near;
            far_ = (far > near)? far : near + default_far_distance;
            recalculateViewMatrix();
            recalculateProjectionMatrix();
        }

        void setFov(float new_fov) {
            if (new_fov > 0) {
                fovy_ = new_fov;
                recalculateProjectionMatrix();
            }
        }

        void setAspect(float new_aspect) {
            if (new_aspect > 0) {
                aspect_ = new_aspect;
                recalculateProjectionMatrix();
            }
        }

        void setNear(float new_near_plane) {
            if (new_near_plane < far_) {
                near_ = new_near_plane;
                recalculateProjectionMatrix();
            }
        }

        void setFar(float new_far_plane) {
            if (new_far_plane > near_) {
                far_ = new_far_plane;
                recalculateProjectionMatrix();
            }
        }

        glm::mat4 getViewMatrix() {
            recalculateViewMatrix(); // TODO this should be recalculated when Transform gets updated, instead of here
            return view_;
        }

        glm::mat4 getProjectionMatrix() {
            return projection_;
        }
};