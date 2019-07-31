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
    SetWindowMinSize(100, 100);

    // Defining the camera
    camera.position = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.target = (Vector3) {0.0f, 10.0f, -1.0f};
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.type = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_FIRST_PERSON);

    // TODO is this meaningful if I'm not using raylib's loop?
    //SetTargetFPS(60);
}

void View::loop() {

    while (!WindowShouldClose()) {

        UpdateCamera(&camera);

        BeginDrawing();
        {

            ///ClearBackground(RAYWHITE);
            ClearBackground(BLACK);

            BeginMode3D(camera);
            {

                DrawGrid(10, 1.0f);

                // 3d Mode
                for (Body b : *drawables) {
                    b.draw();
                }

            }
            EndMode3D();

            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    CloseWindow();
}

void View::update() {

    UpdateCamera(&camera);

    BeginDrawing();
    {

        ///ClearBackground(RAYWHITE);
        ClearBackground(BLACK);

        BeginMode3D(camera);
        {

            DrawGrid(10, 1.0f);

            // 3d Mode
            for (Body b : *drawables) {
                b.draw();
            }

        }
        EndMode3D();

        DrawFPS(10, 10);
    }
    EndDrawing();

}

void View::closeWindow() {

    CloseWindow();
}

void View::setDrawables(std::vector<Body> *drawables) {

    this->drawables = drawables;
}
