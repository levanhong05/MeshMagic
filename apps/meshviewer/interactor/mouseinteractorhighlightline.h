/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      MouseInteractorHighLightLine.h
**
** Created: Wed 06. May 17:07:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef MOUSEINTERACTORHIGHLIGHTLINE_H
#define MOUSEINTERACTORHIGHLIGHTLINE_H

#include "GlobalVariable.h"
#include <QVTKWidget.h>

#include <vtkActor.h>
#include <vtkVersion.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkSelection.h>
#include <vtkCellPicker.h>
#include <vtkIdTypeArray.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkSelectionNode.h>
#include <vtkTriangleFilter.h>
#include <vtkExtractSelection.h>
#include <vtkUnstructuredGrid.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

class MouseInteractorHighLightLine : public vtkInteractorStyleTrackballCamera
{
public:
    static MouseInteractorHighLightLine *New();
    vtkTypeMacro(MouseInteractorHighLightLine, vtkInteractorStyleTrackballCamera);

    MouseInteractorHighLightLine()
    {
        selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        selectedActor = vtkSmartPointer<vtkActor>::New();

        cellId = -1;
    }

    MouseInteractorHighLightLine::~MouseInteractorHighLightLine()
    {
        this->GetDefaultRenderer()->RemoveActor(this->selectedActor);
    }

    virtual void OnLeftButtonDown()
    {
        // Get the location of the click (in window coordinates)
        int *pos = this->GetInteractor()->GetEventPosition();
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0005);

        // Pick from this location.
        picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

        cellId = -1;

        if (picker->GetCellId() >= 0) {
            double *worldPosition = picker->GetPickPosition();
            strLog = QString("Selected cell id is: %1.\n").arg(picker->GetCellId());
            strLog = QString("Pick position is: (%1, %2, %3).").arg(worldPosition[0]).arg(worldPosition[1]).arg(worldPosition[2]);
            listLogInfo->addItem(strLog);

            cellId = picker->GetCellId();

            vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
            ids->SetNumberOfComponents(1);
            ids->InsertNextValue(picker->GetCellId());

            vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
            selectionNode->SetFieldType(vtkSelectionNode::CELL);
            selectionNode->SetContentType(vtkSelectionNode::INDICES);
            selectionNode->SetSelectionList(ids);

            vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
            triangleFilter->SetInputConnection(this->m_AlgorithmOutput);
            triangleFilter->Update();

            vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
            selection->AddNode(selectionNode);

            vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
            #if VTK_MAJOR_VERSION <= 5
                extractSelection->SetInput(0, triangleFilter->GetOutput());
                extractSelection->SetInput(1, selection);
            #else
                extractSelection->SetInputData(0, triangleFilter->GetOutput());
                extractSelection->SetInputData(1, selection);
            #endif
            extractSelection->Update();

            // In selection
            vtkSmartPointer<vtkUnstructuredGrid> selected = vtkSmartPointer<vtkUnstructuredGrid>::New();
            selected->ShallowCopy(extractSelection->GetOutput());

            #if VTK_MAJOR_VERSION <= 5
                selectedMapper->SetInputConnection(selected->GetProducerPort());
            #else
                selectedMapper->SetInputData(selected);
            #endif

            selectedActor->SetMapper(selectedMapper);
            selectedActor->GetProperty()->EdgeVisibilityOn();
            selectedActor->GetProperty()->SetEdgeColor(0,1,0);
            selectedActor->GetProperty()->SetLineWidth(1);
            selectedActor->GetProperty()->SetColor(0,1,0);

            this->GetDefaultRenderer()->AddActor(selectedActor);
        }
        // Forward events
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    virtual void OnKeyPress()
    {
        if (cellId >= 0) {
            // Get the keypress
            vtkRenderWindowInteractor *rwi = this->Interactor;
            std::string key = rwi->GetKeySym();

            // Handle an Delete key or Backspace key
            if(key == "Delete" || key == "BackSpace") {
                this->GetDefaultRenderer()->RemoveActor(this->selectedActor);

                data->BuildLinks();
                data->DeleteCell(cellId);
                data->RemoveDeletedCells();

                this->widget->update();
                cellId = -1;
            }
        }
        // Forward events
        vtkInteractorStyleTrackballCamera::OnKeyPress();
    }

    void setPolyData(vtkSmartPointer<vtkPolyData> polyData) {this->data = polyData;}
    void setAlgorithmOutput(vtkAlgorithmOutput *output){this->m_AlgorithmOutput = output;}
    void setWidget(QVTKWidget *value) {this->widget = value;}

private:
    vtkIdType cellId;
    QVTKWidget *widget;
    vtkSmartPointer<vtkPolyData> data;
    vtkAlgorithmOutput *m_AlgorithmOutput;
    vtkSmartPointer<vtkActor> selectedActor;
    vtkSmartPointer<vtkDataSetMapper> selectedMapper;
};

#endif // MOUSEINTERACTORHIGHLIGHTLINE_H
