//
// Created by jackcamp on 5/22/19.
//

#include "Launcher.h"
#include "Preset.h"
#include "../model/calculation/BarnesHut/BarnesHutSolver.h"
#include "../view/Recorder.h"
#include "../Controller.h"

Launcher::Launcher() {

    // Setting the title
    set_title("n_body Launcher");

    // Window has a 10 pixel border
    set_border_width(10);

    // Window starts with a default size
    ///set_default_size(600, 300);


    // Configuraing the main box that partitions the interface
    mainBox.set_orientation(Gtk::ORIENTATION_VERTICAL);
    add(mainBox);

    // Creating the run_button, linking it to the on_start function, and adding it to the window
    start_button = Gtk::Button("Start");
    start_button.signal_clicked().connect(sigc::mem_fun(*this, &Launcher::on_start));

    // Adding items to the main box
    mainBox.pack_start(physicsSettings, Gtk::PACK_SHRINK);
    mainBox.pack_start(viewSettings, Gtk::PACK_SHRINK);
    mainBox.pack_start(recorderSettings, Gtk::PACK_SHRINK);
    mainBox.pack_start(start_button);

    // Showing everything
    show_all_children();
}

Launcher::~Launcher() {

}

void Launcher::on_start() {

    // Creating the default physics context
    physicsContext = new PhysicsContext();
    physicsContext->setT(0.01)->setG(0.02)->setPower(2);

    // Loading a world from my collection of presets
    Preset myPreset = Preset();
    myPreset.blender();
    physicsContext = myPreset.getPhysicsContext();
    bodies = myPreset.getBodies();

    // Configuring the solving tool
    auto *solver = new BarnesHutSolver();
    solver->enableThreading();
    solver->setTheta(0.8);

    // Creating the model
    model = new Model(physicsContext, solver);

    // Creating the view
    view = viewSettings.getView();


    // Creating the recorder
    //auto recorder = new Recorder(view, "/home/jackcamp/CLionProjects/n_body/src/n_body/staging/output.mp4");
    auto recorder = recorderSettings.getRecorder(view);

    // Launching the program
    auto controller = Controller(model, view, bodies, recorder);
    controller.run();

    return;


}
