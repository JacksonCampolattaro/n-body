//
// Created by jackcamp on 5/21/19.
//

#include <thread>
#include "Controller.h"


using std::cout;
using std::endl;

Controller::Controller(Model *model, vector<Body> *bodies, View *view, Recorder *recorder) {

    this->model = model;
    this->bodies = bodies;
    this->view = view;
    this->recorder = recorder;

}

void Controller::run() {

    cout << "Starting simulation" << endl;
    cout << "Calculating " << numFrames << " frames" << endl;
    cout << "'.' Represents the controller running in the background." << endl;
    cout << "Signals sent from inside the simulation will be printed here." << endl;

    std::thread t(&Controller::increment, this);

    for (int i = 0; i < 2048; ++i) {
        usleep(1000);
        cout << "." << endl;
    }

    t.join();
}

void Controller::increment() {

    cout << "Preparing to calculate frame " << frameNum << "/" << numFrames << endl;
    ///usleep(1);

    model->increment(bodies);
    finishFrame();
}

void Controller::finishFrame() {

    cout << "Finished calculating frame " << frameNum << "/" << numFrames << endl;

    frameNum++;

    if (frameNum <= numFrames) {
        increment();
    }
}





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*void Controller::run() {

    // Preparation
    // TODO I need to reimplement this
    //model->preCalculate(bodies); // Enables leapfrog integration

    // Incrementing the simulation
    int maxFrames = 0;
    int cycle = 0;
    bool shouldContinue = true;
    while ((0 == maxFrames || maxFrames > cycle) && shouldContinue) {
        cycle++;

        // Starting the process of calculating forces in a separate thread
        model->increment(bodies);
        ///std::thread calcThread(&Model::increment, model, bodies);

        // The list of bodies is drawn, based on their previous position
        vector<Drawable *> drawables;
        for (int i = 0; i < bodies->size(); ++i) {
            drawables.push_back(&(*bodies)[i]);
        }
        shouldContinue = view->draw(drawables);
        ///shouldContinue = view->draw(*(std::vector<Drawable*> *)&bodies); // A copy of the list of bodies is passed to the view


        // Record the new drawing if a recorder is attached
        if (nullptr != recorder) {
            recorder->renderFrame(); // This adds whatever is currently on the screen to the video
        }

        // Wait for calculation to stop before starting over
        ///calcThread.join();

        // Updating positions
        #pragma omp parallel for
        for (int b = 0; b < bodies->size(); ++b) {

            (*bodies)[b].shiftBuffers();
        }


    }

    if (nullptr != recorder) {
        recorder->completeVideo();
    }
}*/


