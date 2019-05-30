//
// Created by jackcamp on 5/26/19.
//

#include <chrono>
#include <thread>
#include "POCModel.h"

POCModel::POCModel() {

}

void POCModel::run() {

    cout << "           ***Model Started" << endl;

    cout << "           ***Delaying 5s before further action..." << endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    cout << "           ***...5s Over" << endl;

    cout << "       ***The model will now report it's progress every 2s, using the progress signal" << endl;
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cout << "       ***Sending signal" << endl;
        progressSignal.emit(i);
    }
    cout << "       ***Model is finished running" << endl;
}

POCModel::type_signal_progress POCModel::signal_progress() {
    return progressSignal;
}
