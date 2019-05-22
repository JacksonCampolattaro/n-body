//
// Created by jackcamp on 5/21/19.
//

#include "Controller.h"


using std::cout;
using std::endl;

Controller::Controller(Model *model, View *view, vector<Body *> bodies, Recorder *recorder) {

    this->model = model;
    this->view = view;
    this->bodies = bodies;
    this->recorder = recorder;

    /*// Setting the rules of the universe
    physicsContext = new PhysicsContext();
    physicsContext->setT(0.01)->setG(0.02)->setPower(2);

    // Setting the parameters of the solving algorithm
    auto *solver = new BarnesHutSolver();
    solver->enableThreading();
    solver->setTheta(0.8);

    // Preparing the view
    view = new View();
    view->setTitle("Jackson Campolattaro's n-body Simulator")->setDimensions(glm::ivec2(1920, 1080));

    // Preparing the recorder
    Recorder *recorder = new Recorder(view, "/home/jackcamp/CLionProjects/n_body/src/n_body/staging/output.mp4");

    // Registering the bodies
    this->bodies = bodies;

    // Creating the model
    model = new Model(physicsContext, solver);*/

}

void Controller::run() {

    // Preparation
    model->preCalculate(bodies); // Enables leapfrog integration

    // Tracking
    clock_t startTime, endTime;
    double drawTime, renderTime, calcTime;
    cout << std::fixed << std::setprecision(3);

    // Incrementing the simulation
    int maxFrames = 36000;
    int cycle = 0;
    while (0 == maxFrames || maxFrames > cycle) {
        cycle++;

        startTime = clock();
        view->draw(*(std::vector<Drawable*> *)&bodies); // A copy of the list of bodies is passed to the view
        endTime = clock();
        drawTime = double(endTime - startTime) / CLOCKS_PER_SEC;

        startTime = clock();
        if (nullptr != recorder) { // Only render if a recorder is attached
            recorder->renderFrame(); // This adds whatever is currently on the screen to the video
        }
        endTime = clock();
        renderTime = double(endTime - startTime) / CLOCKS_PER_SEC;

        startTime = clock();
        model->increment(bodies); // The Model calculates and updates the positions of all bodies
        endTime = clock();
        calcTime = double(endTime - startTime) / CLOCKS_PER_SEC;

        cout << endl;
        cout << "Drawing:     " << drawTime << "s (" << 1 / drawTime << "hz)" << endl;
        cout << "Rendering:     " << renderTime << "s (" << 1 / renderTime << "hz)" << endl;
        cout << "Calculation: " << calcTime << "s (" << 1 / calcTime << "hz)" << endl;
        cout << "Total:       " << (drawTime + calcTime + renderTime) << "s (" << 1 / (drawTime + calcTime + renderTime) << "hz)" << endl;
        for (int k = 0; k < drawTime * 100; ++k) {
            cout << "o";
        }
        for (int k = 0; k < renderTime * 100; ++k) {
            cout << "0";
        }
        for (int j = 0; j < calcTime * 100; ++j) {
            cout << "+";
        }
        cout << endl;


        /*// FIXME This isn't actually being done in parallel!
        startTime = clock();
        std::vector<Drawable *> drawables = *(std::vector<Drawable *> *) &bodies;
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                // A copy of the list of bodies is passed to the renderer
                output->draw(drawables);
            }
            #pragma omp section
            {
                model->increment(bodies);
            }
        }

        endTime = clock();
        calcTime = double(endTime - startTime) / CLOCKS_PER_SEC;
        cout << calcTime << endl;*/


    }

    if (nullptr != recorder) {
        recorder->completeVideo();
    }
}
