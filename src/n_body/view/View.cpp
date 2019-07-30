//
// Created by jackcamp on 4/10/19.
//

#include <iostream>
#include <zconf.h>
#include "View.h"
#include "../scenarios/ScenarioStream.h"


View::View(int width, int height, const char *title) {

    this->width = width;
    this->height = height;
    this->title = title;
}

void View::createWindow() {

    // Setting configuration options
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

    // Creating the window
    InitWindow(width, height, title);

    // Configuration for the window
    SetWindowMinSize(320, 240);

    // Defining the camera
    camera.position = (Vector3) {4.0f, 2.0f, 4.0f};
    camera.target = (Vector3) {0.0f, -4.0f, -20.0f};
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.type = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_FIRST_PERSON); // Set a first person camera mode

    // TODO is this meaningful if I'm not using raylib's loop?
    SetTargetFPS(60);
}

void View::loop() {


    auto b = ScenarioStream::loadBodies("/home/jackcamp/CLionProjects/n_body/src/n_body/scenarios/threeBody.xml");

    while (!WindowShouldClose()) {

        UpdateCamera(&camera);

        BeginDrawing();
        {

            //ClearBackground(RAYWHITE);
            ClearBackground(BLACK);

            BeginMode3D(camera);
            {

                // 3d Mode

                draw(&b);

            }
            EndMode3D();

            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    CloseWindow();
}

void View::draw(std::vector<Body> *drawables) {

    for (Body b : *drawables) {
        b.draw();
    }

}
