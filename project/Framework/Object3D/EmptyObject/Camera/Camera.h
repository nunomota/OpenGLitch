#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

#define PI 3.14f

class Camera: public EmptyObject {
    private:
        static const int default_width = 600;

        static const float default_fovy;
        static const float default_aspect;
        static const float default_near_distance;
        static const float default_far_distance;

        float fovy_;
        float aspect_;
        float near_;
        float far_;

        int width_, height_;

        glm::mat4 view_;
        glm::mat4 projection_;

        FrameBuffer framebuffer;
        GLuint render_texture_id;

        bool is_clipping_enabled = false;
        glm::vec4 clip_plane = glm::vec4(0.0f);
        int ignore_tag = -1;

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

        void setAspect() {
            float new_aspect = (float)width_/(float)height_;
            if (new_aspect > 0) {
                aspect_ = new_aspect;
                recalculateProjectionMatrix();
            }
        }

    public:
        Camera(float fovy = default_fovy, float aspect = default_aspect, float near = default_near_distance, float far = default_far_distance) {
            fovy_ = (fovy > 0)? fovy : default_fovy;
            aspect_ = (aspect > 0)? aspect : default_aspect;
            near_ = near;
            far_ = (far > near)? far : near + default_far_distance;
            recalculateProjectionMatrix();

            width_ = default_width;
            height_ = (float)width_ / aspect;
            render_texture_id = framebuffer.Init(width_, height_);
        }

        void setFov(float new_fov) {
            if (new_fov > 0) {
                fovy_ = new_fov;
                recalculateProjectionMatrix();
            }
        }

        void setScreenDimensions(int new_width, int new_height) {
            if (new_width > 0 && new_height > 0) {
                width_ = new_width;
                height_ = new_height;
                setAspect();
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

        void setIgnoreTag(int new_ignore_tag) {
            ignore_tag = new_ignore_tag;
        }

        void enableClipping(glm::vec4 new_clip_plane) {
            clip_plane = new_clip_plane;
            is_clipping_enabled = true;
        }

        void bindRenderBuffer() {
            framebuffer.Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void unbindRenderBuffer() {
            framebuffer.Unbind();
        }

        glm::mat4 getViewMatrix() {
            // TODO maybe calculate this matrix from Transform.getModelMatrix()?
            return transform.getInvertedModelMatrix();
        }

        glm::mat4 getProjectionMatrix() {
            return projection_;
        }

        GLuint getRenderTextureID() {
            return render_texture_id;
        }

        glm::vec4 getClipPlane() {
            return clip_plane;
        }

        bool getClippingState() {
            return is_clipping_enabled;
        }

        int getIgnoreTag() {
            return ignore_tag;
        }

        void Cleanup() {
            framebuffer.Cleanup();
        }
};
const float Camera::default_fovy          = 45.0f;
const float Camera::default_aspect        = 1.0f;
const float Camera::default_near_distance = 0.01f;
const float Camera::default_far_distance  = 100.0f;
