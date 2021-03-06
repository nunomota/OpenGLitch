#pragma once

class Bezier {
    private:
        glm::vec3 control_point_0 = glm::vec3(0.0f);
        glm::vec3 control_point_1 = glm::vec3(0.0f);
        glm::vec3 control_point_2 = glm::vec3(0.0f);

        Time* time;
        float duration;
        float timestamp;

        glm::vec3 calculateIntermediatePoint() {
            glm::vec3 movement_vector = control_point_2 - control_point_0;
            return movement_vector/2.0f + glm::vec3(0.0f, 3.0f, 0.0f);
        }

        /** This method simply return the result of a quadratic
          * bezier function applied to the specified control
          * vectors:
          *
          * B(t) = (1-t)^2 * P0 + 2(1-t)(t) * P1 + t^2 * P2
          */
        glm::vec3 bezierFunction(float t) {
            glm::vec3 component_1 = pow(1.0f-t, 2.0f) * control_point_0;
            glm::vec3 component_2 = 2.0f * (1.0f-t) * t * control_point_1;
            glm::vec3 component_3 = pow(t, 2.0f) * control_point_2;
            return component_1 + component_2 + component_3;
        }

    public:
        Bezier(Time* new_time, float new_duration) {
            if (new_time) time = new_time;
            duration = new_duration;
            resetTime();
        }

        void setReferences(glm::vec3 start_vector, glm::vec3 final_vector) {
            control_point_0 = start_vector;
            control_point_2 = final_vector;
            control_point_1 = calculateIntermediatePoint();
        }

        void setReferences(glm::vec3 start_vector, glm::vec3 middle_vector, glm::vec3 final_vector) {
            control_point_0 = start_vector;
            control_point_1 = middle_vector;
            control_point_2 = final_vector;
        }

        void resetTime() {
            if (time) timestamp = time->getCurrentTime();
        }

        glm::vec3 getNextValue() {
            float current_time = time->getCurrentTime();
            float fraction = (current_time - timestamp) / duration;
            if (fraction <= 1.0f) return bezierFunction(fraction);
            else return control_point_2; 
        }
};