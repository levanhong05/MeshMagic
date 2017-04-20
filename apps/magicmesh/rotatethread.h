/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      RotateThread.h
**
** Created: Thu 21. May 08:54:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** This module will rotate camera when main thread request
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef ROTATETHREAD_H
#define ROTATETHREAD_H

#include <QMutex>
#include <QThread>
//#include <QWaitCondition>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>

#include <vector>

class RotateThread : public QThread
{
    Q_OBJECT
public:
    RotateThread(QObject *parent = 0);
    ~RotateThread();

    void Rotate(double angle, int axis = 3);
    void Rotate(vtkCamera *camera, double angle, int axis = 3);
    void Rotate(std::vector<vtkSmartPointer<vtkActor> > lstActors, double angle, int axis = 3);
    void Rotate(std::vector<vtkSmartPointer<vtkActor> > lstActors, vtkCamera *camera, double angle, int axis = 3);
    void abort();

signals:
    void emitSetCamera(vtkCamera *cam);

protected:
    void run();

private:
    QMutex mutex;
    vtkCamera *m_Camera;
    std::vector<vtkSmartPointer<vtkActor> > lstActors;
    //Specify axis is rotate
    int m_axis;
    //Specify angle is rotate
    double m_angle;
    //Specify previous angle is rotate, default by 0
    double m_pAngle;
    bool m_abort;
    bool m_restart;
};

#endif // ROTATETHREAD_H
