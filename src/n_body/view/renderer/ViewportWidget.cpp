//
// Created by jackcamp on 8/29/19.
//

#include "ViewportWidget.h"

ViewportWidget::ViewportWidget() {

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);


    // Adds the 3d view as a child of this widget
    view->defaultFrameGraph();
    createWindowContainer(view);
}
