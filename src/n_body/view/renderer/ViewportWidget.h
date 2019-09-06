//
// Created by jackcamp on 8/29/19.
//

#ifndef N_BODY_VIEWPORTWIDGET_H
#define N_BODY_VIEWPORTWIDGET_H


#include <QtWidgets/QWidget>
#include <Qt3DExtras/Qt3DWindow>

class ViewportWidget : public QWidget {

public:

    ViewportWidget();


private:

    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
};


#endif //N_BODY_VIEWPORTWIDGET_H
