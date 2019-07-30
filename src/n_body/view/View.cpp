//
// Created by jackcamp on 4/10/19.
//

#include <iostream>
#include "View.h"


View::View(int width, int height, const char *title) {

    this->width = width;
    this->height = height;
    this->title = title;
}

void View::createWindow() {

    // Setting configuration options
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    // Creating the window
    InitWindow(width, height, title);

    // Configuration for the window
    SetWindowMinSize(320, 240);

    // Defining the camera
    camera.position = (Vector3){ 4.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 1.8f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.type = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_ORBITAL); // Set a first person camera mode

    // TODO is this meaningful if I'm not using raylib's loop?
    SetTargetFPS(60);
}

void View::loop() {

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(RAYWHITE);


        BeginMode3D(camera);

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();
}

void View::draw(std::vector<Drawable> drawables) {


}
