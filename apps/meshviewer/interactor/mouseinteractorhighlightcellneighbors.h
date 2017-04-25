/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      MouseInteractorHighLightCellNeighbors.h
**
** Created: Mon 11. May 17:05:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef MOUSEINTERACTORHIGHLIGHTCELLNEIGHBORS_H
#define MOUSEINTERACTORHIGHLIGHTCELLNEIGHBORS_H

#include "GlobalVariable.h"
#include <QVTKWidget.h>

#include <vtkCell.h>
#include <vtkActor.h>
#include <vtkIdList.h>
#include <vtkVersion.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkSelection.h>
#include <vtkTransform.h>
#include <vtkCellPicker.h>
#include <vtkPointPicker.h>
#include <vtkIdTypeArray.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkExtractEdges.h>
#include <vtkDataSetMapper.h>
#include <vtkSelectionNode.h>
#include <vtkTriangleFilter.h>
#include <vtkExtractSelection.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <list>

class MouseInteractorHighLightCellNeighbors : public vtkInteractorStyleTrackballCamera
{
public:
    static MouseInteractorHighLightCellNeighbors *New();
    vtkTypeMacro(MouseInteractorHighLightCellNeighbors, vtkInteractorStyleTrackballCamera);

    MouseInteractorHighLightCellNeighbors()
    {
        this->selectedActor = vtkSmartPointer<vtkActor>::New();
        this->selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        this->connectedActor = vtkSmartPointer<vtkActor>::New();
        this->connectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();

        this->selectedActor->SetMapper(selectedMapper);
        this->connectedActor->SetMapper(connectedMapper);

        this->lastPickedActor = NULL;

        cellId = -1;
        neighbors.clear();
    }

    MouseInteractorHighLightCellNeighbors::~MouseInteractorHighLightCellNeighbors()
    {
        this->GetDefaultRenderer()->RemoveActor(this->selectedActor);
        this->GetDefaultRenderer()->RemoveActor(this->connectedActor);
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
        neighbors.clear();

        if (picker->GetCellId() >= 0) {
            this->lastPickedActor = picker->GetActor();
            vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
            triangleFilter->SetInputConnection(this->m_AlgorithmOutput);
            triangleFilter->Update();

            cellId = picker->GetCellId();

            vtkSmartPointer<vtkIdList> cellPointIds = vtkSmartPointer<vtkIdList>::New();
            triangleFilter->GetOutput()->GetCellPoints(cellId, cellPointIds);

            for (vtkIdType i = 0; i < cellPointIds->GetNumberOfIds(); i++) {
                vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();
                //add one of the edge points
                idList->InsertNextId(cellPointIds->GetId(i));
                //add the other edge point
                if ((i + 1) == cellPointIds->GetNumberOfIds()) {
                    idList->InsertNextId(cellPointIds->GetId(0));
                } else {
                    idList->InsertNextId(cellPointIds->GetId(i+1));
                }
                //get the neighbors of the cell
                vtkSmartPointer<vtkIdList> neighborCellIds = vtkSmartPointer<vtkIdList>::New();

                triangleFilter->GetOutput()->GetCellNeighbors(cellId, idList, neighborCellIds);

                for (vtkIdType j = 0; j < neighborCellIds->GetNumberOfIds(); j++) {
                    neighbors.push_back(neighborCellIds->GetId(j));
                }
            }

            strLog = QString("Edge neighbor ids are:");

            for (std::list<vtkIdType>::iterator it1 = neighbors.begin(); it1 != neighbors.end(); it1++) {
                strLog += QString(" %1").arg(*it1);
            }
            console.logInfo(strLog);

            // Create a dataset with the cell of interest
            {
                vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
                ids->SetNumberOfComponents(1);
                ids->InsertNextValue(cellId);

                vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
                selectionNode->SetFieldType(vtkSelectionNode::CELL);
                selectionNode->SetContentType(vtkSelectionNode::INDICES);
                selectionNode->SetSelectionList(ids);

                vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
                selection->AddNode(selectionNode);

                vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
                extractSelection->SetInputConnection(0, this->m_AlgorithmOutput);
                #if VTK_MAJOR_VERSION <= 5
                    extractSelection->SetInput(1, selection);
                #else
                    extractSelection->SetInputData(1, selection);
                #endif
                extractSelection->Update();
                selectedMapper->SetInputConnection(extractSelection->GetOutputPort());
            }

            selectedActor->SetMapper(selectedMapper);
            selectedActor->GetProperty()->SetColor(1,0,0);

            // Create a dataset with the neighbor cells
            {
                vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
                ids->SetNumberOfComponents(1);
                for (std::list<vtkIdType>::iterator it1 = neighbors.begin(); it1 != neighbors.end(); it1++) {
                    ids->InsertNextValue(*it1);
                }

                vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
                selectionNode->SetFieldType(vtkSelectionNode::CELL);
                selectionNode->SetContentType(vtkSelectionNode::INDICES);
                selectionNode->SetSelectionList(ids);

                vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
                selection->AddNode(selectionNode);

                vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();
                extractSelection->SetInputConnection(0, this->m_AlgorithmOutput);
                #if VTK_MAJOR_VERSION <= 5
                extractSelection->SetInput(1, selection);
                #else
                extractSelection->SetInputData(1, selection);
                #endif
                extractSelection->Update();
                connectedMapper->SetInputConnection(extractSelection->GetOutputPort());
            }
            connectedActor->SetMapper(connectedMapper);
            connectedActor->GetProperty()->SetColor(0,1,0);

            // Add the actors to the scene
            this->GetDefaultRenderer()->AddActor(selectedActor);
            this->GetDefaultRenderer()->AddActor(connectedActor);
        }
        // Forward events
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    //Write by Hong DFM 01/06/2015
    virtual void OnKeyPress()
    {
        if (cellId >= 0) {
            // Get the keypress
            vtkRenderWindowInteractor *rwi = this->Interactor;
            std::string key = rwi->GetKeySym();

            // Handle an Delete key or Backspace key
            if(key == "Delete" || key == "BackSpace") {
                this->GetDefaultRenderer()->RemoveActor(this->selectedActor);
                this->GetDefaultRenderer()->RemoveActor(this->connectedActor);

                data->BuildLinks();
                data->DeleteCell(cellId);
                for (std::list<vtkIdType>::iterator it1 = neighbors.begin(); it1 != neighbors.end(); it1++) {
                    data->DeleteCell(*it1);
                }
                data->RemoveDeletedCells();
                this->widget->update();
                cellId = -1;
                neighbors.clear();
            }
        }
        // Forward events
        vtkInteractorStyleTrackballCamera::OnKeyPress();
    }

    void setAlgorithmOutput(vtkAlgorithmOutput *output){this->m_AlgorithmOutput = output;}
    void setPolyData(vtkSmartPointer<vtkPolyData> polyData) {this->data = polyData;}
    void setWidget(QVTKWidget *value) {this->widget = value;}
private:
    vtkIdType cellId;
    QVTKWidget *widget;
    vtkActor *lastPickedActor;
    std::list<vtkIdType> neighbors;
    vtkSmartPointer<vtkPolyData> data;
    vtkAlgorithmOutput *m_AlgorithmOutput;
    vtkSmartPointer<vtkActor> selectedActor;
    vtkSmartPointer<vtkActor> connectedActor;
    vtkSmartPointer<vtkDataSetMapper> selectedMapper;
    vtkSmartPointer<vtkDataSetMapper> connectedMapper;
};

#endif // MOUSEINTERACTORHIGHLIGHTCELLNEIGHBORS_H
