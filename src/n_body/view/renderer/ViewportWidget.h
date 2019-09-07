//
// Created by jackcamp on 8/29/19.
//

#ifndef N_BODY_VIEWPORTWIDGET_H
#define N_BODY_VIEWPORTWIDGET_H


#include <QtWidgets/QWidget>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/Qt3DCore>

class ViewportWidget : public QWidget {

public:

    ViewportWidget();


private:

    // View
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();

    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    // Camera
    Qt3DRender::QCamera *cameraEntity = view->camera();
};


#endif //N_BODY_VIEWPORTWIDGET_H
