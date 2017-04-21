/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      MouseInteractorHighLightActor.h
**
** Created: Wed 06. May 08:16:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef MOUSEINTERACTORHIGHLIGHTACTOR_H
#define MOUSEINTERACTORHIGHLIGHTACTOR_H

#include "GlobalVariable.h"
#include <QVTKWidget.h>

#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkPropPicker.h>
#include <vtkIdTypeArray.h>
#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>

// Handle mouse events
class MouseInteractorHighLightActor : public vtkInteractorStyleTrackballCamera
{
public:
    static MouseInteractorHighLightActor *New();
    vtkTypeMacro(MouseInteractorHighLightActor, vtkInteractorStyleTrackballCamera);

    MouseInteractorHighLightActor()
    {
        lastPickedActor = NULL;
        lastPickedProperty = vtkProperty::New();
    }
    virtual ~MouseInteractorHighLightActor()
    {
        lastPickedProperty->Delete();
    }
    virtual void OnLeftButtonDown()
    {
        int *clickPos = this->GetInteractor()->GetEventPosition();

        // Pick from this location.
        vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
        picker->Pick(clickPos[0], clickPos[1], 0, this->GetDefaultRenderer());

        // If we picked something before, reset its property
        if (this->lastPickedActor) {
            this->lastPickedActor->GetProperty()->DeepCopy(this->lastPickedProperty);
        }
        this->lastPickedActor = picker->GetActor();

        if (this->lastPickedActor) {
            // Save the property of the picked actor so that we can restore it next time
            this->lastPickedProperty->DeepCopy(this->lastPickedActor->GetProperty());
            // Highlight the picked actor by changing its properties
            this->lastPickedActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
            this->lastPickedActor->GetProperty()->SetDiffuse(1.0);
            this->lastPickedActor->GetProperty()->SetSpecular(0.0);
        }
        // Forward events
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    //Write by Hong DFM 01/06/2015
    virtual void OnKeyPress()
    {
        // Get the keypress
        vtkRenderWindowInteractor *rwi = this->Interactor;
        std::string key = rwi->GetKeySym();

        // Handle an Delete key or Backspace key
        if(key == "Delete" || key == "BackSpace") {
            this->GetDefaultRenderer()->RemoveActor(this->lastPickedActor);
            data = vtkPolyData::SafeDownCast(this->lastPickedActor->GetMapper()->GetInput());
            data->BuildLinks();
            for (vtkIdType it1 = 0; it1 < data->GetNumberOfCells(); it1++) {
                data->DeleteCell(it1);
            }
            data->RemoveDeletedCells();
            this->widget->update();
        }
        // Forward events
        vtkInteractorStyleTrackballCamera::OnKeyPress();
    }

    void setWidget(QVTKWidget *value) {this->widget = value;}

private:
    QVTKWidget *widget;
    vtkActor *lastPickedActor;
    vtkProperty *lastPickedProperty;
    vtkSmartPointer<vtkPolyData> data;
};

#endif // MOUSEINTERACTORHIGHLIGHTACTOR_H
