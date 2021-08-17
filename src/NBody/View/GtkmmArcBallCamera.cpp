//
// Created by jackcamp on 8/17/21.
//

#include "GtkmmArcBallCamera.h"

bool NBody::GtkmmArcBallCamera::on_buttonPress(GdkEventButton *event) {
    std::cout << "test" << std::endl;
    return false;
}

bool NBody::GtkmmArcBallCamera::on_buttonRelease(GdkEventButton *event) {
    return false;
}

bool NBody::GtkmmArcBallCamera::on_mouseMotion(GdkEventMotion *event) {
    return false;
}

bool NBody::GtkmmArcBallCamera::on_scroll(GdkEventScroll *event) {
    return false;
}
