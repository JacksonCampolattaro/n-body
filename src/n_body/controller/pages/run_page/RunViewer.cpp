//
// Created by jackcamp on 6/6/19.
//

#include <iostream>
#include "RunViewer.h"

RunViewer::RunViewer() {

    set_orientation(Gtk::ORIENTATION_VERTICAL);

    set_margin_top(20);
    set_margin_bottom(20);
    set_margin_left(30);
    set_margin_right(30);
    set_spacing(10);
    
    pack_start(frameCount_label);
    
    show_all_children();
}

void RunViewer::on_completed_frame(int frameNumber) {

    frameCount_label.set_text(std::to_string(frameNumber));
}
