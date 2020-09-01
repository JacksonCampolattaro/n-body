//
// Created by jackcamp on 9/1/20.
//

#ifndef N_BODY_VIEW_H
#define N_BODY_VIEW_H

#include <spdlog/spdlog.h>

namespace Controller {
    class Application;
}

namespace View {

    class View {

    public:

        virtual void attach_application(Controller::Application *application) = 0;


    };

}


#endif //N_BODY_VIEW_H
