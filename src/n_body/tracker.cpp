//
// Created by jackcamp on 2/6/19.
//

#include <iostream>

#include "tracker.h"

tracker *tracker::internalInstance = nullptr;

tracker *tracker::instance() {

    if (!internalInstance) {
        internalInstance = new tracker();

        internalInstance->numFrames = 0;
        internalInstance->programStartTime = clock();
    }
    return internalInstance;
}

void tracker::setSimulation(simulation *theSim) {
    this->theSim = theSim;
}

void tracker::markFrameCompleted() {

    renderingTime = clock() - timeStamp;

    numFrames++;

    // Getting time to calculate frame
    totalFrameTime = clock() - frameStartTime;

    // Resetting values for next frame
    frameStartTime = clock();
    timeStamp = frameStartTime;
}

void tracker::markTreeCompleted() {

    treePopulationTime = clock() - timeStamp;
    timeStamp = clock();
}

void tracker::markCenterMassCalculated() {
    centerMassCalculationTime = clock() - timeStamp;
    timeStamp = clock();
}

void tracker::markGravityCalculated() {
    gravityCalculationTime = clock() - timeStamp;
    timeStamp = clock();
}

void tracker::markVelocityApplied() {
    velocityApplicationTime = clock() - timeStamp;
    timeStamp = clock();
}

void tracker::markPositionsUpdated() {
    positionUpdateTime = clock() - timeStamp;
    timeStamp = clock();
}


void tracker::outputStatus() {

    cout << "Frame " << numFrames << ": " << endl;
    cout << "      Number of bodies:                    " << theSim->getNumBodies() << endl;
    cout << "      Total calculation time:              " << totalFrameTime << " clock cycles --> "
         << inSeconds(totalFrameTime) << " s" << endl;
    cout << setprecision(3);
    cout << "           Tree Population:                " << 100.0 * ((double) treePopulationTime / (double) totalFrameTime) << "%         " << inSeconds(treePopulationTime) << " s" << endl;
    cout << "           Center of Mass Propegation:     " << 100.0 * ((double) centerMassCalculationTime / (double) totalFrameTime) << "%         " << inSeconds(centerMassCalculationTime) << " s" << endl;
    cout << "           Gravitational Calculations:     " << 100.0 * ((double) gravityCalculationTime / (double) totalFrameTime) << "%         " << inSeconds(gravityCalculationTime) << " s" << endl;
    cout << "           Application of Velocity:        " << 100.0 * ((double) velocityApplicationTime / (double) totalFrameTime) << "%       " << inSeconds(velocityApplicationTime) << " s" << endl;
    cout << "           Updating Positions:             " << 100.0 * ((double) positionUpdateTime / (double) totalFrameTime) << "%      " << inSeconds(positionUpdateTime) << " s" << endl;
    cout << "           Rendering:                      " << 100.0 * ((double) renderingTime / (double) totalFrameTime) << "%       " << inSeconds(renderingTime) << " s" << endl;
    cout << "      Framerate (un-smoothed):             " << 1 / inSeconds(totalFrameTime) << " FPS" << endl;
    cout << "      Framerate (average):                 " << 1 / inSeconds((clock() - programStartTime) / numFrames) << " FPS" << endl;
    cout << endl;
}

