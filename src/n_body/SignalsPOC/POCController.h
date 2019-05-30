//
// Created by jackcamp on 5/26/19.
//

#ifndef N_BODY_POCCONTROLLER_H
#define N_BODY_POCCONTROLLER_H

#include "POCModel.h"

#include <iostream>
#include <sigc++/signal.h>

using std::cout;
using std::endl;

class POCController : public sigc::trackable {

public:

    POCController();

    void modelStarter();

    void mainLoop();

    void on_progress_announced(int progress);

private:

    POCModel *model;
};


#endif //N_BODY_POCCONTROLLER_H
