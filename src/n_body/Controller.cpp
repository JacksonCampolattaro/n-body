//
// Created by jackcamp on 5/21/19.
//

#include <thread>
#include "Controller.h"


using std::cout;
using std::endl;

POCController::POCController(Model *model, View *view, vector<Body *> bodies, Recorder *recorder) {

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

void POCController::run() {

    // Preparation
    model->preCalculate(bodies); // Enables leapfrog integration

    // Incrementing the simulation
    int maxFrames = 0;
    int cycle = 0;
    bool shouldContinue = true;
    while ((0 == maxFrames || maxFrames > cycle) && shouldContinue) {
        cycle++;

        // Starting the process of calculating forces in a separate thread
        std::thread t(&Model::increment, model, bodies);

        // The list of bodies is drawn, based on their previous position
        shouldContinue = view->draw(*(std::vector<Drawable*> *)&bodies); // A copy of the list of bodies is passed to the view

        // Record the new drawing if a recorder is attached
        if (nullptr != recorder) {
            recorder->renderFrame(); // This adds whatever is currently on the screen to the video
        }

        // Wait for calculation to stop before starting over
        t.join();

    }

    if (nullptr != recorder) {
        recorder->completeVideo();
    }
}
