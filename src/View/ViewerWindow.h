//
// Created by jackcamp on 4/17/20.
//

#ifndef N_BODY_VIEWERWINDOW_H
#define N_BODY_VIEWERWINDOW_H

#include "MagnumViewport.h"
#include "../Controller/Logger.h"

#include <gtkmm/window.h>

namespace View {

    class ViewerWindow : public Gtk::Window {

    public:

        ViewerWindow();
        ~ViewerWindow() override;

    protected:

        MagnumViewport _viewport;

    };

}


#endif //N_BODY_VIEWERWINDOW_H
