/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      MouseInteractorHighLightPoint.h
**
** Created: Wed 06. May 08:16:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef MOUSEINTERACTORHIGHLIGHTPOINT_H
#define MOUSEINTERACTORHIGHLIGHTPOINT_H

#include "GlobalVariable.h"
#include <QScrollBar>

#include <vtkActor.h>
#include <vtkIdList.h>
#include <vtkPlanes.h>
#include <vtkVersion.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkAreaPicker.h>
#include <vtkIdTypeArray.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkExtractGeometry.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleRubberBandPick.h>

class MouseInteractorHighLightPoint : public vtkInteractorStyleRubberBandPick
{
public:
    static MouseInteractorHighLightPoint *New();
    vtkTypeMacro(MouseInteractorHighLightPoint,vtkInteractorStyleRubberBandPick);

    MouseInteractorHighLightPoint() : vtkInteractorStyleRubberBandPick()
    {
        this->selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        this->selectedActor = vtkSmartPointer<vtkActor>::New();
        this->selectedActor->SetMapper(selectedMapper);
    }

    MouseInteractorHighLightPoint::~MouseInteractorHighLightPoint()
    {
        this->GetDefaultRenderer()->RemoveActor(this->selectedActor);
    }

    virtual void OnLeftButtonUp()
    {
        // Forward events
        vtkInteractorStyleRubberBandPick::OnLeftButtonUp();

        vtkPlanes *frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

        if (frustum) {
            vtkSmartPointer<vtkExtractGeometry> extractGeometry = vtkSmartPointer<vtkExtractGeometry>::New();
            extractGeometry->SetImplicitFunction(frustum);
            #if VTK_MAJOR_VERSION <= 5
                extractGeometry->SetInput(this->data);
            #else
                extractGeometry->SetInputData(this->data);
            #endif
            extractGeometry->Update();

            vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
            glyphFilter->SetInputConnection(extractGeometry->GetOutputPort());
            glyphFilter->Update();

            vtkPolyData *selected = glyphFilter->GetOutput();
            numOfPoints = selected->GetNumberOfPoints();

            strLog = QString("Selected %1 points.").arg(numOfPoints);
            listLogInfo->addItem(strLog);
            //listLogInfo->scrollToBottom();

            #if VTK_MAJOR_VERSION <= 5
                this->selectedMapper->SetInput(selected);
            #else
                this->SelectedMapper->SetInputData(selected);
            #endif
            this->selectedMapper->ScalarVisibilityOff();

            this->selectedActor->GetProperty()->SetColor(0.0, 1.0, 0.0); //(R,G,B)
            this->selectedActor->GetProperty()->SetPointSize(5);

            this->GetDefaultRenderer()->AddActor(selectedActor);
            this->GetInteractor()->GetRenderWindow()->Render();
            this->HighlightProp(NULL);
        }
    }

    void SetPoints(vtkSmartPointer<vtkPolyData> polyData) {this->data = polyData;}
private:
    vtkSmartPointer<vtkPolyData> data;
    vtkSmartPointer<vtkActor> selectedActor;
    vtkSmartPointer<vtkDataSetMapper> selectedMapper;
};

#endif // MOUSEINTERACTORHIGHLIGHTPOINT_H
