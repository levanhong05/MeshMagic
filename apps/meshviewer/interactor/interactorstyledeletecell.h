/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      InteractorStyleDeleteCell.h
**
** Created: Mon 18. May 13:12:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef INTERACTORSTYLEDELETECELL_H
#define INTERACTORSTYLEDELETECELL_H

#include "GlobalVariable.h"
#include <QAction>

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <vtkCellPicker.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

class InteractorStyleDeleteCell : public vtkInteractorStyleTrackballCamera
{
public:
    static InteractorStyleDeleteCell *New();
    vtkTypeMacro(InteractorStyleDeleteCell, vtkInteractorStyleTrackballCamera);

    virtual void OnLeftButtonDown()
    {
        if (this->actionCaller->isChecked()) {
            // Get the location of the click (in window coordinates)
            int *pos = this->GetInteractor()->GetEventPosition();

            vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
            picker->SetTolerance(0.0005);

            // Pick from this location.
            picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

            if (picker->GetCellId() >= 0) {
                // Tell the polydata to build 'upward' links from points to cells.
                data->BuildLinks();
                // Mark a cell as deleted.
                data->DeleteCell(picker->GetCellId());
                // Remove the marked cell.
                data->RemoveDeletedCells();

                this->GetInteractor()->GetRenderWindow()->Render();
                this->HighlightProp(NULL);
                actionCaller->setChecked(false);
            }
        }

        // Forward events
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    void setPolyData(vtkSmartPointer<vtkPolyData> polyData) {this->data = polyData;}
    void setActionCaller(QAction *value) {this->actionCaller = value;}

private:
    QAction *actionCaller;
    vtkSmartPointer<vtkPolyData> data;
};

#endif // INTERACTORSTYLEDELETECELL_H
