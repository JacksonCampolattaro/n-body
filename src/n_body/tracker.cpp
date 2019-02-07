//
// Created by jackcamp on 2/6/19.
//

#include "tracker.h"

tracker* tracker::internalInstance = nullptr;

tracker *tracker::instance() {

    if (!internalInstance) {
        internalInstance = new tracker();
    }
    return internalInstance;
}

void tracker::recordFrame() {

}
