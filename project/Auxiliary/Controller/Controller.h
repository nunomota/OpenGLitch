#pragma once

using namespace glm;

class Controller {
    private:
        Transform* target_transform;
        Time* time;
        Terrain* terrain;

        float grounded_height = 0.08f;
        float velocity = 2.0f;
        vec3 movement_vector = vec3(0.0f);

        float drift_max = 1.0f;
        vec3 drifts = vec3(0.0f);

        void updateDrifts(vec3 update_vector, bool increment) {
            if (increment) drifts = drifts + update_vector*time->getDeltaTime();
            else drifts = drifts + (vec3(0.0f) - drifts) * time->getDeltaTime();

            // constrain drift values
            if (drifts.x < -drift_max) drifts.x = -drift_max;
            else if (drifts.x > drift_max) drifts.x = drift_max;
            if (drifts.y < -drift_max) drifts.y = -drift_max;
            else if (drifts.y > drift_max) drifts.y = drift_max;
            if (drifts.z < -drift_max) drifts.z = -drift_max;
            else if (drifts.z > drift_max) drifts.z = drift_max;
        }

        bool isGrounded() {
            // TODO get terrain texture at (x, z)
            // TODO if height <= texture height, return true and height = 0
            // TODO if height > texture height, return false
            vec3 target_position = target_transform->getPosition();
            float terrain_height = terrain->getHeight(target_position);
            cout << terrain->getTransform()->getPosition().x << endl;
            //cout << "Current height: " << terrain_height << endl;
            
            if (target_position.y > grounded_height) return false;
            target_transform->setPosition(vec3(target_position.x, grounded_height, target_position.z));
            return true;
        }

    public:
        void setTarget(Transform* new_target_transform) {
            target_transform = new_target_transform;
        }

        void setTime(Time* new_time) {
            time = new_time;
        }

        void setTerrain(Terrain* new_terrain) {
            terrain = new_terrain;
        }

        void translate(vec3 movement_vector, bool moving) {
            vec3 cur_mov_vector = normalize(movement_vector);
            updateDrifts(cur_mov_vector, moving);
        }

        void update() {
            vec3 cur_position = target_transform->getPosition();
            vec3 mov_vector = drifts * velocity * time->getDeltaTime();
            vec3 next_position = cur_position + mov_vector;
            target_transform->setPosition(next_position);
            if (!isGrounded()) {
                translate(vec3(0.0f, -1.0f, 0.0f), true);
            }
        }

        void setVelocity(float new_velocity) {
            velocity = new_velocity;
        }
};