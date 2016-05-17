#pragma once

using namespace glm;

class Mirror {
    private:
        Camera* target_camera;
        Camera* mirror_camera;

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
            mirror_camera->enableClipping(vec4(0.0f, -1.0f, 0.0f, 0.1f));
            mirrorTargetTransform();
            cout << target_camera->getTransform()->to_string() << endl;
            cout << mirror_camera->getTransform()->to_string() << endl;
        }

        void update() {
            mirrorTargetTransform();
        }

        void setTargetCamera(Camera* new_target_camera) {
            target_camera = new_target_camera;
        }

        void setMirrorCamera(Camera* new_mirror_camera) {
            mirror_camera = new_mirror_camera;
        }

        GLuint getMirrorTextureID() {
            return mirror_camera->getRenderTextureID();
        }
};