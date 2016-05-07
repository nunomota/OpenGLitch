#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

class WorldInstance: public World {
    private:
        Camera* camera;
        Camera* camera2;
        Cube* cube;
        Cube* cube2;
        Cube* cube3;

    protected:

        // method called only once
        void Start() {
            Reporter::println("Start method called");
            camera = instantiate(new Camera(45.0f, 1.0f, 0.1f, 100.0f));
            camera2 = instantiate(new Camera(45.0f, 1.0f, 0.1f, 100.0f));
            cube = instantiate(new Cube());
            cube2 = instantiate(new Cube());
            cube3 = instantiate(new Cube());
            setCamera(camera);

            camera->translate(vec3(0.0f, 0.0f, 8.0f));
            camera->scale(vec3(-0.2f, -0.2f, -0.2f));

            camera2->translate(vec3(-2.0f, 0.0f, 8.0f));
            camera2->rotate(vec3(0.0f, -45.0f, 0.0f));
            camera2->scale(vec3(-0.2f, -0.2f, -0.2f));

            cube->scale(vec3(-0.5f, -0.5f, -0.5f));
            cube->translate(vec3(0.0f, 0.0f, -2.0f));
            cube->rotate(vec3(45.0f, 45.0f, 0.0f));

            cube2->scale(vec3(-0.5f, -0.5f, -0.5f));
            cube2->translate(vec3(-2.0f, 0.0f, -2.0f));
            cube2->rotate(vec3(-45.0f, 0.0f, 0.0f));

            cube3->scale(vec3(-0.5f, -0.5f, -0.5f));
            cube3->translate(vec3(2.0f, 0.0f, -2.0f));
            cube3->rotate(vec3(45.0f, 0.0f, 0.0f));
        }

        // method called every frame
        void Update() {
            if (getKeyDown(Keyboard::A)) {
                Reporter::println("A is being held down");
                //setCamera(camera);
            }

            if (getKeyPressed(Keyboard::B)) {
                Reporter::println("B was pressed");
                //setCamera(camera2);
            }

            if (getMouseButtonPressed(Mouse::RIGHT)) {
                Reporter::println("Mouse right was pressed");
            }

            if (getMouseButtonPressed(Mouse::LEFT)) {
                Reporter::println("Mouse left was pressed");
            }

            if (getKeyPressed(Keyboard::C)) {
                Renderer* cube_renderer = cube->getRenderer();
                cube_renderer->setState(!cube_renderer->getState());
            }

            cube->rotate(vec3(20.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
            cube2->rotate(vec3(0.0f, 25.0f, 0.0f) * getTime()->getDeltaTime());
            cube3->rotate(vec3(0.0f, 0.0f, 30.0f) * getTime()->getDeltaTime());
            //grid->rotate(vec3(20.0f, 0.0f, 0.0f) * getTime()->getDeltaTime());
        }    
};