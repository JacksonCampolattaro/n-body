//
// Created by jackcamp on 5/24/19.
//

#include <thread>
#include <fstream>
#include "Window.h"
#include "../scenarios/Preset.h"
#include "../model/calculation/BarnesHut/BarnesHutSolver.h"

Window::Window() {

    add(mainBox);

    notebook.set_can_focus(false);
    notebook.append_page(physicsPage, "Physics");
    notebook.append_page(viewPage, "View");
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
    auto myPreset = Preset();
    myPreset.blender();

    // Saving bodies to an XML file
    bodies = myPreset.getBodies();
    std::ofstream bodyOut("/home/jackcamp/CLionProjects/n_body/src/n_body/scenarios/blender/blender.bod");
    cereal::XMLOutputArchive bodyOutputArchive(bodyOut);
    bodyOutputArchive(bodies);

    // Saving physics to an XML file
    auto physics = myPreset.getPhysicsContext();
    physicsPage.loadPhysicsContext(&physics);
    std::ofstream physOut("/home/jackcamp/CLionProjects/n_body/src/n_body/scenarios/blender/blender.phys");
    cereal::XMLOutputArchive physOutputArchive(physOut);
    physOutputArchive(physics);

    // Loading bodies from an XML file
    /*std::ifstream in("/home/jackcamp/CLionProjects/n_body/src/n_body/scenarios/test.xml");
    cereal::XMLInputArchive inputArchive(in);
    inputArchive(bodies);*/


}

void Window::on_run_clicked() {

    // Creating the physics context
    auto physicsContext = physicsPage.createPhysicsContext();

    // Configuring the solving tool
    auto solver = solverCreator.createSolver();

    // Creating the view
    auto view = viewPage.createView();

    // Launching the program
    Controller controller = Controller(&bodies, physicsContext, solver, view);
    controller.run();
}
