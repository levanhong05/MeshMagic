/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      GlobalVariable.h
**
** Created: Wed 06. May 08:16:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** This module define global variables, which used in program
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H

#include <QToolButton>
#include <QListWidget>

#include <vtkProp.h>
#include <vtkActor.h>
#include <vtkLight.h>
#include <vtkProp3D.h>
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkIdTypeArray.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkAlgorithmOutput.h>
#include <vtkInteractorObserver.h>
#include <vtkOrientationMarkerWidget.h>

#include <vector>

vtkProperty *lastProperty;
vtkSmartPointer<vtkActor> actorSTL;
vtkSmartPointer<vtkLight> lightMain;
vtkSmartPointer<vtkRenderer> renderer;
vtkSmartPointer<vtkOrientationMarkerWidget> widgetAxesConner;

long numOfCells;
long numOfPoints;
bool isAnaglyph;

QString strLog;
QString nameFileInput;
QToolButton *toolSelect;
QToolButton *toolAddObject;
QToolButton *toolView;
QListWidget *listLogInfo;

std::vector<vtkSmartPointer<vtkActor> > lstActors;
std::vector<vtkSmartPointer<vtkLight> > lstLights;
//std::vector<vtkSmartPointer<vtkActor> > lstRegions;
std::vector<vtkSmartPointer<vtkProp> > lstLightActors;
#endif // GLOBALVARIABLE_H
