//
// Created by jackcamp on 2/6/19.
//

#include "Tracker.h"

#include <iostream> // Used for printing outputs to the terminal
#include <iomanip> // Used to set precision of results

Tracker *Tracker::internalInstance = nullptr;

Tracker *Tracker::instance() {

    if (!internalInstance) {
        internalInstance = new Tracker();

        internalInstance->numFrames = 0;
        internalInstance->programStartTime = clock();
    }
    return internalInstance;
}

void Tracker::markFrameCompleted() {

    b = clock();

    // The timestamp was set at the end of the last frame,
    // rendering time accounts for the gap between calculation times
    renderingTime = clock() - timeStamp;

    numFrames++;

    // Getting time to calculate frame
    totalFrameTime = clock() - frameStartTime;

    // Resetting values for next frame
    frameStartTime = clock();
    timeStamp = frameStartTime;
}

void Tracker::markFrameStarted() {

    a = clock();

    calculationStartTime = clock() - timeStamp;
    timeStamp = clock();
}

void Tracker::markTreeCreated() {

    treeCreationTime = clock() - timeStamp;
    timeStamp = clock();
}

void Tracker::markTreeCompleted() {

    treePopulationTime = clock() - timeStamp;
    timeStamp = clock();
}

void Tracker::markCenterMassCalculated() {
    centerMassCalculationTime = clock() - timeStamp;
    timeStamp = clock();
}

void Tracker::markGravityCalculated() {
    gravityCalculationTime = clock() - timeStamp;
    timeStamp = clock();
}

void Tracker::markPositionsUpdated() {
    positionUpdateTime = clock() - timeStamp;
    timeStamp = clock();
}


void Tracker::outputStatus() {

    std::cout << "Frame " << numFrames << ": " << std::endl;
    std::cout << "      Total calculation time: " << inSeconds(totalFrameTime) << std::endl;
    std::cout << "      Framerate (un-smoothed):             " << 1 / inSeconds(b-a) << " FPS" << std::endl;
    std::cout << "      Framerate (average):                 " << 1 / inSeconds((clock() - programStartTime) / numFrames) << " FPS" << std::endl;
    /*std::cout << "      Total calculation time:              " << totalFrameTime << " clock cycles --> "
         << inSeconds(totalFrameTime) << " s" << std::endl;
    std::cout << std::setprecision(3);
    std::cout << "           Tree Creation:                    " << 100.0 * ((double) treeCreationTime / (double) totalFrameTime) << "%         " << inSeconds(treeCreationTime) << " s" << std::endl;
    std::cout << "           Tree Population:                  " << 100.0 * ((double) treePopulationTime / (double) totalFrameTime) << "%         " << inSeconds(treePopulationTime) << " s" << std::endl;
    std::cout << "           Center of Mass Propegation:       " << 100.0 * ((double) centerMassCalculationTime / (double) totalFrameTime) << "%         " << inSeconds(centerMassCalculationTime) << " s" << std::endl;
    std::cout << "           Gravitational Calculations:       " << 100.0 * ((double) gravityCalculationTime / (double) totalFrameTime) << "%         " << inSeconds(gravityCalculationTime) << " s" << std::endl;
    std::cout << "           Updating Positions:               " << 100.0 * ((double) positionUpdateTime / (double) totalFrameTime) << "%      " << inSeconds(positionUpdateTime) << " s" << std::endl;
    std::cout << "           Rendering:                        " << 100.0 * ((double) renderingTime / (double) totalFrameTime) << "%       " << inSeconds(renderingTime) << " s" << std::endl;
    std::cout << "      Framerate (un-smoothed):             " << 1 / inSeconds(totalFrameTime) << " FPS" << std::endl;
    std::cout << "      Framerate (average):                 " << 1 / inSeconds((clock() - programStartTime) / numFrames) << " FPS" << std::endl;
    std::cout << std::endl;*/
}

