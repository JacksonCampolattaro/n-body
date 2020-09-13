//
// Created by jackcamp on 9/12/20.
//

#ifndef N_BODY_INTERACTIVE_H
#define N_BODY_INTERACTIVE_H

#include "View.h"
#include "Painter.h"

#include <gtkmm.h>

namespace View {

    class Interactive : public View {
    public:

        Interactive(Controller::Application &application);

        void on_drawables_changed(const std::deque<Model::Drawable::Drawable> &drawables) override;

        void on_update_clicked();
        void on_run_clicked();

    private:

        std::unique_ptr<Painter> _painter;

    };

}

#endif //N_BODY_INTERACTIVE_H
