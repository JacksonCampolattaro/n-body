//
// Created by jackcamp on 5/24/19.
//

#include <thread>
#include "Window.h"
#include "../Scenarios/Preset.h"
#include "../model/calculation/BarnesHut/BarnesHutSolver.h"
#include "../Controller.h"
#include "../Scenarios/ScenarioStream.h"

Window::Window() {

    add(mainBox);

    notebook.set_can_focus(false);
    notebook.append_page(physicsPage, "Physics");
    notebook.append_page(viewPage, "View");
    viewPage.pack_start(recorderCreator, Gtk::PACK_SHRINK);
    notebook.append_page(solverCreator, "Solver");
    mainBox.pack_start(notebook);


    run_button.set_halign(Gtk::ALIGN_END);
    run_button.override_background_color(Gdk::RGBA("green"));
    run_button.signal_clicked().connect(sigc::mem_fun(*this, &Window::on_run_clicked));
    actionBar.pack_end(run_button);
    actionBar.set_size_request(200, 40);
    mainBox.pack_start(actionBar, Gtk::PACK_SHRINK);

    show_all_children();


    // Loading a world from my collection of presets
    Preset myPreset = Preset();
    myPreset.blender();

    // The physics settings page should match the preset

    physicsPage.loadPhysicsContext(myPreset.getPhysicsContext());
    bodies = myPreset.getBodies();

    /*
    bodies = myPreset.getBodies();
    bodies.clear();

    std::vector<Body> bodyList;

    *//*
    for (int i = 0; i < bodies.size(); ++i) {
        bodyList.push_back(*bodies[i]);
    }

    ScenarioStream::saveBodies(bodyList, "/home/jackcamp/CLionProjects/n_body/src/n_body/Scenarios/test.xml");*//*

    bodyList = ScenarioStream::loadBodies("/home/jackcamp/CLionProjects/n_body/src/n_body/Scenarios/threeBody.xml");
    std::vector<Body *> newList;
    for (int j = 0; j < bodyList.size(); ++j) {
        newList.push_back(&bodyList[j]);
        std::cout << j << newList[j]->toString();
    }
    std::cout << newList.size();
    bodies = newList;
*/

    /*auto bodyList = ScenarioStream::loadBodies("/home/jackcamp/CLionProjects/n_body/src/n_body/Scenarios/threeBody.xml");
    for (int j = 0; j < bodyList.size(); ++j) {
        bodies.push_back(&bodyList[j]);
        std::cout << "Adding " << bodies[j]->toString() << endl;
    }*/


}

void Window::on_run_clicked() {

    // Creating the physics context
    auto physicsContext = physicsPage.createPhysicsContext();

    // Configuring the solving tool
    auto solver = solverCreator.createSolver();

    // Creating the model
    auto model = new Model(physicsContext, solver);

    // Creating the view
    auto view = viewPage.createView();

    // Creating the recorder
    auto recorder = recorderCreator.createRecorder(view);

    // Launching the program
    auto controller = Controller(model, view, bodies, recorder);

    controller.run();
}
