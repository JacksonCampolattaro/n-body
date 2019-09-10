//
// Created by jackcamp on 9/9/19.
//

#include <n_body/model/Body.h>
#include <n_body/scenarios/Preset.h>
#include <n_body/model/calculation/BarnesHut/BarnesHutSolver.h>
#include <n_body/Controller.h>
#include "Launcher.h"
#include "ViewportWidget.h"


Launcher::Launcher() {

    auto vbox = new QVBoxLayout(this);

    viewport = new ViewportWidget();
    QWidget *container = QWidget::createWindowContainer(viewport);
    vbox->addWidget(container);

    auto hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    auto startButton = new QPushButton("Start");
    hbox->addWidget(startButton);
    connect(startButton, &QPushButton::clicked, this, &Launcher::start);

    auto nextButton = new QPushButton("Next", this);
    hbox->addWidget(nextButton);
    connect(nextButton, &QPushButton::clicked, this, &Launcher::next);

    viewport->show();
    resize(2000, 1000);

}

void Launcher::start() {

    auto preset = Preset();
    preset.threeBodyDemo();
    vector<Body> b;
    auto presetBodies = preset.getBodies();
    for (int i = 0; i < presetBodies.size(); ++i) {
        b.push_back(*presetBodies[i]);
        viewport->addEntity(b[i].getSphereEntity());
    }
    auto p = new PhysicsContext();
    p->setPower(2)->setG(.02)->setT(0.1);

    auto s = new BarnesHutSolver();
    s->setTheta(0.9);

    solver = s;
    physics = p;
    bodies = &b;

    controller = new Controller(&b, p, s);
}

void Launcher::next() {

    /*for (int i = 0; i < 1; ++i) {

        solver->solve(bodies, physics);
    }*/
    (*bodies)[0].drift(1);
    (*bodies)[0].shiftBuffers();
}
