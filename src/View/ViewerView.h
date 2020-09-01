//
// Created by jackcamp on 9/1/20.
//

#ifndef N_BODY_VIEWERVIEW_H
#define N_BODY_VIEWERVIEW_H

#include "View.h"


namespace Controller {
    class Application;
}

namespace View {

    class ViewerView : public View {
    public:
        ViewerView();

        void attach_application(Controller::Application *application) override;

    };

}


#endif //N_BODY_VIEWERVIEW_H
