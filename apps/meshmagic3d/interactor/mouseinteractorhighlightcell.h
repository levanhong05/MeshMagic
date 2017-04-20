/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      MouseInteractorHighLightCell.h
**
** Created: Wed 06. May 08:16:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef MOUSEINTERACTORHIGHLIGHTCELL_H
#define MOUSEINTERACTORHIGHLIGHTCELL_H

#include "GlobalVariable.h"
#include <QScrollBar>
#include <QVTKWidget.h>

#include <vtkActor.h>
#include <vtkPlanes.h>
#include <vtkVersion.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkCellData.h>
#include <vtkIdFilter.h>
#include <vtkAreaPicker.h>
#include <vtkIdTypeArray.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkInteractorStyleRubberBandPick.h>

#include <list>

#define VTKISRBP_ORIENT 0
#define VTKISRBP_SELECT 1

class MouseInteractorHighLightCell : public vtkInteractorStyleRubberBandPick
{
public:
    static MouseInteractorHighLightCell *New();
    vtkTypeMacro(MouseInteractorHighLightCell,vtkInteractorStyleRubberBandPick);

    MouseInteractorHighLightCell() : vtkInteractorStyleRubberBandPick()
    {
        this->selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        this->selectedActor = vtkSmartPointer<vtkActor>::New();
        this->selectedActor->SetMapper(selectedMapper);

        cellIds.clear();
    }

    MouseInteractorHighLightCell::~MouseInteractorHighLightCell()
    {
        this->GetDefaultRenderer()->RemoveActor(this->selectedActor);
    }

    virtual void OnLeftButtonUp()
    {
        // Forward events
        vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
        cellIds.clear();

        if (this->CurrentMode == VTKISRBP_SELECT) {
            vtkPlanes *frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

            vtkSmartPointer<vtkIdFilter> idFilter = vtkSmartPointer<vtkIdFilter>::New();
            idFilter->SetInputConnection(this->m_AlgorithmOutput);
            idFilter->SetIdsArrayName("OriginalIds");
            idFilter->Update();

            // This is needed to convert the ouput of vtkIdFilter (vtkDataSet) back to vtkPolyData
            vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
            surfaceFilter->SetInputConnection(idFilter->GetOutputPort());
            surfaceFilter->Update();

            vtkPolyData *input = surfaceFilter->GetOutput();

            vtkSmartPointer<vtkExtractPolyDataGeometry> extractPolyDataGeometry = vtkSmartPointer<vtkExtractPolyDataGeometry>::New();
            #if VTK_MAJOR_VERSION <= 5
                extractPolyDataGeometry->SetInput(input);
            #else
                extractPolyDataGeometry->SetInputData(input);
            #endif
            extractPolyDataGeometry->SetImplicitFunction(frustum);
            extractPolyDataGeometry->Update();

            numOfCells = extractPolyDataGeometry->GetOutput()->GetNumberOfCells();

            vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(extractPolyDataGeometry->GetOutput()->GetCellData()->GetArray("OriginalIds"));
            for (vtkIdType i = 0; i < numOfCells; i++) {
                cellIds.push_back(ids->GetValue(i));
            }

            strLog = QString("Extracted %1 cells.").arg(numOfCells);
            listLogInfo->addItem(strLog);

            #if VTK_MAJOR_VERSION <= 5
                this->selectedMapper->SetInputConnection(extractPolyDataGeometry->GetOutputPort());
            #else
                this->SelectedMapper->SetInputData(extractPolyDataGeometry->GetOutput());
            #endif
            this->selectedMapper->ScalarVisibilityOff();

            this->selectedActor->GetProperty()->SetColor(0.0, 1.0, 0.0); //(R,G,B)
            this->selectedActor->GetProperty()->SetPointSize(5);

            this->GetDefaultRenderer()->AddActor(selectedActor);
            this->GetInteractor()->GetRenderWindow()->Render();
            this->HighlightProp(NULL);
        }
    }

    //Write by Hong DFM 01/06/2015
    virtual void OnKeyPress()
    {
        // Get the keypress
        vtkRenderWindowInteractor *rwi = this->Interactor;
        std::string key = rwi->GetKeySym();

        // Handle an Delete key or Backspace key
        if(key == "Delete" || key == "BackSpace") {
            this->GetDefaultRenderer()->RemoveActor(this->selectedActor);

            data->BuildLinks();
            for (std::list<vtkIdType>::iterator it1 = cellIds.begin(); it1 != cellIds.end(); it1++) {
                data->DeleteCell(*it1);
            }
            data->RemoveDeletedCells();
            this->widget->update();
            cellIds.clear();
        }
        // Forward events
        vtkInteractorStyleRubberBandPick::OnKeyPress();
    }

    void setAlgorithmOutput(vtkAlgorithmOutput *output){this->m_AlgorithmOutput = output;}
    void setPolyData(vtkSmartPointer<vtkPolyData> polyData) {this->data = polyData;}
    void setWidget(QVTKWidget *value) {this->widget = value;}
private:
    QVTKWidget *widget;
    std::list<vtkIdType> cellIds;
    vtkSmartPointer<vtkPolyData> data;
    vtkAlgorithmOutput *m_AlgorithmOutput;
    vtkSmartPointer<vtkActor> selectedActor;
    vtkSmartPointer<vtkDataSetMapper> selectedMapper;
};

#endif // MOUSEINTERACTORHIGHLIGHTCELL_H
