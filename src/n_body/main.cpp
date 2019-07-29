//
// Created by jackcamp on 10/25/18.
//

#include <gtkmm/application.h>
#include "controller/Application.h"
#include "Controller.h"
#include "model/calculation/BarnesHut/BarnesHutSolver.h"
#include "Scenarios/ScenarioStream.h"

int main(int argc, char **argv) {

    /*auto application = new Application();

    return application->run(argc, argv);*/

    auto p = new PhysicsContext();

    Solver *s = new BarnesHutSolver();

    /*auto view = new View();
    view->setDimensions(glm::ivec2(1000, 1000))->setTitle("Test");*/

    ///std::vector<Body> b;
    auto b = ScenarioStream::loadBodies("/home/jackcamp/CLionProjects/n_body/src/n_body/Scenarios/test.xml");

    Controller c = Controller(&b, p, s);
    c.run();

}