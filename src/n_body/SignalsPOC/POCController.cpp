//
// Created by jackcamp on 5/26/19.
//

#include <thread>
#include "POCController.h"

POCController::POCController() {

    cout << "Creating a controller" << endl;

    model = new POCModel();
    cout << "   Instantiated the model" << endl;

    model->signal_progress().connect(sigc::mem_fun(this, &POCController::on_progress_announced));
    cout << "   Connected model->signal_progress to on_progress_announced()" << endl;

    cout << endl;


}

void POCController::modelStarter() {

    cout << "Using the controller to spawn and run a model" << endl;

    std::thread t(&POCModel::run, model);
    cout << "   Started the model in it's own thread" << endl;

    t.detach();
    cout << "   Detached the model's thread" << endl;


    cout << "   Model launched" << endl;
    cout << endl;

}

void POCController::mainLoop() {

    cout << "Running the controller's main loop" << endl;
    cout << "   In an actual program, this would involve things like checking for input" << endl;
    cout << "   In this case, it outputs a '.' once every second" << endl;

    for (int i = 0; i < 60; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        cout << "." << endl;
    }
}

void POCController::on_progress_announced(int progress) {
    cout << progress << endl;
}
