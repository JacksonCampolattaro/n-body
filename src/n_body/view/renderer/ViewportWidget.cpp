//
// Created by jackcamp on 8/29/19.
//

#include <Qt3DExtras/QSphereMesh>
#include <Qt3DRender/QMaterial>
#include <Qt3DExtras/QPhongMaterial>

#include "ViewportWidget.h"

ViewportWidget::ViewportWidget() {

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);


    // Adds the 3d view as a child of this widget
    view->defaultFrameGraph();
    createWindowContainer(view);

    // Adds a root entity to the 3d view
    view->setRootEntity(rootEntity);

    // Adding a sphere for testing
    Qt3DCore::QEntity *sphereEntity = new Qt3DCore::QEntity(rootEntity);

    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh;
    sphereMesh->setRadius(3);
    sphereEntity->addComponent(sphereMesh);

    Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(rootEntity);
    sphereEntity->addComponent(material);

}
