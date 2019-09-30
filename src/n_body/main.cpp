//
// Created by jackcamp on 10/25/18.
//

#include <gtkmm/application.h>
#include <thread>
#include "controller/Application.h"
#include "model/calculation/BarnesHut/BarnesHutSolver.h"
#include "scenarios/Preset.h"

int main(int argc, char **argv) {

    auto application = new Application();

    return application->run(argc, argv);

    /*//auto b = ScenarioStream::loadBodies("/home/jackcamp/CLionProjects/n_body/src/n_body/scenarios/blender.xml");
    auto preset = Preset();
    preset.blender();
    vector<Body> b;
    auto presetBodies = preset.getBodies();
    for (int i = 0; i < presetBodies.size(); ++i) {
        b.push_back(*presetBodies[i]);
    }
    ScenarioStream::saveBodies(b, "/home/jackcamp/CLionProjects/n_body/src/n_body/scenarios/test.xml");



    auto p = new PhysicsContext();
    p->setPower(2)->setG(.02)->setT(0.001);

    auto s = new BarnesHutSolver();
    s->setTheta(0.9);

    auto v = new View(4000, 2000, "title");

    Controller c = Controller(&b, p, s, v);
    c.run();*/

}