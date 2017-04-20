/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      AboutMeshMagic.h
**
** Created: Wed 06. May 08:16:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#include "STLFileCore.h"

//Reader file
#include <vtkSTLReader.h>
#include <vtkPLYReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkGenericDataObjectReader.h>

//Writer file
#include <vtkSTLWriter.h>
#include <vtkPLYWriter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkGenericDataObjectWriter.h>

#include <vtkTransform.h>
#include <vtkCleanPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkTriangleFilter.h>
#include <vtkTextureMapToPlane.h>
#include <vtkTransformTextureCoords.h>
#include <vtkTransformPolyDataFilter.h>

STLFileCore::STLFileCore(QObject *parent) : QThread(parent)
{
    m_working = false;
    m_abort = false;
    m_Actor = vtkActor::New();
}

STLFileCore::~STLFileCore()
{
    mutex.lock();
    m_abort = true;
    m_Actor->Delete();
    mutex.unlock();

    wait();
}

void STLFileCore::STLReading(QString fileName, int fileType, int typeAction)
{
    QMutexLocker locker(&mutex);
    this->m_FileName = fileName;
    this->m_Type = typeAction;
    this->m_FileSupport = fileType;

    if (!isRunning()) {
        start(LowPriority);
    } else {
        m_working = true;
    }
}

void STLFileCore::STLSaving(std::vector<vtkSmartPointer<vtkActor> > lstActor, QString fileName,
                            int fileType, int encode, int typeAction)
{
    QMutexLocker locker(&mutex);
    this->m_FileName = fileName;
    this->m_Type = typeAction;
    this->m_FileSupport = fileType;
    this->m_EncodeSupport = encode;
    this->lstActors = lstActor;

    if (!isRunning()) {
        start(LowPriority);
    } else {
        m_working = true;
    }
}

void STLFileCore::abort()
{
    mutex.lock();
    if (isRunning()) {
        m_abort = true;
        m_Actor->Delete();
    }
    mutex.unlock();

    wait();
}

void STLFileCore::ReadSTLFile(QString fileName)
{
    vtkSmartPointer<vtkTextureMapToPlane> textureMapper = vtkSmartPointer<vtkTextureMapToPlane>::New();

    if (m_FileSupport == 0) { //STL File
        vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
        reader->SetFileName(fileName.toStdString().c_str());
        reader->Update();

        textureMapper->SetInputConnection(reader->GetOutputPort());
    } else if (m_FileSupport == 1) { //PLY File
        vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();
        reader->SetFileName(fileName.toStdString().c_str());
        reader->Update();

        textureMapper->SetInputConnection(reader->GetOutputPort());
    } else if (m_FileSupport == 2) { //VTK File
        vtkSmartPointer<vtkGenericDataObjectReader> reader = vtkSmartPointer<vtkGenericDataObjectReader>::New();
        reader->SetFileName(fileName.toStdString().c_str());
        reader->Update();

        textureMapper->SetInputConnection(reader->GetOutputPort());
    } else if (m_FileSupport == 3) { //VTP File
        vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
        reader->SetFileName(fileName.toStdString().c_str());
        reader->Update();

        textureMapper->SetInputConnection(reader->GetOutputPort());
    }

    // We scale the texture coordinate to get some repeat patterns.
    vtkSmartPointer<vtkTransformTextureCoords> textureTransform = vtkSmartPointer<vtkTransformTextureCoords>::New();
    textureTransform->SetInputConnection(textureMapper->GetOutputPort());

    // Visualize
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(textureTransform->GetOutputPort());

    this->m_Actor->SetMapper(mapper);
    //this->m_Actor->GetProperty()->SetColor(1.0, 0.8941, 0.7686); // bisque
}

