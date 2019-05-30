//
// Created by jackcamp on 5/26/19.
//

#ifndef N_BODY_POCMODEL_H
#define N_BODY_POCMODEL_H

#include <iostream>
#include <sigc++/signal.h>

using std::cout;
using std::endl;

class POCModel {

public:

    POCModel();

    void run();

    using type_signal_progress = sigc::signal<void(int)>;
    type_signal_progress signal_progress();

protected:

    type_signal_progress progressSignal;
};


#endif //N_BODY_POCMODEL_H
