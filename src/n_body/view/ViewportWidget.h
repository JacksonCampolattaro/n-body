//
// Created by jackcamp on 8/29/19.
//

#ifndef N_BODY_VIEWPORTWIDGET_H
#define N_BODY_VIEWPORTWIDGET_H


#include <QtWidgets/QWidget>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DCore/Qt3DCore>

class ViewportWidget : public Qt3DExtras::Qt3DWindow {

public:

    ViewportWidget();

    void addEntity(Qt3DCore::QEntity *entity);

private:

    Qt3DCore::QEntity *rootEntity;
};


#endif //N_BODY_VIEWPORTWIDGET_H
