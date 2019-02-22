//
// Created by jackcamp on 2/6/19.
//

#include <iostream>
#include "tracker.h"

tracker* tracker::internalInstance = nullptr;

tracker *tracker::instance() {

    if (!internalInstance) {
        internalInstance = new tracker();
        internalInstance->numFrames = 0;
    }
    return internalInstance;
}

void tracker::setSimulation(simulation* theSim) {
    this->theSim = theSim;
}

void tracker::recordFrame() {

    numFrames++;
    frameTime = clock() - timeStamp;
    timeStamp = clock();
}

void tracker::reportFrame() {

    std::cout << "Number of bodies: " << theSim->getNumBodies() << std::endl;
    std::cout << "Time to generate frame " << numFrames << ": " << frameTime << std::endl;
}