void STLFileCore::SaveSTLFile(QString fileName)
{
    if (m_FileSupport == 0) { //STL File
        vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();
        writer->SetFileName(fileName.toStdString().c_str());

        vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();

        for (unsigned int i = 0; i < this->lstActors.size(); i++) {
            vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
            input->ShallowCopy(vtkPolyData::SafeDownCast(this->lstActors[i]->GetMapper()->GetInput()));

            vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
            translation->Identity();
            translation->Translate(this->lstActors[i]->GetCenter()[0], this->lstActors[i]->GetCenter()[1], this->lstActors[i]->GetCenter()[2]);

            vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            transformFilter->SetInputConnection(input->GetProducerPort());
            transformFilter->SetTransform(translation);
            transformFilter->Update();
            #if VTK_MAJOR_VERSION <= 5
                appendFilter->AddInputConnection(transformFilter->GetOutputPort());
            #else
                appendFilter->AddInputData(transformFilter->GetOutput());
            #endif
            appendFilter->Update();
        }

        // Remove any duplicate points.
        vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
        cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
        cleanFilter->Update();

        vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
        triangleFilter->SetInputConnection(cleanFilter->GetOutputPort());
        triangleFilter->Update();

        writer->SetInputConnection(triangleFilter->GetOutputPort());

        if (this->m_EncodeSupport == 0) {
            writer->SetFileTypeToASCII();
        } else if (this->m_EncodeSupport == 1) {
            writer->SetFileTypeToBinary();
        }
        writer->Write();
    } else if (m_FileSupport == 1) { //PLY File
        vtkSmartPointer<vtkPLYWriter> writer = vtkSmartPointer<vtkPLYWriter>::New();
        writer->SetFileName(fileName.toStdString().c_str());

        vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();

        for (unsigned int i = 0; i < this->lstActors.size(); i++) {
            vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
            input->ShallowCopy(vtkPolyData::SafeDownCast(this->lstActors[i]->GetMapper()->GetInput()));

            vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
            translation->Identity();
            translation->Translate(this->lstActors[i]->GetCenter()[0], this->lstActors[i]->GetCenter()[1], this->lstActors[i]->GetCenter()[2]);

            vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            transformFilter->SetInputConnection(input->GetProducerPort());
            transformFilter->SetTransform(translation);
            transformFilter->Update();
            #if VTK_MAJOR_VERSION <= 5
                appendFilter->AddInputConnection(transformFilter->GetOutputPort());
            #else
                appendFilter->AddInputData(transformFilter->GetOutput());
            #endif
            appendFilter->Update();
        }

        // Remove any duplicate points.
        vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
        cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
        cleanFilter->Update();

        vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
        triangleFilter->SetInputConnection(cleanFilter->GetOutputPort());
        triangleFilter->Update();

        writer->SetInputConnection(triangleFilter->GetOutputPort());

        if (this->m_EncodeSupport == 0) {
            writer->SetFileTypeToASCII();
        } else if (this->m_EncodeSupport == 1) {
            writer->SetFileTypeToBinary();
        }
        writer->Write();
    } else if (m_FileSupport == 2) { //VTK File
        vtkSmartPointer<vtkGenericDataObjectWriter> writer = vtkSmartPointer<vtkGenericDataObjectWriter>::New();
        writer->SetFileName(fileName.toStdString().c_str());

        vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();

        for (unsigned int i = 0; i < this->lstActors.size(); i++) {
            vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
            input->ShallowCopy(vtkPolyData::SafeDownCast(this->lstActors[i]->GetMapper()->GetInput()));

            vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
            translation->Identity();
            translation->Translate(this->lstActors[i]->GetCenter()[0], this->lstActors[i]->GetCenter()[1], this->lstActors[i]->GetCenter()[2]);

            vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            transformFilter->SetInputConnection(input->GetProducerPort());
            transformFilter->SetTransform(translation);
            transformFilter->Update();
            #if VTK_MAJOR_VERSION <= 5
                appendFilter->AddInputConnection(transformFilter->GetOutputPort());
            #else
                appendFilter->AddInputData(transformFilter->GetOutput());
            #endif
            appendFilter->Update();
        }

        // Remove any duplicate points.
        vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
        cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
        cleanFilter->Update();

        vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
        triangleFilter->SetInputConnection(cleanFilter->GetOutputPort());
        triangleFilter->Update();

        writer->SetInputConnection(triangleFilter->GetOutputPort());

        if (this->m_EncodeSupport == 0) {
            writer->SetFileTypeToASCII();
        } else if (this->m_EncodeSupport == 1) {
            writer->SetFileTypeToBinary();
        }
        writer->Write();
    } else if (m_FileSupport == 3) { //VTP File
        vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
        writer->SetFileName(fileName.toStdString().c_str());

        vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();

        for (unsigned int i = 0; i < this->lstActors.size(); i++) {
            vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
            input->ShallowCopy(vtkPolyData::SafeDownCast(this->lstActors[i]->GetMapper()->GetInput()));

            vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
            translation->Identity();
            translation->Translate(this->lstActors[i]->GetCenter()[0], this->lstActors[i]->GetCenter()[1], this->lstActors[i]->GetCenter()[2]);

            vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            transformFilter->SetInputConnection(input->GetProducerPort());
            transformFilter->SetTransform(translation);
            transformFilter->Update();
            #if VTK_MAJOR_VERSION <= 5
                appendFilter->AddInputConnection(transformFilter->GetOutputPort());
            #else
                appendFilter->AddInputData(transformFilter->GetOutput());
            #endif
            appendFilter->Update();
        }

        // Remove any duplicate points.
        vtkSmartPointer<vtkCleanPolyData> cleanFilter = vtkSmartPointer<vtkCleanPolyData>::New();
        cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
        cleanFilter->Update();

        vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
        triangleFilter->SetInputConnection(cleanFilter->GetOutputPort());
        triangleFilter->Update();

        //writer->SetInputConnection(triangleFilter->GetOutputPort());
        writer->SetInput(triangleFilter->GetOutput());

        if (this->m_EncodeSupport == 0) {
            writer->SetDataModeToAscii();
        } else if (this->m_EncodeSupport == 1) {
            writer->SetDataModeToBinary();
        }
        writer->Write();
    }
}

void STLFileCore::run()
{
    mutex.lock();
    if (m_Type == 0) {
        ReadSTLFile(this->m_FileName);
    } else {
        SaveSTLFile(this->m_FileName);
    }
    mutex.unlock();

    if (m_Type == 0) {
        if (!m_working) {
            emit emitReadSTLDone(this->m_Actor);
        }
    } else {
        if (!m_working) {
            emit emitWriteSTLDone();
        }
    }

    mutex.lock();
    m_working = false;
    mutex.unlock();
}
