//
// Created by jackcamp on 9/12/20.
//

#ifndef N_BODY_INTERACTIVE_H
#define N_BODY_INTERACTIVE_H

#include "View.h"
#include <gtkmm.h>

namespace View {

    class Interactive : public View {
    public:

        Interactive(Controller::Application &application);

        void on_start_simulation() override;

        void on_simulation_progress(float progress, const std::string &status) override;

        void on_drawables_changed(const std::deque<Model::Drawable::Drawable> &drawables) override;

        void on_simulation_complete() override;

    private:

        std::shared_ptr<Gtk::Builder> _builder;
    };

}

#endif //N_BODY_INTERACTIVE_H
