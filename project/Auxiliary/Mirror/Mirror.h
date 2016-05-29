#pragma once

using namespace glm;

class Mirror {
    private:
        Camera* target_camera;
        Camera* mirror_camera;

        vec3 prev_camera_position;

        void mirrorTargetTransform() {
            Transform* mirror_camera_transform = mirror_camera->getTransform();

            Transform* target_camera_transform = target_camera->getTransform();
            vec3 target_camera_position = target_camera_transform->getPosition();
            vec3 target_camera_rotation = target_camera_transform->getRotation();

            mirror_camera_transform->setPosition(vec3(target_camera_position.x, -target_camera_position.y, target_camera_position.z));
            mirror_camera_transform->setRotation(vec3(-target_camera_rotation.x, target_camera_rotation.y, target_camera_rotation.z));
            mirror_camera_transform->setScale(target_camera_transform->getScale());
        }

    public:
        void setup() {
            mirror_camera->setIgnoreTag(1); // ignore water tag
            mirrorTargetTransform();
            cout << target_camera->getTransform()->to_string() << endl;
            cout << mirror_camera->getTransform()->to_string() << endl;
        }

        void update() {
            mirrorTargetTransform();
            vec3 cur_camera_position = target_camera->getTransform()->getPosition();
            if ((cur_camera_position.y < 0.0f && prev_camera_position.y >= 0.0f) ||
                (cur_camera_position.y >= 0.0f && prev_camera_position.y < 0.0f)) {
                mirror_camera->enableClipping(-1.0f * mirror_camera->getClipPlane());
            }
            prev_camera_position = cur_camera_position;
        }

        void setTargetCamera(Camera* new_target_camera) {
            target_camera = new_target_camera;
            prev_camera_position = target_camera->getTransform()->getPosition();
        }

        void setMirrorCamera(Camera* new_mirror_camera) {
            mirror_camera = new_mirror_camera;
        }

        void setClipPlane(vec4 new_clip_plane) {
            mirror_camera->enableClipping(new_clip_plane);
        }

        Camera* getMirrorCamera() {
            return mirror_camera;
        }

        GLuint getMirrorTextureID() {
            return mirror_camera->getRenderTextureID();
        }
};