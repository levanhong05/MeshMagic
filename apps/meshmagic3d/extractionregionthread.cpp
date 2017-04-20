/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      ExtractionRegionThread.cpp
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

#include "extractionregionthread.h"

#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataConnectivityFilter.h>

ExtractionRegionThread::ExtractionRegionThread(QObject *parent) :
    QThread(parent)
{
    m_working = false;
    m_abort = false;
    m_OutputActor = vtkActor::New();
    //m_ActorCollection = vtkActorCollection::New();
}

ExtractionRegionThread::~ExtractionRegionThread()
{
    mutex.lock();
    m_abort = true;
    m_OutputActor->Delete();
    //m_ActorCollection->Delete();
    mutex.unlock();

    wait();
}

void ExtractionRegionThread::ExtractRegion(vtkActor *actor)
{
    QMutexLocker locker(&mutex);
    this->m_InputActor = actor;

    if (!isRunning()) {
        start(LowPriority);
    } else {
        m_working = true;
    }
}

void ExtractionRegionThread::abort()
{
    mutex.lock();
    if (isRunning()) {
        m_abort = true;
        m_OutputActor->Delete();
        //m_ActorCollection->Delete();
    }
    mutex.unlock();

    wait();
}

void ExtractionRegionThread::run()
{
    mutex.lock();
    vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
    connectivityFilter->SetInputConnection(this->m_InputActor->GetMapper()->GetInputConnection(0,0));
    connectivityFilter->SetExtractionModeToAllRegions();
    connectivityFilter->ColorRegionsOn();
    connectivityFilter->Update();

    unsigned int numRegions;
    numRegions = connectivityFilter->GetNumberOfExtractedRegions();
    //if (numRegions == 1) {
    // create a mapper and actor for extracted data
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(connectivityFilter->GetOutputPort());
    mapper->SetScalarRange(connectivityFilter->GetOutput()->GetPointData()->GetArray("RegionId")->GetRange());
    mapper->Update();

    this->m_OutputActor->SetMapper(mapper);

    if (!m_working) {
        emit emitExtractDone(this->m_OutputActor, numRegions);
    }
    /*} else {
        this->m_ActorCollection->RemoveAllItems();
        for (unsigned int i = 0; i < numRegions; i++) {
            vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter2 = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
            connectivityFilter2->SetInputConnection(this->m_InputActor->GetMapper()->GetInputConnection(0,0));
            connectivityFilter2->SetExtractionModeToSpecifiedRegions();
            connectivityFilter2->AddSpecifiedRegion(i); //select the region to extract here
            connectivityFilter2->ColorRegionsOn();
            connectivityFilter2->Update();

            // create a mapper and actor for extracted data
            vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            mapper->SetInputConnection(connectivityFilter2->GetOutputPort());
            mapper->SetScalarRange(connectivityFilter2->GetOutput()->GetPointData()->GetArray("RegionId")->GetRange());
            mapper->Update();

            vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
            actor->SetMapper(mapper);
            this->m_ActorCollection->AddItem(actor);
        }

        if (!m_working) {
            emit emitExtractDone(this->m_ActorCollection, numRegions);
        }
    }*/
    mutex.unlock();

    mutex.lock();
    m_working = false;
    mutex.unlock();
}
