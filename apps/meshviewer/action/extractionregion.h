/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      ExtractionRegionThread.h
**
** Created: Thu 28. May 08:44:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** This module will extract model to all region
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef EXTRACTIONREGIONTHREAD_H
#define EXTRACTIONREGIONTHREAD_H

#include <QMutex>
#include <QThread>

#include <vtkActor.h>
#include <vtkSmartPointer.h>
//#include <vtkActorCollection.h>

#include <vector>

class ExtractionRegion : public QThread
{
    Q_OBJECT
public:
    ExtractionRegion(QObject *parent = 0);
    ~ExtractionRegion();

    void ExtractRegion(vtkActor *actor);
    void abort();

signals:
    void emitExtractDone(vtkActor *actor, unsigned int numRegions);
    //void emitExtractDone(vtkActorCollection *actorCollection, unsigned int numRegions);

protected:
    void run();

private:
    QMutex mutex;
    bool m_abort;
    bool m_working;

    vtkActor *m_InputActor;
    vtkActor *m_OutputActor;

};

#endif // EXTRACTIONREGIONTHREAD_H
