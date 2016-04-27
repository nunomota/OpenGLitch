#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class WorldInstance: public World {
    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            Camera* camera = instantiate(new Camera(45.0f, 1.0f, 0.1f, 100.0f));
            Cube* cube = instantiate(new Cube());
            Cube* cube2 = instantiate(new Cube());
            Cube* cube3 = instantiate(new Cube());
            setCamera(camera);

            camera->translate(glm::vec3(0.0f, 0.0f, 8.0f));
            camera->scale(glm::vec3(-0.2f, -0.2f, -0.2f));

            cube->scale(glm::vec3(-0.5f, -0.5f, -0.5f));
            cube->translate(glm::vec3(0.0f, 0.0f, -2.0f));
            cube->rotate(glm::vec3(45.0f, 45.0f, 0.0f));

            cube2->scale(glm::vec3(-0.5f, -0.5f, -0.5f));
            cube2->translate(glm::vec3(-2.0f, 0.0f, -2.0f));
            cube2->rotate(glm::vec3(-45.0f, 0.0f, 0.0f));

            cube3->scale(glm::vec3(-0.5f, -0.5f, -0.5f));
            cube3->translate(glm::vec3(2.0f, 0.0f, -2.0f));
            cube3->rotate(glm::vec3(45.0f, 0.0f, 0.0f));
        }

        // method called every frame
        void Update() {
            if (getKeyDown(Keyboard::A)) {
                Reporter::println("A is being pressed");
            }

            if (getKeyPressed(Keyboard::B)) {
                Reporter::println("B was pressed");
            }
        }    
};