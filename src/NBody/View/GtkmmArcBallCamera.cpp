//
// Created by jackcamp on 8/17/21.
//

#include "GtkmmArcBallCamera.h"

bool NBody::GtkmmArcBallCamera::on_buttonPress(GdkEventButton *event) {
    initTransformation({static_cast<int>(event->x), static_cast<int>(event->y)});
    return false;
}

bool NBody::GtkmmArcBallCamera::on_buttonRelease(GdkEventButton *event) {
    return false;
}

bool NBody::GtkmmArcBallCamera::on_mouseMotion(GdkEventMotion *event) {

    if (event->state & GDK_BUTTON1_MASK)
        rotate({static_cast<int>(event->x), static_cast<int>(event->y)});

    if (event->state & GDK_BUTTON2_MASK)
        translate({static_cast<int>(event->x), static_cast<int>(event->y)});

    return false;
}

bool NBody::GtkmmArcBallCamera::on_scroll(GdkEventScroll *event) {
    zoom((float) -event->delta_y);
    return false;
}
