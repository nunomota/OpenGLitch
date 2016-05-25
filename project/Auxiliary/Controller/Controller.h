#pragma once

using namespace glm;

class Controller {
    private:
        Transform* target_transform;
        Time* time;
        Terrain* terrain;

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

            int nb_component;
            int width,height;

            unsigned char* image; // 3rgb colors per pixel in this one dimensional array
            string height_tex = "perlin.jpg"; // 500x500
            image = stbi_load(height_tex.c_str(), &width, &height, &nb_component, 0);
            printf("%d\n", image[4]);

            // calculate index for x,z in 1 dim array image
            int index_x = target_position.x * width/2;
            int index_z = target_position.z * height/2;

            if(target_position.x < 0){
                index_x = -target_position.x * width/2;
            }else{
                index_x = (target_position.x * width/2) + width/2; 
            }

            if(target_position.z < 0){
                index_z = -target_position.z * height/2;
            }else{
                index_z = (target_position.z * width/2) + width/2;
            }
            printf("x: %d\n", index_x);
            printf("z: %d\n", index_z);   
            int array_index = index_x * index_z * 3;

            float tex_height = 0.0f;
            if (array_index < width * height * 3){
                printf("arr: %d\n", image[array_index]);            
                tex_height = image[array_index]/255.0f; // normalize height value in image array
                printf("tex: %f\n", tex_height);
                printf("y: %f\n", target_position.y);
            }

            if (target_position.y > tex_height) return false;
            target_transform->setPosition(vec3(target_position.x, 0.0f, target_position.z));
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
            GlmStrings glmStrings;
            cout << glmStrings.create(drifts) << endl;
        }

        void setVelocity(float new_velocity) {
            velocity = new_velocity;
        }
};