//
// Created by jackcamp on 5/24/19.
//

#include <thread>
#include "Window.h"
#include "../Scenarios/Preset.h"
#include "../model/calculation/BarnesHut/BarnesHutSolver.h"
#include "../Controller.h"

Window::Window() {


    Preset myPreset = Preset();
    myPreset.blender();
    bodies = myPreset.getBodies();

    add(mainBox);

    notebook.set_can_focus(false);
    notebook.append_page(physicsPage, "Physics");
    notebook.append_page(viewPage, "View");
    viewPage.pack_start(recorderCreator, Gtk::PACK_SHRINK);
    notebook.append_page(solverCreator, "Solver");
    mainBox.pack_start(notebook);


    run_button.set_halign(Gtk::ALIGN_END);
    run_button.signal_clicked().connect(sigc::mem_fun(*this, &Window::on_run_clicked));
    actionBar.pack_end(run_button);
    actionBar.set_size_request(200, 40);
    mainBox.pack_start(actionBar, Gtk::PACK_SHRINK);

    show_all_children();
}

void Window::on_run_clicked() {

    // Creating the default physicsContext context
    auto physicsContext = new PhysicsContext();
    ///physicsContext->setT(0.01)->setG(0.02)->setPower(2);

    // Loading a world from my collection of presets
    Preset myPreset = Preset();
    myPreset.cubicGrid(glm::vec3(-45, -45, -100), glm::vec3(0, 0, -100), glm::vec3(10, 10, 10));
    bodies = myPreset.getBodies();
    physicsContext = myPreset.getPhysicsContext();

    physicsContext = physicsPage.createPhysicsContext();

    // Configuring the solving tool
    auto *solver = new BarnesHutSolver();
    solver->enableThreading();
    solver->setTheta(0.8);

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
