/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      KeyInteractorExtractRegions.h
**
** Created: Thu 14. May 16:45:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef KEYINTERACTOREXTRACTREGIONS_H
#define KEYINTERACTOREXTRACTREGIONS_H

#include "GlobalVariable.h"
#include <QVTKWidget.h>

#include <vtkActor.h>
#include <vtkPropPicker.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>

class KeyInteractorExtractRegions : public vtkInteractorStyleTrackballCamera
{
public:
    static KeyInteractorExtractRegions *New();
    vtkTypeMacro(KeyInteractorExtractRegions, vtkInteractorStyleTrackballCamera);

    KeyInteractorExtractRegions()
    {
        selectActor = NULL;
    }

    virtual void OnLeftButtonUp()
    {
        int *clickPos = this->GetInteractor()->GetEventPosition();

        // Pick from this location.
        vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
        picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

        this->selectActor = picker->GetActor();

        // Forward events
        vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
    }

    virtual void OnMiddleButtonUp()
    {
        int *clickPos = this->GetInteractor()->GetEventPosition();

        // Pick from this location.
        vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
        picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

        this->selectActor = picker->GetActor();

        // Forward events
        vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
    }

    virtual void OnRightButtonUp()
    {
        int *clickPos = this->GetInteractor()->GetEventPosition();

        // Pick from this location.
        vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
        picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

        this->selectActor = picker->GetActor();

        // Forward events
        vtkInteractorStyleTrackballCamera::OnRightButtonUp();
    }

    virtual void OnKeyPress()
    {
        // Get the keypress
        vtkRenderWindowInteractor *rwi = this->Interactor;
        std::string key = rwi->GetKeySym();

        // Handle an Delete key or Backspace key
        if(key == "Delete" || key == "BackSpace") {
            this->GetDefaultRenderer()->RemoveActor(this->selectActor);
            this->widget->update();
        }
        // Forward events
        vtkInteractorStyleTrackballCamera::OnKeyPress();
    }

    void setWidget(QVTKWidget *value) {this->widget = value;}

private:
    QVTKWidget *widget;
    vtkActor *selectActor;
};

#endif // KEYINTERACTOREXTRACTREGIONS_H
