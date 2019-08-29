//
// Created by jackcamp on 5/21/19.
//

#include <thread>
#include "Controller.h"


using std::cout;
using std::endl;

Controller::Controller(vector<Body> *bodies, PhysicsContext *physics, Solver *solver) {

    /*solver->signal_preparing_solver().connect(sigc::mem_fun(*this, &Controller::on_preparing_solver));
    solver->signal_solving().connect(sigc::mem_fun(*this, &Controller::on_solving));
    solver->signal_shifting_buffers().connect(sigc::mem_fun(*this, &Controller::on_shifting_buffers));
    solver->signal_complete().connect(sigc::mem_fun(*this, &Controller::on_solver_complete));*/

    this->solver = solver;
    this->physics = physics;
    this->bodies = bodies;

}

void Controller::run() {


    /*auto v = new View(1000, 1000, "title");
    v->setDrawables(bodies);*/

    /*auto spawnWindow = [](View *v) {

        v->createWindow();
        v->loop();
    };

    std::thread rendering_thread(spawnWindow, v);
    std::thread calculation_thread(&Controller::increment, this);

    rendering_thread.join();
    calculation_thread.join();*/

    increment();
}

void Controller::increment() {

    while (frameNum < numFrames) {

        cout << "Calculating frame " << frameNum << "/" << numFrames << endl;
        frameNum++;

        std::thread solving_thread(&Solver::solve, solver, bodies, physics);

        solving_thread.join();
    }


}



void Controller::on_preparing_solver() {

    cout << "Preparing Solver" << endl;
}

void Controller::on_solving() {

    cout << "Solving forces" << endl;
}

void Controller::on_shifting_buffers() {

    cout << "Shifting buffers" << endl;
}

void Controller::on_solver_complete() {

    cout << "Finished calculating frame " << frameNum << "/" << numFrames << endl;

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


