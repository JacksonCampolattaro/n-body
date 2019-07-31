//
// Created by jackcamp on 10/25/18.
//

#include <gtkmm/application.h>
#include <thread>
#include "controller/Application.h"
#include "Controller.h"
#include "model/calculation/BarnesHut/BarnesHutSolver.h"
#include "scenarios/ScenarioStream.h"

int main(int argc, char **argv) {

    /*auto application = new Application();

    return application->run(argc, argv);*/

    auto b = ScenarioStream::loadBodies("/home/jackcamp/CLionProjects/n_body/src/n_body/scenarios/threeBody.xml");

    auto p = new PhysicsContext();
    p->setPower(1)->setG(.2)->setT(0.0001);

    auto s = new BarnesHutSolver();

    auto v = new View(1000, 1000, "title");

    Controller c = Controller(&b, p, s, v);
    c.run();

}