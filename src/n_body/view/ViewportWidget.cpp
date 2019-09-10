//
// Created by jackcamp on 8/29/19.
//

#include <Qt3DExtras/QSphereMesh>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QMaterial>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DInput/QInputAspect>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DRender/QPointLight>

#include "ViewportWidget.h"


ViewportWidget::ViewportWidget() {


    // Background color
    defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));

    // Root entity
    rootEntity = new Qt3DCore::QEntity();
    setRootEntity(rootEntity);

    // Camera
    Qt3DRender::QCamera *cameraEntity = camera();

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    // Input for camera controls
    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(cameraEntity);

    // Light (Emitted from the camera's position)
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    /*

    // Sphere
    *//*Sphere shape data*//*
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRings(20);
    sphereMesh->setSlices(20);
    sphereMesh->setRadius(2);
    *//*Sphere mesh transform*//*
    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform();
    sphereTransform->setScale(1.3f);
    sphereTransform->setTranslation(QVector3D(0.0f, 0.0f, -10.0f));
    *//*Sphere material*//*
    Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial();
    QColor c;
    c.setRgbF(0.7, 0.6, 0.1     );
    sphereMaterial->setDiffuse(c);
    sphereMaterial->setAmbient(c);
    sphereMaterial->setSpecular(c);
    sphereMaterial->setShininess(0);
    *//*Assembling the sphere*//*
     auto sphereEntity = new Qt3DCore::QEntity(rootEntity);
    sphereEntity->addComponent(sphereMesh);
    sphereEntity->addComponent(sphereMaterial);
    sphereEntity->addComponent(sphereTransform);

    // Second Sphere
    *//*Sphere 2 mesh transform*//*
    Qt3DCore::QTransform *sphere2Transform = new Qt3DCore::QTransform();
    sphere2Transform->setScale(1.3f);
    sphere2Transform->setTranslation(QVector3D(1.0f, 0.0f, 0.0f));
    *//*Assembling the sphere*//*
    auto sphere2Entity = new Qt3DCore::QEntity(rootEntity);
    sphere2Entity->addComponent(sphereMesh);
    sphere2Entity->addComponent(sphereMaterial);
    sphere2Entity->addComponent(sphere2Transform);*/
}

void ViewportWidget::addEntity(Qt3DCore::QEntity *entity) {

    entity->setParent(rootEntity);
}
