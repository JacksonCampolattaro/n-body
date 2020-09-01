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
        HeadlessView();

        void attach_application(Controller::Application *application) override;

    };

}

#endif //N_BODY_HEADLESSVIEW_H
