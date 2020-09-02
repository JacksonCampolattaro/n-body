//
// Created by jackcamp on 9/1/20.
//

#ifndef N_BODY_HEADLESSVIEW_H
#define N_BODY_HEADLESSVIEW_H

#include "View.h"


namespace Controller {
    class Application;
}

namespace View {

    class HeadlessView : public View {
    public:
        explicit HeadlessView(Controller::Application &application, std::shared_ptr<Model::Simulation> simulation);

    };

}

#endif //N_BODY_HEADLESSVIEW_H
