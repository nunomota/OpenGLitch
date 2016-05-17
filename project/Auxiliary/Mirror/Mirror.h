#pragma once

class Mirror {
    private:
        Camera* target_camera;
        Camera* mirror_camera;

    public:
        void setup() {
            // TODO enable clipping plane for camera
        }

        void update() {
            // TODO mirror camera transform
        }

        void setTargetCamera(Camera* new_target_camera) {
            target_camera = new_target_camera;
        }

        void setMirrorCamera(Camera* new_mirror_camera) {
            mirror_camera = new_mirror_camera;
        }
};