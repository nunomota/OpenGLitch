#pragma once

using namespace glm;

class MinimapContainer {
    private:
        Minimap* minimap_bg;
        LiveViewer* minimap;
        Camera* viewer_camera;
        Camera* target_camera;

    public:
        void setBackground(Minimap* new_bg) {
            minimap_bg = new_bg;
        }

        void setViewer(Camera* new_viewer, LiveViewer* new_viewer_texture) {
            viewer_camera = new_viewer;
            viewer_camera->setIgnoreTag(1);
            minimap = new_viewer_texture;
        }

        void setTargetCamera(Camera* new_target) {
            target_camera = new_target;
        }

        void setup() {
            viewer_camera->rotate(vec3(-90.0f, 0.0f, 0.0f));
            viewer_camera->getTransform()->setPosition(target_camera->getTransform()->getPosition());
            viewer_camera->translate(vec3(0.0f, 3.0f, 0.0f));
            
            minimap->rotate(vec3(90.0f, 0.0f, 0.0f));
            minimap->translate(vec3(-0.75f, 0.75f, 0.0f));
            minimap->scale(vec3(-0.79f, 0.0f, -0.79f));

            minimap_bg->rotate(vec3(90.0f, 0.0f, 0.0f));
            minimap_bg->translate(vec3(-0.75f, 0.75f, 0.1f));
            minimap_bg->scale(vec3(-0.75f, 0.0f, -0.75f));
        }

        void update() {
            // make minimap camera follow the main camera
            Transform* target_transform = target_camera->getTransform();
            Transform* viewer_transform = viewer_camera->getTransform();
            vec3 viewer_rotation = viewer_transform->getRotation();
            viewer_transform->setPosition(target_transform->getPosition() + vec3(0.0f, 3.0f, 0.0f));
            viewer_transform->setRotation(vec3(viewer_rotation.x, viewer_rotation.y, target_transform->getRotation().y));
        }
};