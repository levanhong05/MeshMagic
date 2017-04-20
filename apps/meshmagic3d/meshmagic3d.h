/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      MeshMagic3D.h
**
** Created: Wed 06. May 08:16:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef MESHMAGIC3D_H
#define MESHMAGIC3D_H

#include <QList>
#include <qthread.h>
#include <QMainWindow>
#include <QVTKWidget.h>

#include <vtkProp.h>
#include <vtkPoints.h>
#include <vtkIdList.h>
#include <vtkPolyData.h>
#include <vtkAxesActor.h>
#include <vtkIdTypeArray.h>
#include <vtkSmartPointer.h>
#include <vtkAlgorithmOutput.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include "STLFileCore.h"
#include "RotateThread.h"
#include "ExtractionRegionThread.h"

namespace Ui {
class MeshMagic3D;
}

class MeshMagic3D : public QMainWindow
{
    Q_OBJECT

public:
    explicit MeshMagic3D(QWidget *parent = 0);
    ~MeshMagic3D();

private:
    void createActions();
    void initMesh();
    void showAxesCenter();
    void showAxesConner();
    void createToolButtonMenu();
    void setCheckActionDefault();
    void setStyleInteractionDefault();
    void displayAxesConner(vtkSmartPointer<vtkOrientationMarkerWidget> &widget, vtkRenderWindowInteractor *renderWindowInteractor);
    void setButtonEnable(bool value);
    void setExtrasActionEnable(bool value);
    void addShaderViewProps(vtkRenderer *ren);
    void createConnerAnnotation();
    void reallyDeletePoint(vtkSmartPointer<vtkPoints> points, vtkIdType id);

public slots:
    void slotOpen();
    void slotSave();
    void slotSaveAs();
    void slotClose();
    void slotExit();

    void slotAddShadow();
    void slotAddLight();
    void slotAddMaterial();
    void slotColor();
    void slotShowRegion();
    void slotDelaunay3D();

    void slotShowSolid();
    void slotShowWireFrame();
    void slotShowOutline();
    void slotShowEdges();
    void slotShowPolygon();
    void slotShowPoints();

    void slotSelectObject();
    void slotSelectCell();
    void slotSelectCellNeighbors();
    void slotSelectTriangle();
    void slotSelectLine();
    void slotSelectPoint();
    void slotSelectPointNeighbors();

    void slotAddCube();
    void slotAddSphere();
    void slotAddIcosahedron();
    void slotAddTorus();

    void slotResetCamera();
    void slotShowAxesCenter();
    void slotDeleteCell();
    void slotResetView();

    void slotAnaglyph();

    void slotScreenshot();
    void slotFullscreen();

    void slotHelp();
    void slotAbout();

    //slot listen Occulus emit rotate signal
    void listenOcculusRotateX(double angle);
    void listenOcculusRotateY(double angle);
    void listenOcculusRotateZ(double angle);
    void listenOcculusRotateXYZ(double angle);

public:
    vtkSmartPointer<vtkAxesActor> axesActor;

private slots:
    void slotRotateCamera(vtkCamera *cam);
    void slotReadSTLDone(vtkActor *actor);
    void slotWriteSTLDone();
    void slotExtractDone(vtkActor *actor, unsigned int numRegions);

private:
    Ui::MeshMagic3D *ui;

    //User thread object
    RotateThread thread;
    STLFileCore STLCore;
    ExtractionRegionThread extractThread;

    bool isChangeInputFile;

signals:
    void emitTerminate();
};

#endif // MESHMAGIC3D_H
