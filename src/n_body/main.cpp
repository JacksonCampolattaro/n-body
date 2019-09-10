//
// Created by jackcamp on 10/25/18.
//

#include <thread>
#include "Controller.h"
#include "model/calculation/BarnesHut/BarnesHutSolver.h"
#include "scenarios/ScenarioStream.h"
#include "scenarios/Preset.h"


#include <n_body/view/ViewportWidget.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <n_body/view/Launcher.h>

int main(int argc, char **argv) {

    /*auto preset = Preset();
    preset.blender();
    vector<Body> b;
    auto presetBodies = preset.getBodies();
    for (int i = 0; i < presetBodies.size(); ++i) {
        b.push_back(*presetBodies[i]);
    }
    ScenarioStream::saveBodies(b, "/home/jackcamp/CLionProjects/n_body/src/n_body/scenarios/test.xml");*/
    /*auto b = ScenarioStream::loadBodies("/home/jackcamp/CLionProjects/n_body/src/n_body/scenarios/test.xml");

    auto p = new PhysicsContext();
    p->setPower(2)->setG(.02)->setT(0.001);

    auto s = new BarnesHutSolver();
    s->setTheta(0.9);

    Controller c = Controller(&b, p, s);
    c.run();*/

    QApplication app(argc, argv);

    /*auto b = new Body(Position(5, 50, -200));

    auto viewPort = new ViewportWidget;
    viewPort->setMinimumSize(QSize(1920, 1080));
    viewPort->setTitle("Test");
    viewPort->addEntity(b->getSphereEntity());

    viewPort->show();
    viewPort->resize(480, 360);
    b->setColor(rgbaColor(0, 0.5, 0, 1));
    b->setVelocity(Velocity(1, -2, 0));
    b->drift(10);
    b->shiftBuffers();*/

    auto launcher = new Launcher();
    launcher->show();
    return app.exec();
}