//
// Created by jackcamp on 9/1/20.
//

#ifndef N_BODY_INTERACTIVEVIEW_H
#define N_BODY_INTERACTIVEVIEW_H

#include "View.h"

#include "ViewerWindow.h"

namespace View {

    class InteractiveView : public View {
    public:
        InteractiveView();

        void attach_application(Controller::Application *application) override;
    };

}


#endif //N_BODY_INTERACTIVEVIEW_H
