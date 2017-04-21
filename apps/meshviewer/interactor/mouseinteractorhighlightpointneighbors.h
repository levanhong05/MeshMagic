/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      MouseInteractorHighLightPointNeighbors.h
**
** Created: Mon 11. May 10:01:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef MOUSEINTERACTORHIGHLIGHTPOINTNEIGHBORS_H
#define MOUSEINTERACTORHIGHLIGHTPOINTNEIGHBORS_H

#include "GlobalVariable.h"

#include <vtkCell.h>
#include <vtkActor.h>
#include <vtkIdList.h>
#include <vtkVersion.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkSelection.h>
#include <vtkCellPicker.h>
#include <vtkPointPicker.h>
#include <vtkIdTypeArray.h>
#include <vtkRenderWindow.h>
#include <vtkExtractEdges.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkSelectionNode.h>
#include <vtkTriangleFilter.h>
#include <vtkExtractSelection.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

vtkSmartPointer<vtkIdList> GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int id)
{
    vtkSmartPointer<vtkIdList> connectedVertices = vtkSmartPointer<vtkIdList>::New();

    //get all cells that vertex 'id' is a part of
    vtkSmartPointer<vtkIdList> cellIdList = vtkSmartPointer<vtkIdList>::New();
    mesh->GetPointCells(id, cellIdList);

    for (vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++) {
        vtkSmartPointer<vtkIdList> pointIdList = vtkSmartPointer<vtkIdList>::New();
        mesh->GetCellPoints(cellIdList->GetId(i), pointIdList);

        if (pointIdList->GetId(0) != id) {
            connectedVertices->InsertNextId(pointIdList->GetId(0));
        } else {
            connectedVertices->InsertNextId(pointIdList->GetId(1));
        }
    }
    return connectedVertices;
}

class MouseInteractorHighLightPointNeighbors : public vtkInteractorStyleTrackballCamera
{
public:
    static MouseInteractorHighLightPointNeighbors *New();
    vtkTypeMacro(MouseInteractorHighLightPointNeighbors, vtkInteractorStyleTrackballCamera);

    MouseInteractorHighLightPointNeighbors()
    {
        selectedPointActor = vtkSmartPointer<vtkActor>::New();
        selectedPointMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        connectedPointActor = vtkSmartPointer<vtkActor>::New();
        connectedPointMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        connectedPointActor->SetMapper(connectedPointMapper);
        selectedPointActor->SetMapper(selectedPointMapper);

        this->lastPickedActor = NULL;
    }

    MouseInteractorHighLightPointNeighbors::~MouseInteractorHighLightPointNeighbors()
    {
        this->GetDefaultRenderer()->RemoveActor(this->selectedPointActor);
        this->GetDefaultRenderer()->RemoveActor(this->connectedPointActor);
    }

    virtual void OnLeftButtonDown()
    {
        // Get the location of the click (in window coordinates)
        int *pos = this->GetInteractor()->GetEventPosition();

        vtkSmartPointer<vtkPointPicker> picker = vtkSmartPointer<vtkPointPicker>::New();
        picker->SetTolerance(0.005);
        // Pick from this location.
        picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

        if (picker->GetPointId() >= 0) {
            this->lastPickedActor = picker->GetActor();

            vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
            triangleFilter->SetInputConnection(this->m_AlgorithmOutput);
            triangleFilter->Update();

            vtkSmartPointer<vtkExtractEdges> extractEdges = vtkSmartPointer<vtkExtractEdges>::New();
            extractEdges->SetInputConnection(triangleFilter->GetOutputPort());
            extractEdges->Update();

            vtkSmartPointer<vtkPolyData> mesh = extractEdges->GetOutput();

            vtkSmartPointer<vtkIdList> connectedVertices = GetConnectedVertices(mesh, picker->GetPointId());

            vtkSmartPointer<vtkIdTypeArray> ids = vtkSmartPointer<vtkIdTypeArray>::New();
            ids->SetNumberOfComponents(1);

            for (vtkIdType i = 0; i < connectedVertices->GetNumberOfIds(); i++) {
                ids->InsertNextValue(connectedVertices->GetId(i));
            }

            {
                vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
                selectionNode->SetFieldType(vtkSelectionNode::POINT);
                selectionNode->SetContentType(vtkSelectionNode::INDICES);
                selectionNode->SetSelectionList(ids);

                vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
                selection->AddNode(selectionNode);

                vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();

                extractSelection->SetInputConnection(0, extractEdges->GetOutputPort());
                #if VTK_MAJOR_VERSION <= 5
                    extractSelection->SetInput(1, selection);
                #else
                    extractSelection->SetInputData(1, selection);
                #endif
                extractSelection->Update();

                vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
                glyphFilter->SetInputConnection(extractSelection->GetOutputPort());
                glyphFilter->Update();

                connectedPointMapper->SetInputConnection(glyphFilter->GetOutputPort());
            }
            connectedPointActor->GetProperty()->SetColor(0,1,0);
            connectedPointActor->GetProperty()->SetPointSize(5);

            {
                vtkSmartPointer<vtkIdTypeArray> ids2 = vtkSmartPointer<vtkIdTypeArray>::New();
                ids2->SetNumberOfComponents(1);
                ids2->InsertNextValue(picker->GetPointId());

                vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
                selectionNode->SetFieldType(vtkSelectionNode::POINT);
                selectionNode->SetContentType(vtkSelectionNode::INDICES);
                selectionNode->SetSelectionList(ids2);

                vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
                selection->AddNode(selectionNode);

                vtkSmartPointer<vtkExtractSelection> extractSelection = vtkSmartPointer<vtkExtractSelection>::New();

                extractSelection->SetInputConnection(0, extractEdges->GetOutputPort());
                #if VTK_MAJOR_VERSION <= 5
                    extractSelection->SetInput(1, selection);
                #else
                    extractSelection->SetInputData(1, selection);
                #endif
                extractSelection->Update();

                vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
                glyphFilter->SetInputConnection(extractSelection->GetOutputPort());
                glyphFilter->Update();

                selectedPointMapper->SetInputConnection(glyphFilter->GetOutputPort());
            }
            selectedPointActor->GetProperty()->SetColor(1,0,0);
            selectedPointActor->GetProperty()->SetPointSize(5);

            this->GetDefaultRenderer()->AddActor(selectedPointActor);
            this->GetDefaultRenderer()->AddActor(connectedPointActor);
        }
        // Forward events
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }

    void setAlgorithmOutput(vtkAlgorithmOutput *output){this->m_AlgorithmOutput = output;}
private:
    vtkActor *lastPickedActor;
    vtkAlgorithmOutput *m_AlgorithmOutput;
    vtkSmartPointer<vtkActor> selectedPointActor;
    vtkSmartPointer<vtkActor> connectedPointActor;
    vtkSmartPointer<vtkDataSetMapper> selectedPointMapper;
    vtkSmartPointer<vtkDataSetMapper> connectedPointMapper;
};

#endif // MOUSEINTERACTORHIGHLIGHTPOINTNEIGHBORS_H
