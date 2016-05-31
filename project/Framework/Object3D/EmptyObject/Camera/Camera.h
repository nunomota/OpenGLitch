#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

#define PI 3.14f

class Camera: public EmptyObject {
    private:
        static const int default_width = 600;

        const float default_fovy = 45.0f;
        const float default_aspect = 1.0f;
        const float default_far_distance = 10.0f;

        float fovy_;
        float aspect_;
        float near_;
        float far_;

        int width_, height_;

        glm::mat4 view_;
        glm::mat4 projection_;

        FrameBuffer framebuffer;
        FrameBuffer shadowbuffer;
        GLuint render_texture_id;
        GLuint shadow_texture_id;

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
        Camera(float fovy = 45.0f, float aspect = 1.0f, float near = 0.1f, float far = 100.0f) {
            fovy_ = (fovy > 0)? fovy : default_fovy;
            aspect_ = (aspect > 0)? aspect : default_aspect;
            near_ = near;
            far_ = (far > near)? far : near + default_far_distance;
            recalculateProjectionMatrix();

            width_ = default_width;
            height_ = (float)width_ / aspect;
            render_texture_id = framebuffer.Init(width_, height_);
            shadow_texture_id = shadowbuffer.Init(width_, height_);
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

        void bindRenderBuffer() {
            framebuffer.Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void unbindRenderBuffer() {
            framebuffer.Unbind();
        }

        void bindShadowBuffer() {
            shadowbuffer.Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void unbindShadowBuffer() {
            shadowbuffer.Unbind();
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

        GLuint getShadowTextureID() {
            return shadow_texture_id;
        }

        void Cleanup() {
            framebuffer.Cleanup();
            shadowbuffer.Cleanup();
        }
};