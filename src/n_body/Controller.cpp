//
// Created by jackcamp on 5/21/19.
//

#include <thread>
#include "Controller.h"


using std::cout;
using std::endl;

Controller::Controller(Model *model, View *view, vector<Body *> bodies, Recorder *recorder) {

    this->model = model;
    this->view = view;
    this->bodies = bodies;
    this->recorder = recorder;

}

void Controller::run() {

    // Preparation
    model->preCalculate(bodies); // Enables leapfrog integration

    // Incrementing the simulation
    int maxFrames = 0;
    int cycle = 0;
    bool shouldContinue = true;
    while ((0 == maxFrames || maxFrames > cycle) && shouldContinue) {
        cycle++;

        // Starting the process of calculating forces in a separate thread
        std::thread calcThread(&Model::increment, model, bodies);

        // The list of bodies is drawn, based on their previous position
        shouldContinue = view->draw(*(std::vector<Drawable*> *)&bodies); // A copy of the list of bodies is passed to the view

        // Record the new drawing if a recorder is attached
        if (nullptr != recorder) {
            recorder->renderFrame(); // This adds whatever is currently on the screen to the video
        }

        // Wait for calculation to stop before starting over
        calcThread.join();

        // Updating positions
        #pragma omp parallel for
        for (int b = 0; b < bodies.size(); ++b) {

            bodies[b]->shiftBuffers();
        }

        m_signal_completed_frame.emit(cycle);


    }

    if (nullptr != recorder) {
        recorder->completeVideo();
    }
}

Controller::type_signal_completed_frame Controller::signal_completed_frame() {
    return m_signal_completed_frame;
}
