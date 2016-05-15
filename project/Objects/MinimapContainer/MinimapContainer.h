#pragma once

class MinimapContainer {
    private:
        Minimap* minimap_bg;
        LiveViewer* minimap;
        Camera* viewer_camera;
        Camera* target_camera;

    public:
        void setBackground(Minimap* minimap) {
            minimap_bg = minimap;
        }

        void setViewer(Camera* new_viewer, LiveViewer* new_viewer_texture) {
            viewer_camera = new_viewer;
            minimap = new_viewer_texture;
        }

        void setTargetCamera(Camera* new_target) {
            target_camera = new_target;
        }

        void setup() {
            minimap->rotate(vec3(90.0f, 0.0f, 0.0f));
            minimap->translate(vec3(-0.75f, 0.75f, 0.0f));
            minimap->scale(vec3(-0.79f, 0.0f, -0.79f));

            minimapBg->rotate(vec3(90.0f, 0.0f, 0.0f));
            minimapBg->translate(vec3(-0.75f, 0.75f, 0.1f));
            minimapBg->scale(vec3(-0.75f, 0.0f, -0.75f));
        }

        void update() {
            // make minimap camera follow the main camera
            Transform* target_transform = target_camera->getTransform();
            Transform* viewer_transform = viewer_camera->getTransform();
            vec3 viewer_rotation = viewer_transform->getRotation();
            viewer_rotation->setPosition(target_transform->getPosition() + vec3(0.0f, 3.0f, 0.0f));
            viewer_rotation->setRotation(vec3(viewer_rotation.x, viewer_rotation.y, target_transform->getRotation().y));
        }
};