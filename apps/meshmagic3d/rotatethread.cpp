/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      RotateThread.cpp
**
** Created: Thu 21. May 08:54:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#include <QtGui>

#include "RotateThread.h"

RotateThread::RotateThread(QObject *parent) : QThread(parent)
{
    this->m_pAngle = 0;
    m_restart = false;
    m_abort = false;
}

RotateThread::~RotateThread()
{
    mutex.lock();
    m_abort = true;
    //condition.wakeOne();
    mutex.unlock();

    wait();
}

void RotateThread::Rotate(double angle, int axis)
{
    QMutexLocker locker(&mutex);

    this->m_angle = angle;
    this->m_axis = axis;

    if (!isRunning()) {
        start(LowPriority);
    } else {
        m_restart = true;
        //condition.wakeOne();
    }
}

void RotateThread::Rotate(std::vector<vtkSmartPointer<vtkActor> > lstActors, double angle, int axis)
{
    QMutexLocker locker(&mutex);

    this->m_angle = angle;
    this->m_axis = axis;
    this->lstActors = lstActors;
    if (!isRunning()) {
        start(LowPriority);
    } else {
        m_restart = true;
        //condition.wakeOne();
    }
}

void RotateThread::Rotate(vtkCamera *camera, double angle, int axis)
{
    QMutexLocker locker(&mutex);

    this->m_angle = angle;
    this->m_axis = axis;
    this->m_Camera = camera;
    if (!isRunning()) {
        start(LowPriority);
    } else {
        m_restart = true;
        //condition.wakeOne();
    }
}

void RotateThread::Rotate(std::vector<vtkSmartPointer<vtkActor> > lstActors, vtkCamera *camera, double angle, int axis)
{
    QMutexLocker locker(&mutex);

    this->m_angle = angle;
    this->m_axis = axis;
    this->lstActors = lstActors;
    this->m_Camera = camera;

    if (!isRunning()) {
        start(LowPriority);
    } else {
        m_restart = true;
        //condition.wakeOne();
    }
}

void RotateThread::abort()
{
    mutex.lock();
    if (isRunning()) {
        m_abort = true;
        //condition.wakeOne();
    }
    mutex.unlock();

    wait();
}

void RotateThread::run()
{
    //if ((m_restart) || (m_abort)) {
    //    return;
    //}

    //Method 1: Set Azimuth and Elevation for camera,
    //but these parameters need calculate (see vtkInteractionStyleTrackballCamera.h)
    mutex.lock();
    if (this->m_axis == 0) {
        //Rotate camera about x-axis
        this->m_Camera->Elevation(this->m_angle - this->m_pAngle);
    } else if (this->m_axis == 1) {
        //Rotate camera about y-axis
        this->m_Camera->Azimuth(this->m_angle - this->m_pAngle);
    } else if (this->m_axis == 2) {
        //Rotate camera about z-axis
        this->m_Camera->Roll(this->m_angle - this->m_pAngle);
    } else {
        //Rotate camera about xyz-axis
        this->m_Camera->Azimuth(this->m_angle - this->m_pAngle);
        this->m_Camera->Elevation(this->m_angle - this->m_pAngle);
        this->m_Camera->Roll(this->m_angle - this->m_pAngle);
    }
    this->m_Camera->OrthogonalizeViewUp();
    mutex.unlock();

    //Method 2: Create a transform matrix and add it to camera,
    //but center point of rotation is not coincide focal point of camera
//    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
//    transform->Identity();
//    transform->PreMultiply();
//    if (this->m_axis == 0) {
//        //this->m_Camera->SetViewUp(1,0,0);
//        transform->RotateX(this->m_angle - this->m_pAngle);
//    } else if (this->m_axis == 1) {
//        //this->m_Camera->SetViewUp(0,1,0);
//        transform->RotateY(this->m_angle - this->m_pAngle);
//    } else if (this->m_axis == 2) {
//        //this->m_Camera->SetViewUp(0,0,1);
//        transform->RotateZ(this->m_angle - this->m_pAngle);
//    } else {
//        //this->m_Camera->SetViewUp(1,1,1);
//        transform->RotateWXYZ(this->m_angle - this->m_pAngle,1,1,1);
//    }
//    transform->Modified();
//    mutex.lock();
//    this->m_Camera->ApplyTransform(transform);
//    this->m_Camera->Modified();
//    mutex.unlock();

    //Method 3: Create a matrix transform and add it to each actor
    //but center point of rotation is not coincide focal point of camera
//    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
//    transform->Identity();
//    transform->PreMultiply();
//    if (this->m_axis == 0) {
//        transform->RotateX(this->m_angle);
//    } else if (this->m_axis == 1) {
//        transform->RotateY(this->m_angle);
//    } else if (this->m_axis == 2) {
//        transform->RotateZ(this->m_angle);
//    } else {
//        transform->RotateWXYZ(this->m_angle,1,1,1);
//    }
//    transform->Modified();
//    mutex.lock();
//    for (int i = 0; i < this->lstActors.size(); i++) {
//        this->lstActors[i]->SetUserTransform(transform);
//    }
//    mutex.unlock();

    if (!m_restart) {
        emit emitSetCamera(this->m_Camera);
    }

    mutex.lock();
    //if (!m_restart) {
        //condition.wait(&mutex);
    //}
    this->m_pAngle = this->m_angle;
    m_restart = false;
    mutex.unlock();
}
