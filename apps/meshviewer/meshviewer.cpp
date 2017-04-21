/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      MeshMagic3D.cpp
**
** Created: Wed 06. May 08:16:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#include "meshviewer.h"
#include "about.h"
#include "configurewriter.h"
#include "ui_meshviewer.h"

#include "version.h"
#include "globalvariable.h"

//User-interaction
#include "interactor/interactorstyledeletecell.h"
#include "interactor/keyinteractorextractregions.h"
#include "interactor/mouseinteractorhighlightcell.h"
#include "interactor/mouseinteractorhighlightline.h"
#include "interactor/mouseinteractorhighlightactor.h"
#include "interactor/mouseinteractorhighlightpoint.h"
#include "interactor/mouseinteractorhighlighttriangle.h"
#include "interactor/mouseinteractorhighlightcellneighbors.h"
#include "interactor/mouseinteractorhighlightpointneighbors.h"

#include <math.h>

//Qt Library
#include <QUrl>
#include <QMenu>
#include <QList>
#include <QTime>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolButton>
#include <QColorDialog>
#include <QInputDialog>
#include <QDesktopServices>
#include <QDebug>

//General Library
#include <vtkMath.h>
#include <vtkCutter.h>
#include <vtkIdList.h>
#include <vtkCommand.h>
#include <vtkProperty.h>
#include <vtkStripper.h>
#include <vtkAssembly.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>
#include <vtkPropAssembly.h>
#include <vtkObjectFactory.h>
#include <vtkAlgorithmOutput.h>

//Filter Library
#include <vtkIdFilter.h>
#include <vtkOutlineFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataConnectivityFilter.h>

//Interactor Library
#include <vtkInteractorStyleSwitch.h>
#include <vtkInteractorStyleTrackballCamera.h>

//Read/Write Library
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>

#include <vtkPNGReader.h>
#include <vtkPNGWriter.h>

#include <vtkJPEGWriter.h>
#include <vtkJPEGReader.h>

#include <vtkImageReader.h>
#include <vtkImageWriter.h>

#include <vtkImageReader2Factory.h>

//Object Source Library
#include <vtkCubeSource.h>
#include <vtkPlaneSource.h>
#include <vtkSphereSource.h>
#include <vtkParametricTorus.h>
#include <vtkPlatonicSolidSource.h>
#include <vtkParametricFunctionSource.h>

//Render Library
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPropCollection.h>
#include <vtkLightCollection.h>
#include <vtkActorCollection.h>
#include <vtkRendererCollection.h>

//Actor Library
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkLightActor.h>
#include <vtkScalarBarActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkCornerAnnotation.h>
#include <vtkOrientationMarkerWidget.h>

//Poly Library
#include <vtkPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkPolyDataMapper.h>

//Data Library
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkDelaunay2D.h>
#include <vtkDelaunay3D.h>
#include <vtkLookupTable.h>
#include <vtkAppendPoints.h>
#include <vtkExtractEdges.h>
#include <vtkUnsignedCharArray.h>
#include <vtkSelectVisiblePoints.h>

//Shadow Library
#include <vtkCameraPass.h>
#include <vtkOpaquePass.h>
#include <vtkLightsPass.h>
#include <vtkOverlayPass.h>
#include <vtkSequencePass.h>
#include <vtkShadowMapPass.h>
#include <vtkVolumetricPass.h>
#include <vtkTranslucentPass.h>
#include <vtkDepthPeelingPass.h>
#include <vtkShadowMapBakerPass.h>
#include <vtkRenderPassCollection.h>

//Object Library
#include <vtkLine.h>
#include <vtkPlane.h>
#include <vtkLight.h>
#include <vtkPoints.h>
#include <vtkProp3D.h>
#include <vtkCamera.h>

//Other Library
#include <vtkAreaPicker.h>
#include <vtkInformation.h>
#include <vtkTextProperty.h>
#include <vtkFrameBufferObject.h>
#include <vtkTextureMapToPlane.h>
#include <vtkWindowToImageFilter.h>
#include <vtkTransformTextureCoords.h>

vtkStandardNewMacro(InteractorStyleDeleteCell);
vtkStandardNewMacro(KeyInteractorExtractRegions);
vtkStandardNewMacro(MouseInteractorHighLightCell);
vtkStandardNewMacro(MouseInteractorHighLightLine);
vtkStandardNewMacro(MouseInteractorHighLightActor);
vtkStandardNewMacro(MouseInteractorHighLightPoint);
vtkStandardNewMacro(MouseInteractorHighLightTriangle);
vtkStandardNewMacro(MouseInteractorHighLightCellNeighbors);
vtkStandardNewMacro(MouseInteractorHighLightPointNeighbors);

MeshViewer::MeshViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MeshViewer)
{
    ui->setupUi(this);
    listLogInfo = new QListWidget;
    listLogInfo = new QListWidget(ui->centralWidget);
    listLogInfo->setObjectName(QString::fromUtf8("listLogInfo"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(listLogInfo->sizePolicy().hasHeightForWidth());
    listLogInfo->setSizePolicy(sizePolicy1);
    listLogInfo->setMaximumSize(QSize(16777215, 100));
    ui->mainLayout->addWidget(listLogInfo);
    initMesh();
    widgetAxesConner = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    createActions();
    createToolButtonMenu();
    setCheckActionDefault();
    setButtonEnable(false);
    setExtrasActionEnable(false);
    setStyleInteractionDefault();
    lastProperty = vtkProperty::New();
    isChangeInputFile = false;
    ui->actionAnaglyph->setEnabled(false);
    ui->statusBar->showMessage(QString("Mesh Magic 3D v%1").arg(APP_VERSION_SHORT));
    listLogInfo->addItem(tr("Welcome to Mesh Magic 3D !!!"));
    QWidget::showMaximized();
}

void MeshViewer::createActions()
{
    //Set up action signals and slots
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(slotOpen()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(slotSave()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(slotSaveAs()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(slotClose()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(ui->actionAddShader, SIGNAL(triggered()), this, SLOT(slotAddShadow()));
    connect(ui->actionAddLight, SIGNAL(triggered()), this, SLOT(slotAddLight()));
    connect(ui->actionAddMaterial, SIGNAL(triggered()), this, SLOT(slotAddMaterial()));
    connect(ui->actionColor, SIGNAL(triggered()), this, SLOT(slotColor()));
    connect(ui->actionShowZones, SIGNAL(triggered()), this, SLOT(slotShowRegion()));
    connect(ui->actionDelaunay3D, SIGNAL(triggered()), this, SLOT(slotDelaunay3D()));
    connect(ui->actionSolid, SIGNAL(triggered()), this, SLOT(slotShowSolid()));
    connect(ui->actionWireFrame, SIGNAL(triggered()), this, SLOT(slotShowWireFrame()));
    connect(ui->actionOutline, SIGNAL(triggered()), this, SLOT(slotShowOutline()));
    connect(ui->actionEdges, SIGNAL(triggered()), this, SLOT(slotShowEdges()));
    connect(ui->actionPolygon, SIGNAL(triggered()), this, SLOT(slotShowPolygon()));
    connect(ui->actionPoints, SIGNAL(triggered()), this, SLOT(slotShowPoints()));
    connect(ui->actionSelectObject, SIGNAL(triggered()), this, SLOT(slotSelectObject()));
    connect(ui->actionSelectCell, SIGNAL(triggered()), this, SLOT(slotSelectCell()));
    connect(ui->actionSelectCellNeighbors, SIGNAL(triggered()), this, SLOT(slotSelectCellNeighbors()));
    connect(ui->actionSelectTriangle, SIGNAL(triggered()), this, SLOT(slotSelectTriangle()));
    connect(ui->actionSelectLine, SIGNAL(triggered()), this, SLOT(slotSelectLine()));
    connect(ui->actionSelectPoint, SIGNAL(triggered()), this, SLOT(slotSelectPoint()));
    connect(ui->actionSelectPointNeighbors, SIGNAL(triggered()), this, SLOT(slotSelectPointNeighbors()));
    connect(ui->actionAddCube, SIGNAL(triggered()), this, SLOT(slotAddCube()));
    connect(ui->actionAddSphere, SIGNAL(triggered()), this, SLOT(slotAddSphere()));
    connect(ui->actionAddIcosahedron, SIGNAL(triggered()), this, SLOT(slotAddIcosahedron()));
    connect(ui->actionAddTorus, SIGNAL(triggered()), this, SLOT(slotAddTorus()));
    connect(ui->actionDeleteCell, SIGNAL(triggered()), this, SLOT(slotDeleteCell()));
    connect(ui->actionResetCenter, SIGNAL(triggered()), this, SLOT(slotResetCamera()));
    connect(ui->actionShowAxesCenter, SIGNAL(triggered()), this, SLOT(slotShowAxesCenter()));
    connect(ui->actionResetView, SIGNAL(triggered()), this, SLOT(slotResetView()));
    connect(ui->actionAnaglyph, SIGNAL(triggered()), this, SLOT(slotAnaglyph()));
    connect(ui->actionScreenshot, SIGNAL(triggered()), this, SLOT(slotScreenshot()));
    connect(ui->actionFullscreen, SIGNAL(triggered()), this, SLOT(slotFullscreen()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
    connect(ui->actionHelpContent, SIGNAL(triggered()), this, SLOT(slotHelp()));
    connect(&_threadRotate, SIGNAL(emitSetCamera(vtkCamera *)), this, SLOT(slotRotateCamera(vtkCamera *)));
    connect(&_threadRotate, SIGNAL(finished()), ui->qvtkWidget, SLOT(update()));
    connect(this, SIGNAL(emitTerminate()), &_threadRotate, SLOT(quit()));
    connect(&STLCore, SIGNAL(emitReadSTLDone(vtkActor *)), this, SLOT(slotReadSTLDone(vtkActor *)));
    connect(&STLCore, SIGNAL(emitWriteSTLDone()), this, SLOT(slotWriteSTLDone()));
    connect(&STLCore, SIGNAL(finished()), ui->qvtkWidget, SLOT(update()));
    connect(this, SIGNAL(emitTerminate()), &STLCore, SLOT(quit()));
    connect(&extractThread, SIGNAL(emitExtractDone(vtkActor *, unsigned int)), this, SLOT(slotExtractDone(vtkActor *, unsigned int)));
    connect(&extractThread, SIGNAL(finished()), ui->qvtkWidget, SLOT(update()));
    connect(this, SIGNAL(emitTerminate()), &extractThread, SLOT(quit()));
}

void MeshViewer::createConnerAnnotation()
{
    //Annotate the image with window/level and mouse over pixel information
    vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation =  vtkSmartPointer<vtkCornerAnnotation>::New();
    cornerAnnotation->SetLinearFontScaleFactor(2);
    cornerAnnotation->SetNonlinearFontScaleFactor(1);
    cornerAnnotation->SetMaximumFontSize(10);
    cornerAnnotation->SetText(3, tr("Version ").toUtf8() + QString(APP_VERSION_SHORT).toUtf8());
    cornerAnnotation->SetText(2, tr("Mesh Magic 3D").toUtf8());
    cornerAnnotation->GetTextProperty()->SetColor(1, 1, 1);
    renderer->AddViewProp(cornerAnnotation);
    renderer->ResetCamera();
}

void MeshViewer::initMesh()
{
    renderer = vtkSmartPointer<vtkRenderer>::New();
    // Setup the background gradient
    //renderer->SetBackground(.2, .3, .4);
    renderer->GradientBackgroundOn();
    renderer->SetBackground(0.9, 0.9, 0.9); //1,1,1
    renderer->SetBackground2(0, 0.30196078431372549019607843137255, 0.6);
    //Corner text
    createConnerAnnotation();
    renderer->ResetCamera();
    // VTK/Qt wedded
    ui->qvtkWidget->GetRenderWindow()->StereoCapableWindowOn();
    ui->qvtkWidget->GetRenderWindow()->StereoRenderOn();
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void MeshViewer::showAxesCenter()
{
    if (actorSTL != NULL) {
        // Add the axes to the scene
        vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
        transform->PostMultiply();
        transform->Translate(actorSTL->GetCenter()[0], actorSTL->GetCenter()[1], actorSTL->GetCenter()[2]);
        //qDebug() << actorMain->GetCenter()[0] << "," << actorMain->GetCenter()[1] << "," <<  actorMain->GetCenter()[2];
        axesActor = vtkSmartPointer<vtkAxesActor>::New();
        axesActor->SetUserTransform(transform);
        axesActor->AxisLabelsOff();
        double l[3];
        l[0] = (actorSTL->GetBounds()[1] - actorSTL->GetBounds()[0]);
        l[1] = (actorSTL->GetBounds()[3] - actorSTL->GetBounds()[2]);
        l[2] = (actorSTL->GetBounds()[5] - actorSTL->GetBounds()[4]);
        axesActor->SetTotalLength(l);
        axesActor->SetConeRadius(0);
        // Add the actors to the scene
        //renderer->AddViewProp(axesActor);
        renderer->AddActor(axesActor);
        /*double wc1[3];
        double fp[3];

        renderer->GetActiveCamera()->GetFocalPoint(fp);
        renderer->GetActiveCamera()->GetPosition(wc1);

        float distance = sqrt((wc1[0] - fp[0]) * (wc1[0] - fp[0]) + (wc1[1] - fp[1]) * (wc1[1] - fp[1]) + (wc1[2] - fp[2]) * (wc1[2] - fp[2]));

        renderer->GetActiveCamera()->SetPosition(fp[0], fp[1], fp[2] + distance);
        renderer->GetActiveCamera()->SetFocalPoint(actorSTL->GetCenter());*/
        ui->qvtkWidget->GetRenderWindow()->Render();
    }
}

void MeshViewer::showAxesConner()
{
    ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
    displayAxesConner(widgetAxesConner, ui->qvtkWidget->GetRenderWindow()->GetInteractor());
    renderer->ResetCamera();
    double wc1[3];
    double fp[3];
    renderer->GetActiveCamera()->GetFocalPoint(fp);
    renderer->GetActiveCamera()->GetPosition(wc1);
    float distance = sqrt((wc1[0] - fp[0]) * (wc1[0] - fp[0]) + (wc1[1] - fp[1]) * (wc1[1] - fp[1]) + (wc1[2] - fp[2]) * (wc1[2] - fp[2]));
    renderer->GetActiveCamera()->SetPosition(fp[0], fp[1], fp[2] + distance);
    renderer->GetActiveCamera()->SetFocalPoint(actorSTL->GetCenter());
    //set style
    setStyleInteractionDefault();
    //end set
    ui->qvtkWidget->render(this);
}

void MeshViewer::displayAxesConner(vtkSmartPointer<vtkOrientationMarkerWidget> &widget, vtkRenderWindowInteractor *renderWindowInteractor)
{
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);
    axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 1, 0);
    axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 1);
    axes->SetShaftTypeToCylinder();
    axes->SetCylinderRadius(0.05);
    axes->PickableOff();
    widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
    widget->SetOrientationMarker(axes);
    widget->SetInteractor(renderWindowInteractor);
    widget->SetViewport(0.0, 0.0, 0.2, 0.2);
    widget->EnabledOn();
    widget->SetEnabled(1);
    widget->InteractiveOff();
}

void MeshViewer::setButtonEnable(bool value)
{
    ui->actionSave->setEnabled(value);
    ui->actionSaveAs->setEnabled(value);
    ui->actionClose->setEnabled(value);
    ui->actionAddShader->setEnabled(value);
    ui->actionAddLight->setEnabled(value);
    ui->actionAddMaterial->setEnabled(value);
    ui->actionColor->setEnabled(value);
    ui->actionShowZones->setEnabled(value);
    ui->actionDelaunay3D->setEnabled(value);
    ui->actionSelectObject->setEnabled(value);
    ui->actionSelectCell->setEnabled(value);
    ui->actionSelectCellNeighbors->setEnabled(value);
    ui->actionSelectTriangle->setEnabled(value);
    ui->actionSelectPoint->setEnabled(value);
    ui->actionSelectPointNeighbors->setEnabled(value);
    ui->actionSolid->setEnabled(value);
    ui->actionWireFrame->setEnabled(value);
    ui->actionOutline->setEnabled(value);
    ui->actionEdges->setEnabled(value);
    ui->actionPolygon->setEnabled(value);
    ui->actionPoints->setEnabled(value);
    ui->actionResetCenter->setEnabled(value);
    ui->actionShowAxesCenter->setEnabled(value);
    ui->actionDeleteCell->setEnabled(value);
}

void MeshViewer::setExtrasActionEnable(bool value)
{
    ui->actionAddCube->setEnabled(value);
    ui->actionAddSphere->setEnabled(value);
    ui->actionAddIcosahedron->setEnabled(value);
    ui->actionAddTorus->setEnabled(value);
    ui->actionSelectLine->setEnabled(value);
}

void MeshViewer::createToolButtonMenu()
{
    QMenu *menuAddObject = new QMenu(this);
    toolAddObject = new QToolButton(this);
    menuAddObject->addAction(ui->actionAddCube);
    menuAddObject->addAction(ui->actionAddSphere);
    menuAddObject->addAction(ui->actionAddIcosahedron);
    menuAddObject->addAction(ui->actionAddTorus);
    toolAddObject->setMenu(menuAddObject);
    toolAddObject->setDefaultAction(ui->actionAddCube);
    toolAddObject->setPopupMode(QToolButton::MenuButtonPopup);
    ui->editTool->insertWidget(ui->actionDeleteCell, toolAddObject);
    ui->editTool->insertSeparator(ui->actionDeleteCell);
    QMenu *menuSelect = new QMenu(this);
    toolSelect = new QToolButton(this);
    menuSelect->addAction(ui->actionSelectObject);
    menuSelect->addAction(ui->actionSelectCell);
    menuSelect->addAction(ui->actionSelectCellNeighbors);
    menuSelect->addAction(ui->actionSelectTriangle);
    menuSelect->addAction(ui->actionSelectLine);
    menuSelect->addAction(ui->actionSelectPoint);
    menuSelect->addAction(ui->actionSelectPointNeighbors);
    toolSelect->setMenu(menuSelect);
    toolSelect->setDefaultAction(ui->actionSelectObject);
    toolSelect->setPopupMode(QToolButton::MenuButtonPopup);
    ui->viewTool->insertWidget(ui->actionSolid, toolSelect);
    ui->viewTool->insertSeparator(ui->actionSolid);
}

void MeshViewer::setCheckActionDefault()
{
    ui->actionSolid->setChecked(true);
    ui->actionWireFrame->setChecked(false);
    ui->actionOutline->setChecked(false);
    ui->actionEdges->setChecked(false);
    ui->actionPolygon->setChecked(false);
    ui->actionPoints->setChecked(false);
    ui->actionAddShader->setChecked(false);
    ui->actionAddLight->setChecked(false);
    ui->actionAddMaterial->setChecked(false);
    ui->actionShowAxesCenter->setChecked(false);
    ui->actionShowZones->setChecked(false);
    ui->actionDelaunay3D->setChecked(false);
    ui->actionDeleteCell->setChecked(false);
}

void MeshViewer::setStyleInteractionDefault()
{
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
}

void MeshViewer::slotExit()
{
    if (isChangeInputFile) {
        QString message = tr("Do you want to save your changes before quitting?");
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Warning"),
                                     message,
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Cancel) {
            return;
        } else if (reply == QMessageBox::Yes) {
            if (nameFileInput.endsWith(tr(".stl") , Qt::CaseInsensitive)) {
                STLCore.STLSaving(lstActors, nameFileInput, 0, 0, 1);
            } else if (nameFileInput.endsWith(tr(".ply") , Qt::CaseInsensitive)) {
                STLCore.STLSaving(lstActors, nameFileInput, 1, 0, 1);
            } else if (nameFileInput.endsWith(tr(".vtk") , Qt::CaseInsensitive)) {
                STLCore.STLSaving(lstActors, nameFileInput, 2, 0, 1);
            } else if (nameFileInput.endsWith(tr(".vtp") , Qt::CaseInsensitive)) {
                STLCore.STLSaving(lstActors, nameFileInput, 3, 0, 1);
            }
        }
    }

    emitTerminate();
    lstActors.clear();
    lstLights.clear();
    lstLightActors.clear();
    qApp->exit();
}

void MeshViewer::slotOpen()
{
    nameFileInput = QFileDialog::getOpenFileName(this, tr("Open File"),
                    QString(), tr("Legacy VTK Files (*.vtk);;"
                                  "PLY Polygonal File Format (*.ply);;"
                                  "Stereo Lithography File Format (*.stl);;"
                                  "VTK PolyData Files (*.vtp)"));

    if (!nameFileInput.isEmpty()) {
        if (nameFileInput.endsWith(tr(".stl") , Qt::CaseInsensitive)) {
            strLog = QString("Reading model \"%1\"...").arg(nameFileInput.mid(nameFileInput.lastIndexOf("/") + 1));
            listLogInfo->addItem(strLog);
            STLCore.STLReading(nameFileInput, 0);
            isChangeInputFile = false;
        } else if (nameFileInput.endsWith(tr(".ply") , Qt::CaseInsensitive)) {
            strLog = QString("Reading model \"%1\"...").arg(nameFileInput.mid(nameFileInput.lastIndexOf("/") + 1));
            listLogInfo->addItem(strLog);
            STLCore.STLReading(nameFileInput, 1);
            isChangeInputFile = false;
        } else if (nameFileInput.endsWith(tr(".vtk") , Qt::CaseInsensitive)) {
            strLog = QString("Reading model \"%1\"...").arg(nameFileInput.mid(nameFileInput.lastIndexOf("/") + 1));
            listLogInfo->addItem(strLog);
            STLCore.STLReading(nameFileInput, 2);
            isChangeInputFile = false;
        } else if (nameFileInput.endsWith(tr(".vtp") , Qt::CaseInsensitive)) {
            strLog = QString("Reading model \"%1\"...").arg(nameFileInput.mid(nameFileInput.lastIndexOf("/") + 1));
            listLogInfo->addItem(strLog);
            STLCore.STLReading(nameFileInput, 3);
            isChangeInputFile = false;
        } else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::critical(this, tr("Error"),
                                          tr("File type not supported..."),
                                          QMessageBox::Retry | QMessageBox::Ignore);

            if (reply == QMessageBox::Retry) {
                ui->actionOpen->trigger();
            }
        }
    }
}

void MeshViewer::slotSave()
{
    if (nameFileInput.isEmpty()) {
        ui->actionSaveAs->trigger();
    } else {
        QString message = tr("%1 already exists.\n" \
                             "Do you want to replace it?").arg(nameFileInput.mid(nameFileInput.lastIndexOf("/") + 1));
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Confirm Save File"),
                                     message,
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Yes) {
            if (nameFileInput.endsWith(tr(".stl") , Qt::CaseInsensitive)) {
                strLog = QString("Saving file \"%1\"...").arg(nameFileInput.mid(nameFileInput.lastIndexOf("/") + 1));
                listLogInfo->addItem(strLog);
                STLCore.STLSaving(lstActors, nameFileInput, 0, 0, 1);
                isChangeInputFile = false;
            } else if (nameFileInput.endsWith(tr(".ply") , Qt::CaseInsensitive)) {
                strLog = QString("Saving file \"%1\"...").arg(nameFileInput.mid(nameFileInput.lastIndexOf("/") + 1));
                listLogInfo->addItem(strLog);
                STLCore.STLSaving(lstActors, nameFileInput, 1, 0, 1);
                isChangeInputFile = false;
            } else if (nameFileInput.endsWith(tr(".vtk") , Qt::CaseInsensitive)) {
                strLog = QString("Saving file \"%1\"...").arg(nameFileInput.mid(nameFileInput.lastIndexOf("/") + 1));
                listLogInfo->addItem(strLog);
                STLCore.STLSaving(lstActors, nameFileInput, 2, 0, 1);
                isChangeInputFile = false;
            } else if (nameFileInput.endsWith(tr(".vtp") , Qt::CaseInsensitive)) {
                strLog = QString("Saving file \"%1\"...").arg(nameFileInput.mid(nameFileInput.lastIndexOf("/") + 1));
                listLogInfo->addItem(strLog);
                STLCore.STLSaving(lstActors, nameFileInput, 3, 0, 1);
                isChangeInputFile = false;
            }
        } else if (reply == QMessageBox::No) {
            ui->actionSaveAs->trigger();
        }
    }
}

void MeshViewer::slotSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File As"),
                       QString(), tr("Legacy VTK Files (*.vtk);;"
                                     "PLY Polygonal File Format (*.ply);;"
                                     "Stereo Lithography File Format (*.stl);;"
                                     "VTK PolyData Files (*.vtp)"));

    if (!fileName.isEmpty()) {
        if (!((fileName.endsWith(".stl", Qt::CaseInsensitive)) || (fileName.endsWith(".ply", Qt::CaseInsensitive)) ||
                (fileName.endsWith(".vtk", Qt::CaseInsensitive)) || (fileName.endsWith(".vtp", Qt::CaseInsensitive)))) {
            fileName += ".stl"; // default
        }

        //Choose encode type Anscii or Binary
        int encodeType;
        ConfigureWriter configure;

        if (configure.exec() == QDialog::Accepted) {
            encodeType = configure.fileType();
        }

        if (fileName.endsWith(tr(".stl") , Qt::CaseInsensitive)) {
            strLog = QString("Saving file \"%1\"...").arg(fileName.mid(fileName.lastIndexOf("/") + 1));
            listLogInfo->addItem(strLog);
            STLCore.STLSaving(lstActors, fileName, 0, encodeType, 1);
            nameFileInput = fileName;
            isChangeInputFile = false;
        } else if (fileName.endsWith(tr(".ply") , Qt::CaseInsensitive)) {
            strLog = QString("Saving file \"%1\"...").arg(fileName.mid(fileName.lastIndexOf("/") + 1));
            listLogInfo->addItem(strLog);
            STLCore.STLSaving(lstActors, fileName, 1, encodeType, 1);
            nameFileInput = fileName;
            isChangeInputFile = false;
        } else if (fileName.endsWith(tr(".vtk") , Qt::CaseInsensitive)) {
            strLog = QString("Saving file \"%1\"...").arg(fileName.mid(fileName.lastIndexOf("/") + 1));
            listLogInfo->addItem(strLog);
            STLCore.STLSaving(lstActors, fileName, 2, encodeType, 1);
            nameFileInput = fileName;
            isChangeInputFile = false;
        } else if (fileName.endsWith(tr(".vtp") , Qt::CaseInsensitive)) {
            strLog = QString("Saving file \"%1\"...").arg(fileName.mid(fileName.lastIndexOf("/") + 1));
            listLogInfo->addItem(strLog);
            STLCore.STLSaving(lstActors, fileName, 3, encodeType, 1);
            nameFileInput = fileName;
            isChangeInputFile = false;
        }
    }
}

void MeshViewer::slotAddShadow()
{
    if (ui->actionAddShader->isChecked()) {
        bool supported = vtkFrameBufferObject::IsSupported(ui->qvtkWidget->GetRenderWindow());

        if (!supported) {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::critical(this, tr("Attention"),
                                          tr("Shadow rendering is not supported by the current video driver!"),
                                          QMessageBox::Ok);

            if (reply == QMessageBox::Ok) {
                ui->actionAddShader->setChecked(false);
                return;
            }
        }

        vtkSmartPointer<vtkCameraPass> cameraP = vtkSmartPointer<vtkCameraPass>::New();
        vtkSmartPointer<vtkOpaquePass> opaque = vtkSmartPointer<vtkOpaquePass>::New();
        vtkSmartPointer<vtkDepthPeelingPass> peeling = vtkSmartPointer<vtkDepthPeelingPass>::New();
        peeling->SetMaximumNumberOfPeels(200);
        peeling->SetOcclusionRatio(0.1);
        vtkSmartPointer<vtkTranslucentPass> translucent = vtkSmartPointer<vtkTranslucentPass>::New();
        peeling->SetTranslucentPass(translucent);
        vtkSmartPointer<vtkVolumetricPass> volume = vtkSmartPointer<vtkVolumetricPass>::New();
        vtkSmartPointer<vtkOverlayPass> overlay = vtkSmartPointer<vtkOverlayPass>::New();
        vtkSmartPointer<vtkLightsPass> lights = vtkSmartPointer<vtkLightsPass>::New();
        vtkSmartPointer<vtkSequencePass> opaqueSequence = vtkSmartPointer<vtkSequencePass>::New();
        vtkSmartPointer<vtkRenderPassCollection> passes2 = vtkSmartPointer<vtkRenderPassCollection>::New();
        passes2->AddItem(lights);
        passes2->AddItem(opaque);
        opaqueSequence->SetPasses(passes2);
        vtkSmartPointer<vtkCameraPass> opaqueCameraPass = vtkSmartPointer<vtkCameraPass>::New();
        opaqueCameraPass->SetDelegatePass(opaqueSequence);
        vtkSmartPointer<vtkShadowMapBakerPass> shadowsBaker = vtkSmartPointer<vtkShadowMapBakerPass>::New();
        shadowsBaker->SetOpaquePass(opaqueCameraPass);
        shadowsBaker->SetResolution(1024);
        // To cancel self-shadowing.
        shadowsBaker->SetPolygonOffsetFactor(3.1f);
        shadowsBaker->SetPolygonOffsetUnits(10.0f);
        vtkSmartPointer<vtkShadowMapPass> shadows = vtkSmartPointer<vtkShadowMapPass>::New();
        shadows->SetShadowMapBakerPass(shadowsBaker);
        shadows->SetOpaquePass(opaqueSequence);
        vtkSmartPointer<vtkSequencePass> seq = vtkSmartPointer<vtkSequencePass>::New();
        vtkSmartPointer<vtkRenderPassCollection> passes = vtkSmartPointer<vtkRenderPassCollection>::New();
        passes->AddItem(shadowsBaker);
        passes->AddItem(shadows);
        passes->AddItem(lights);
        passes->AddItem(peeling);
        passes->AddItem(volume);
        passes->AddItem(overlay);
        seq->SetPasses(passes);
        cameraP->SetDelegatePass(seq);
        double bounds[6];
        actorSTL->GetBounds(bounds);
        vtkSmartPointer<vtkPlaneSource> planeSource = vtkSmartPointer<vtkPlaneSource>::New();
        planeSource->SetOrigin(bounds[0] - 5.0, bounds[2] - 2.0, bounds[4] - 5.0);
        planeSource->SetPoint1(bounds[1] + 5.0, bounds[2] - 2.0, bounds[4] - 5.0);
        planeSource->SetPoint2(bounds[0] - 5.0, bounds[2] - 2.0, bounds[5] + 5.0);
        planeSource->SetResolution(200, 200);
        vtkSmartPointer<vtkPolyDataMapper> planeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        planeMapper->SetInputConnection(planeSource->GetOutputPort());
        planeMapper->SetScalarVisibility(0);
        vtkSmartPointer<vtkActor> planeActor = vtkSmartPointer<vtkActor>::New();
        vtkSmartPointer<vtkInformation> planeKeyProperties = vtkSmartPointer<vtkInformation>::New();
        planeKeyProperties->Set(vtkShadowMapBakerPass::OCCLUDER(), 0); // dummy val.
        planeKeyProperties->Set(vtkShadowMapBakerPass::RECEIVER(), 0); // dummy val.
        planeActor->SetPropertyKeys(planeKeyProperties);
        planeActor->SetMapper(planeMapper);
        planeActor->SetVisibility(1);
        planeActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
        lstLightActors.push_back(planeActor);
        //renderer->SetPass(cameraP);
        renderer->AddViewProp(planeActor);
        // Spotlights.
        // lighting left
        vtkSmartPointer<vtkLight> l1 = vtkSmartPointer<vtkLight>::New();
        l1->SetPosition(bounds[0] - 5.0, bounds[3] + 5.0, bounds[4] - 1.0);
        l1->SetFocalPoint(actorSTL->GetPosition());
        l1->SetColor(1.0, 1.0, 1.0);
        l1->SetPositional(1);
        renderer->AddLight(l1);
        l1->SetSwitch(1);
        lstLights.push_back(l1);
        // lighting right
        vtkSmartPointer<vtkLight> l2 = vtkSmartPointer<vtkLight>::New();
        l2->SetPosition(bounds[1] + 5.0, bounds[3] + 5.0, bounds[4] + 1.0);
        l2->SetFocalPoint(actorSTL->GetPosition());
        l2->SetColor(1.0, 0.0, 1.0);
        l2->SetPositional(1);
        renderer->AddLight(l2);
        l2->SetSwitch(1);
        lstLights.push_back(l2);
        //addShaderViewProps(renderer);
        listLogInfo->addItem(tr("Add shader successful !!!"));
    } else {
        for (unsigned int i = 0; i < lstLights.size(); i++) {
            renderer->RemoveLight(lstLights[i]);
        }

        for (unsigned int i = 0; i < lstLightActors.size(); i++) {
            renderer->RemoveViewProp(lstLightActors[i]);
        }

        //ui->qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
        //ui->qvtkWidget->GetRenderWindow()->Render();
        listLogInfo->addItem(tr("Remove shader successful !!!"));
    }

    renderer->ResetCamera();
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void MeshViewer::slotAddLight()
{
    if (lightMain == NULL) {
        if (ui->actionAddLight->isChecked()) {
            // Create a light
            vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
            light->SetLightTypeToCameraLight();
            light->SetPosition(renderer->GetCenter());
            light->SetPositional(true); // required for vtkLightActor below
            light->SetConeAngle(20);
            light->SetFocalPoint(renderer->GetViewPoint());
            light->SetDiffuseColor(1, 0, 0);
            light->SetAmbientColor(0, 1, 0);
            light->SetSpecularColor(0, 0, 1);
            // Display where the light is
            vtkSmartPointer<vtkLightActor> lightActor = vtkSmartPointer<vtkLightActor>::New();
            lightActor->SetLight(light);
            lightMain = light;
            renderer->AddLight(light);
            ui->qvtkWidget->GetRenderWindow()->Render();
            listLogInfo->addItem(tr("Add light successful !!!"));
        }
    } else {
        if (ui->actionAddLight->isChecked()) {
            renderer->AddLight(lightMain);
            ui->qvtkWidget->GetRenderWindow()->Render();
            listLogInfo->addItem(tr("Add light successful !!!"));
        } else {
            renderer->RemoveLight(lightMain);
            ui->qvtkWidget->GetRenderWindow()->Render();
            listLogInfo->addItem(tr("Remove light successful !!!"));
        }
    }
}

void MeshViewer::slotAddMaterial()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select File..."),
                       QString(), tr("JPEG File (*.jpg *jpeg);;PNG File (*.png)"));

    if (!fileName.isEmpty()) {
        // Apply the texture
        vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
        // Read texture file
        vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
        vtkImageReader2 *imageReader = readerFactory->CreateImageReader2(fileName.toStdString().c_str());
        imageReader->SetFileName(fileName.toStdString().c_str());
        texture->SetInputConnection(imageReader->GetOutputPort());
        //The triangulation has texture coordinates generated so we can map a texture onto it.
        vtkSmartPointer<vtkTextureMapToPlane> textureMapper = vtkSmartPointer<vtkTextureMapToPlane>::New();
        textureMapper->SetInputConnection(actorSTL->GetMapper()->GetInputConnection(0, 0));
        // We scale the texture coordinate to get some repeat patterns.
        vtkSmartPointer<vtkTransformTextureCoords> textureTransform = vtkSmartPointer<vtkTransformTextureCoords>::New();
        textureTransform->SetInputConnection(textureMapper->GetOutputPort());

        for (int i = 0; i < lstActors.size(); i++) {
            lstActors[i]->GetMapper()->SetInputConnection(textureTransform->GetOutputPort());
            lstActors[i]->SetTexture(texture);
        }

        actorSTL->GetMapper()->SetInputConnection(textureTransform->GetOutputPort());
        actorSTL->SetTexture(texture);
        ui->qvtkWidget->GetRenderWindow()->Render();
        strLog = QString("Add texture \"%1\" successful !!!").arg(fileName.mid(fileName.lastIndexOf("/") + 1));
        listLogInfo->addItem(strLog);
        imageReader->Delete();
    }
}

void MeshViewer::slotColor()
{
    QColor color = QColorDialog::getColor(Qt::green, this);

    if (color.isValid()) {
        for (int i = 0; i < lstActors.size(); i++) {
            lstActors[i]->GetProperty()->SetColor(double(color.red()) / 255, double(color.green()) / 255, double(color.blue()) / 255);
            //renderer->AddActor(lstActors[i]);
        }

        actorSTL->GetProperty()->SetColor(double(color.red()) / 255, double(color.green()) / 255, double(color.blue()) / 255);
        lastProperty->DeepCopy(actorSTL->GetProperty());
        ui->qvtkWidget->GetRenderWindow()->Render();
        strLog = QString("Color selected (R,G,B): (%1,%2,%3)").arg(color.red()).arg(color.green()).arg(color.blue());
        listLogInfo->addItem(strLog);
    }
}

void MeshViewer::slotShowRegion()
{
    if (ui->actionShowZones->isChecked()) {
        ui->actionDelaunay3D->setChecked(false);
        strLog = QString("The model is extracting region...");
        listLogInfo->addItem(strLog);
        extractThread.ExtractRegion(actorSTL);
        //vtkSmartPointer<KeyInteractorExtractRegions> styleRegions = vtkSmartPointer<KeyInteractorExtractRegions>::New();
        //styleRegions->SetDefaultRenderer(renderer);
        //ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(styleRegions);
    } else {
        //lstRegions.clear();
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();
        renderer->AddActor(actorSTL);
        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
        ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
        ui->qvtkWidget->update();
    }
}

void MeshViewer::slotDelaunay3D()
{
    if (ui->actionDelaunay3D->isChecked()) {
        strLog = QString("Generating color a mesh by heigh...");
        listLogInfo->addItem(strLog);

        if (actorSTL) {
            renderer->RemoveAllViewProps();
            renderer->AddViewProp(axesActor);
            createConnerAnnotation();
            ui->actionShowZones->setChecked(false);
            vtkPolyData *outputPolyData = vtkPolyData::New();
            outputPolyData->DeepCopy(vtkPolyData::SafeDownCast(actorSTL->GetMapper()->GetInput()));
            double bounds[6];
            outputPolyData->GetBounds(bounds);
            // Find min and max z
            double minz = bounds[4];
            double maxz = bounds[5];
            // Create the color map
            vtkSmartPointer<vtkLookupTable> colorLookupTable = vtkSmartPointer<vtkLookupTable>::New();
            colorLookupTable->SetTableRange(minz, maxz);
            colorLookupTable->Build();
            // Generate the colors for each point based on the color map
            vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<vtkUnsignedCharArray>::New();
            colors->SetNumberOfComponents(3);
            colors->SetName("Colors");

            for (int i = 0; i < outputPolyData->GetNumberOfPoints(); i++) {
                double p[3];
                outputPolyData->GetPoint(i, p);
                double dcolor[3];
                colorLookupTable->GetColor(p[2], dcolor);
                unsigned char color[3];

                for (unsigned int j = 0; j < 3; j++) {
                    color[j] = static_cast<unsigned char>(255.0 * dcolor[j]);
                }

                colors->InsertNextTupleValue(color);
            }

            outputPolyData->GetPointData()->SetScalars(colors);
            // Create a mapper and actor
            vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
            mapper->SetInputConnection(outputPolyData->GetProducerPort());
#else
            mapper->SetInputData(outputPolyData);
#endif
            vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
            actor->SetMapper(mapper);
            vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();
            scalarBar->SetLookupTable(colorLookupTable);
            scalarBar->SetMaximumWidthInPixels(50);
            scalarBar->SetTitle("color");
            scalarBar->SetNumberOfLabels(5);
            // Add the actor to the scene
            renderer->AddActor(actor);
            renderer->AddActor2D(scalarBar);
            strLog = QString("Generate color a mesh by heigh finished !!!");
            listLogInfo->addItem(strLog);
            outputPolyData->Delete();
        }
    } else {
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();
        renderer->AddActor(actorSTL);
        strLog = QString("Remove color a mesh by heigh finished !!!");
        listLogInfo->addItem(strLog);
    }

    ui->qvtkWidget->GetRenderWindow()->Render();
}

void MeshViewer::slotClose()
{
    if (isChangeInputFile) {
        QString message = tr("Do you want to save your changes before closed file?");
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Warning"),
                                     message,
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Cancel) {
            return;
        } else if (reply == QMessageBox::Yes) {
            if (nameFileInput.endsWith(tr(".stl") , Qt::CaseInsensitive)) {
                STLCore.STLSaving(lstActors, nameFileInput, 0, 0, 1);
            } else if (nameFileInput.endsWith(tr(".ply") , Qt::CaseInsensitive)) {
                STLCore.STLSaving(lstActors, nameFileInput, 1, 0, 1);
            } else if (nameFileInput.endsWith(tr(".vtk") , Qt::CaseInsensitive)) {
                STLCore.STLSaving(lstActors, nameFileInput, 2, 0, 1);
            } else if (nameFileInput.endsWith(tr(".vtp") , Qt::CaseInsensitive)) {
                STLCore.STLSaving(lstActors, nameFileInput, 3, 0, 1);
            }
        }
    }

    isChangeInputFile = false;
    renderer->RemoveAllViewProps();
    createConnerAnnotation();
    widgetAxesConner->EnabledOff();
    ui->qvtkWidget->GetRenderWindow()->Render();
    ui->actionWireFrame->setChecked(false);
    ui->actionSolid->setChecked(false);
    listLogInfo->clear();
    setButtonEnable(false);
    setExtrasActionEnable(false);
    nameFileInput = "";
    listLogInfo->addItem(tr("Welcome to Mesh Magic 3D !!!"));
    actorSTL = NULL;
    lstActors.clear();
    lstLights.clear();
    lstLightActors.clear();
}

void MeshViewer::slotShowSolid()
{
    //init();
    ui->actionSolid->setChecked(true);
    ui->actionWireFrame->setChecked(false);
    ui->actionOutline->setChecked(false);
    ui->actionEdges->setChecked(false);
    ui->actionPolygon->setChecked(false);
    ui->actionPoints->setChecked(false);
    ui->actionShowZones->setChecked(false);
    ui->actionDelaunay3D->setChecked(false);

    if (actorSTL != NULL) {
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();
        actorSTL->GetProperty()->SetRepresentationToSurface();

        for (int i = 0; i < lstActors.size(); i++) {
            lstActors[i]->GetProperty()->SetRepresentationToSurface();
            renderer->AddActor(lstActors[i]);
        }

        lastProperty->DeepCopy(actorSTL->GetProperty());
        renderer->ResetCamera();
        ui->qvtkWidget->GetRenderWindow()->Render();
    }
}

void MeshViewer::slotShowWireFrame()
{
    //init();
    ui->actionSolid->setChecked(false);
    ui->actionWireFrame->setChecked(true);
    ui->actionOutline->setChecked(false);
    ui->actionEdges->setChecked(false);
    ui->actionPolygon->setChecked(false);
    ui->actionPoints->setChecked(false);
    ui->actionShowZones->setChecked(false);
    ui->actionDelaunay3D->setChecked(false);

    if (actorSTL != NULL) {
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();
        actorSTL->GetProperty()->SetRepresentationToWireframe();

        for (int i = 0; i < lstActors.size(); i++) {
            lstActors[i]->GetProperty()->SetRepresentationToWireframe();
            renderer->AddActor(lstActors[i]);
        }

        lastProperty->DeepCopy(actorSTL->GetProperty());
        renderer->ResetCamera();
        ui->qvtkWidget->GetRenderWindow()->Render();
    }
}

void MeshViewer::slotShowOutline()
{
    ui->actionSolid->setChecked(false);
    ui->actionWireFrame->setChecked(false);
    ui->actionOutline->setChecked(true);
    ui->actionEdges->setChecked(false);
    ui->actionPolygon->setChecked(false);
    ui->actionPoints->setChecked(false);
    ui->actionShowZones->setChecked(false);
    ui->actionDelaunay3D->setChecked(false);

    if (actorSTL != NULL) {
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();

        for (unsigned int i = 0; i < lstActors.size(); i++) {
            // Create the outline
            vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
            input->ShallowCopy(vtkPolyData::SafeDownCast(lstActors[i]->GetMapper()->GetInput()));
            vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
            translation->Identity();
            translation->Translate(lstActors[i]->GetCenter()[0], lstActors[i]->GetCenter()[1], lstActors[i]->GetCenter()[2]);
            vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            transformFilter->SetInputData(input);
            transformFilter->SetTransform(translation);
            transformFilter->Update();
            vtkSmartPointer<vtkOutlineFilter> outline = vtkSmartPointer<vtkOutlineFilter>::New();
#if VTK_MAJOR_VERSION <= 5
            outline->SetInput(transformFilter->GetOutput());
#else
            outline->SetInputData(transformFilter->GetOutput());
#endif
            vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            outlineMapper->SetInputConnection(outline->GetOutputPort());
            vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
            outlineActor->SetMapper(outlineMapper);

            //outlineActor->GetProperty()->SetColor(0,0,0);
            if (outlineActor) {
                lastProperty->DeepCopy(outlineActor->GetProperty());
            }

            renderer->AddActor(outlineActor);
            double bounds[6];
            transformFilter->GetOutput()->GetBounds(bounds);
            strLog = QString("Show model %1 as outline:\n").arg(i + 1);
            strLog += QString("    Bounds:\n        Xmin,Xmax: (%1, %2)\n        Ymin,Ymax: (%3, %4)\n        Zmin,Zmax: (%5, %6)")
                      .arg(bounds[0]).arg(bounds[1]).arg(bounds[2]).arg(bounds[3]).arg(bounds[4]).arg(bounds[5]);
            listLogInfo->addItem(strLog);
        }

        renderer->ResetCamera();
        ui->qvtkWidget->GetRenderWindow()->Render();
    }
}

void MeshViewer::slotShowEdges()
{
    ui->actionSolid->setChecked(false);
    ui->actionWireFrame->setChecked(false);
    ui->actionOutline->setChecked(false);
    ui->actionEdges->setChecked(true);
    ui->actionPolygon->setChecked(false);
    ui->actionPoints->setChecked(false);
    ui->actionShowZones->setChecked(false);
    ui->actionDelaunay3D->setChecked(false);

    if (actorSTL != NULL) {
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();

        for (unsigned int i = 0; i < lstActors.size(); i++) {
            vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
            input->ShallowCopy(vtkPolyData::SafeDownCast(lstActors[i]->GetMapper()->GetInput()));
            vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
            translation->Identity();
            translation->Translate(lstActors[i]->GetCenter()[0], lstActors[i]->GetCenter()[1], lstActors[i]->GetCenter()[2]);
            vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            transformFilter->SetInputData(input);
            transformFilter->SetTransform(translation);
            transformFilter->Update();
            vtkSmartPointer<vtkExtractEdges> extractEdges =  vtkSmartPointer<vtkExtractEdges>::New();
            //extractEdges->SetInputConnection(lstActors[i]->GetMapper()->GetInputConnection(0,0)->GetProducer()->GetOutputPort());
            extractEdges->SetInputConnection(transformFilter->GetOutputPort());
            extractEdges->Update();
            vtkCellArray *lines = extractEdges->GetOutput()->GetLines();
            vtkPoints *points = extractEdges->GetOutput()->GetPoints();
            strLog = QString("Show model %1 as edges:\n").arg(i + 1);
            strLog += QString("    There are %1 cells.\n").arg(lines->GetNumberOfCells());
            strLog += QString("    There are %1 points.").arg(points->GetNumberOfPoints());
            listLogInfo->addItem(strLog);
            // Visualize the edges and Create a mapper and actor
            vtkSmartPointer<vtkPolyDataMapper> edgeMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
            edgeMapper->SetInputConnection(extractEdges->GetOutputPort());
            vtkSmartPointer<vtkActor> edgeActor = vtkSmartPointer<vtkActor>::New();
            edgeActor->SetMapper(edgeMapper);

            //edgeActor->GetProperty()->SetColor(0,0,0);
            if (edgeActor) {
                lastProperty->DeepCopy(edgeActor->GetProperty());
            }

            renderer->AddActor(edgeActor);
        }

        renderer->ResetCamera();
        ui->qvtkWidget->GetRenderWindow()->Render();
    }
}

void MeshViewer::slotShowPolygon()
{
    ui->actionSolid->setChecked(false);
    ui->actionWireFrame->setChecked(false);
    ui->actionOutline->setChecked(false);
    ui->actionEdges->setChecked(false);
    ui->actionPolygon->setChecked(true);
    ui->actionPoints->setChecked(false);
    ui->actionShowZones->setChecked(false);
    ui->actionDelaunay3D->setChecked(false);

    if (actorSTL != NULL) {
        renderer->RemoveAllViewProps();
        //showAxesCenter();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();

        for (unsigned int i = 0; i < lstActors.size(); i++) {
            vtkSmartPointer<vtkPolyData> input = vtkSmartPointer<vtkPolyData>::New();
            input->ShallowCopy(vtkPolyData::SafeDownCast(lstActors[i]->GetMapper()->GetInput()));
            vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
            translation->Identity();
            translation->Translate(lstActors[i]->GetCenter()[0], lstActors[i]->GetCenter()[1], lstActors[i]->GetCenter()[2]);
            vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            transformFilter->SetInputData(input);
            transformFilter->SetTransform(translation);
            transformFilter->Update();
            double bounds[6];
            transformFilter->GetOutput()->GetBounds(bounds);
            strLog = QString("Show model %1 as polygon:\n").arg(i + 1);
            strLog += QString("    Bounds:\n        Xmin,Xmax: (%1, %2)\n        Ymin,Ymax: (%3, %4)\n        Zmin,Zmax: (%5, %6)")
                      .arg(bounds[0]).arg(bounds[1]).arg(bounds[2]).arg(bounds[3]).arg(bounds[4]).arg(bounds[5]);
            listLogInfo->addItem(strLog);
            vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
            plane->SetOrigin((bounds[1] + bounds[0]) / 2.0, (bounds[3] + bounds[2]) / 2.0, bounds[4]);
            plane->SetNormal(0, 0, 1);
            // Create cutter
            double high = plane->EvaluateFunction((bounds[1] + bounds[0]) / 2.0, (bounds[3] + bounds[2]) / 2.0, bounds[5]);
            vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
            //cutter->SetInputConnection(actorSTL->GetMapper()->GetInputConnection(0,0));
            //cutter->SetInputConnection(lstActors[i]->GetMapper()->GetInputConnection(0,0)->GetProducer()->GetOutputPort());
            cutter->SetInputConnection(transformFilter->GetOutputPort());
            cutter->SetCutFunction(plane);
            cutter->GenerateValues(30, -.99, .99 * high);
            vtkSmartPointer<vtkPolyDataMapper> cutterMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            cutterMapper->SetInputConnection(cutter->GetOutputPort());
            cutterMapper->ScalarVisibilityOff();
            // Create cut actor
            vtkSmartPointer<vtkActor> cutterActor = vtkSmartPointer<vtkActor>::New();
            cutterActor->SetMapper(cutterMapper);

            if (cutterActor) {
                lastProperty->DeepCopy(cutterActor->GetProperty());
            }

            // Add the actors to the renderer
            renderer->AddActor(cutterActor);
        }

        renderer->ResetCamera();
        ui->qvtkWidget->GetRenderWindow()->Render();
    }
}

void MeshViewer::slotShowPoints()
{
    ui->actionSolid->setChecked(false);
    ui->actionWireFrame->setChecked(false);
    ui->actionOutline->setChecked(false);
    ui->actionEdges->setChecked(false);
    ui->actionPolygon->setChecked(false);
    ui->actionPoints->setChecked(true);
    ui->actionShowZones->setChecked(false);
    ui->actionDelaunay3D->setChecked(false);

    if (actorSTL != NULL) {
        renderer->RemoveAllViewProps();
        createConnerAnnotation();
        renderer->AddViewProp(axesActor);
        actorSTL->GetProperty()->SetRepresentationToPoints();

        for (unsigned int i = 0; i < lstActors.size(); i++) {
            lstActors[i]->GetProperty()->SetRepresentationToPoints();
            renderer->AddActor(lstActors[i]);
        }

        lastProperty->DeepCopy(actorSTL->GetProperty());
        renderer->ResetCamera();
        ui->qvtkWidget->GetRenderWindow()->Render();
    }
}

void MeshViewer::slotSelectObject()
{
    toolSelect->setDefaultAction(ui->actionSelectObject);
    renderer->RemoveAllViewProps();
    renderer->AddViewProp(axesActor);
    createConnerAnnotation();

    for (unsigned int i = 0; i < lstActors.size(); i++) {
        lstActors[i]->GetProperty()->DeepCopy(lastProperty);
        renderer->AddActor(lstActors[i]);
    }

    ui->qvtkWidget->GetRenderWindow()->Render();
    // Set the custom type to use for interaction.
    vtkSmartPointer<MouseInteractorHighLightActor> styleSelectObject = vtkSmartPointer<MouseInteractorHighLightActor>::New();
    styleSelectObject->SetDefaultRenderer(renderer);
    styleSelectObject->setWidget(ui->qvtkWidget);
    styleSelectObject->GlobalWarningDisplayOff();
    ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(styleSelectObject);
    isChangeInputFile = true;
}

void MeshViewer::slotSelectCell()
{
    toolSelect->setDefaultAction(ui->actionSelectCell);

    if (actorSTL != NULL) {
        vtkSmartPointer<vtkAreaPicker> areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
        ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetPicker(areaPicker);
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();

        for (unsigned int i = 0; i < lstActors.size(); i++) {
            lstActors[i]->GetProperty()->DeepCopy(lastProperty);
            renderer->AddActor(lstActors[i]);
        }

        ui->qvtkWidget->GetRenderWindow()->Render();
        vtkSmartPointer<MouseInteractorHighLightCell> styleCell = vtkSmartPointer<MouseInteractorHighLightCell>::New();
        styleCell->SetDefaultRenderer(renderer);
        styleCell->setAlgorithmOutput(actorSTL->GetMapper()->GetInputConnection(0, 0));
        styleCell->setPolyData(vtkPolyData::SafeDownCast(actorSTL->GetMapper()->GetInput()));
        styleCell->setWidget(ui->qvtkWidget);
        ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(styleCell);
        ui->qvtkWidget->GetRenderWindow()->Render();
        isChangeInputFile = true;
    }
}

void MeshViewer::slotSelectCellNeighbors()
{
    toolSelect->setDefaultAction(ui->actionSelectCellNeighbors);

    if (actorSTL != NULL) {
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();

        for (unsigned int i = 0; i < lstActors.size(); i++) {
            lstActors[i]->GetProperty()->DeepCopy(lastProperty);
            renderer->AddActor(lstActors[i]);
        }

        ui->qvtkWidget->GetRenderWindow()->Render();
        vtkSmartPointer<MouseInteractorHighLightCellNeighbors> styleCellNeighbors = vtkSmartPointer<MouseInteractorHighLightCellNeighbors>::New();
        styleCellNeighbors->SetDefaultRenderer(renderer);
        styleCellNeighbors->setAlgorithmOutput(actorSTL->GetMapper()->GetInputConnection(0, 0));
        styleCellNeighbors->setPolyData(vtkPolyData::SafeDownCast(actorSTL->GetMapper()->GetInput()));
        styleCellNeighbors->setWidget(ui->qvtkWidget);
        ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(styleCellNeighbors);
        isChangeInputFile = true;
    }
}

void MeshViewer::slotSelectTriangle()
{
    toolSelect->setDefaultAction(ui->actionSelectTriangle);

    if (actorSTL != NULL) {
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();

        for (unsigned int i = 0; i < lstActors.size(); i++) {
            lstActors[i]->GetProperty()->DeepCopy(lastProperty);
            renderer->AddActor(lstActors[i]);
        }

        ui->qvtkWidget->GetRenderWindow()->Render();
        vtkSmartPointer<MouseInteractorHighLightTriangle> styleTriangle = vtkSmartPointer<MouseInteractorHighLightTriangle>::New();
        styleTriangle->SetDefaultRenderer(renderer);
        styleTriangle->setPolyData(vtkPolyData::SafeDownCast(actorSTL->GetMapper()->GetInput()));
        styleTriangle->setAlgorithmOutput(actorSTL->GetMapper()->GetInputConnection(0, 0));
        styleTriangle->setWidget(ui->qvtkWidget);
        styleTriangle->GlobalWarningDisplayOff();
        ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(styleTriangle);
        isChangeInputFile = true;
    }
}

void MeshViewer::slotSelectLine()
{
    toolSelect->setDefaultAction(ui->actionSelectLine);

    if (actorSTL != NULL) {
        vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
        triangleFilter->SetInputConnection(actorSTL->GetMapper()->GetInputConnection(0, 0));
        triangleFilter->Update();
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();

        for (unsigned int i = 0; i < lstActors.size(); i++) {
            lstActors[i]->GetProperty()->DeepCopy(lastProperty);
            renderer->AddActor(lstActors[i]);
        }

        ui->qvtkWidget->GetRenderWindow()->Render();
        vtkSmartPointer<MouseInteractorHighLightLine> styleLine = vtkSmartPointer<MouseInteractorHighLightLine>::New();
        styleLine->SetDefaultRenderer(renderer);
        styleLine->setPolyData(vtkPolyData::SafeDownCast(actorSTL->GetMapper()->GetInput()));
        styleLine->setAlgorithmOutput(actorSTL->GetMapper()->GetInputConnection(0, 0));
        styleLine->setWidget(ui->qvtkWidget);
        styleLine->GlobalWarningDisplayOff();
        ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(styleLine);
    }
}

void MeshViewer::slotSelectPoint()
{
    toolSelect->setDefaultAction(ui->actionSelectPoint);

    if (actorSTL != NULL) {
        vtkSmartPointer<vtkIdFilter> idFilter = vtkSmartPointer<vtkIdFilter>::New();
        idFilter->SetInputConnection(actorSTL->GetMapper()->GetInputConnection(0, 0));
        idFilter->SetIdsArrayName("OriginalIds");
        idFilter->Update();
        vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
        surfaceFilter->SetInputConnection(idFilter->GetOutputPort());
        surfaceFilter->Update();
        vtkPolyData *input = surfaceFilter->GetOutput();
        vtkSmartPointer<vtkAreaPicker> areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
        ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetPicker(areaPicker);
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();

        for (unsigned int i = 0; i < lstActors.size(); i++) {
            lstActors[i]->GetProperty()->DeepCopy(lastProperty);
            renderer->AddActor(lstActors[i]);
        }

        ui->qvtkWidget->GetRenderWindow()->Render();
        vtkSmartPointer<MouseInteractorHighLightPoint> stylePoint = vtkSmartPointer<MouseInteractorHighLightPoint>::New();
        stylePoint->SetDefaultRenderer(renderer);
        stylePoint->SetPoints(input);
        ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(stylePoint);
    }
}

void MeshViewer::slotSelectPointNeighbors()
{
    toolSelect->setDefaultAction(ui->actionSelectPointNeighbors);

    if (actorSTL != NULL) {
        renderer->RemoveAllViewProps();
        renderer->AddViewProp(axesActor);
        createConnerAnnotation();

        for (unsigned int i = 0; i < lstActors.size(); i++) {
            lstActors[i]->GetProperty()->DeepCopy(lastProperty);
            renderer->AddActor(lstActors[i]);
        }

        ui->qvtkWidget->GetRenderWindow()->Render();
        vtkSmartPointer<MouseInteractorHighLightPointNeighbors> stylePointNeighbors = vtkSmartPointer<MouseInteractorHighLightPointNeighbors>::New();
        stylePointNeighbors->SetDefaultRenderer(renderer);
        stylePointNeighbors->setAlgorithmOutput(actorSTL->GetMapper()->GetInputConnection(0, 0));
        ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(stylePointNeighbors);
    }
}

void MeshViewer::slotAddCube()
{
    toolAddObject->setDefaultAction(ui->actionAddCube);
    vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();
    cubeSource->Update();
    // Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(cubeSource->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->DeepCopy(lastProperty);
    actor->SetTexture(actorSTL->GetTexture());
    renderer->AddActor(actor);
    renderer->ResetCamera();
    lstActors.push_back(actor);
    isChangeInputFile = true;
    //vtkSmartPointer<vtkInteractorStyleTrackballActor> style = vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void MeshViewer::slotAddSphere()
{
    toolAddObject->setDefaultAction(ui->actionAddSphere);
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->Update();
    // Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->DeepCopy(lastProperty);
    actor->SetTexture(actorSTL->GetTexture());
    renderer->AddActor(actor);
    renderer->ResetCamera();
    lstActors.push_back(actor);
    isChangeInputFile = true;
    //vtkSmartPointer<vtkInteractorStyleTrackballActor> style = vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void MeshViewer::slotAddIcosahedron()
{
    toolAddObject->setDefaultAction(ui->actionAddIcosahedron);
    vtkSmartPointer<vtkPlatonicSolidSource> platonicSolidSource = vtkSmartPointer<vtkPlatonicSolidSource>::New();
    //platonicSolidSource->SetSolidTypeToTetrahedron(); //4 faces
    //platonicSolidSource->SetSolidTypeToOctahedron(); //8 faces
    //platonicSolidSource->SetSolidTypeToDodecahedron(); //12 faces
    platonicSolidSource->SetSolidTypeToIcosahedron(); //20 faces
    platonicSolidSource->Update();
    // Each face has a different cell scalar
    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(20);
    lut->SetTableRange(0.0, 19.0);
    lut->Build();

    for (vtkIdType i = 0; i < 20; i++) {
        lut->SetTableValue(i, actorSTL->GetProperty()->GetColor());
    }

    //Create a mapper and actor.
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(platonicSolidSource->GetOutputPort());
    mapper->SetLookupTable(lut);
    mapper->SetScalarRange(0, 19);
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    //The below codes not useful, maybe we must rebuild lookup table for this object.
    //actor->GetProperty()->DeepCopy(lastProperty);
    //actor->GetProperty()->SetColor(actorSTL->GetProperty()->GetColor());
    actor->SetTexture(actorSTL->GetTexture());
    renderer->AddActor(actor);
    renderer->ResetCamera();
    lstActors.push_back(actor);
    isChangeInputFile = true;
    //showAxesConner();
    //vtkSmartPointer<vtkInteractorStyleTrackballActor> style = vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void MeshViewer::slotAddTorus()
{
    toolAddObject->setDefaultAction(ui->actionAddTorus);
    vtkSmartPointer<vtkParametricTorus> function = vtkSmartPointer<vtkParametricTorus>::New();
    vtkSmartPointer<vtkParametricFunctionSource> source = vtkSmartPointer<vtkParametricFunctionSource>::New();
    source->SetParametricFunction(function);
    source->Update();
    // Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(source->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->DeepCopy(lastProperty);
    //actor->GetProperty()->SetColor(actorSTL->GetProperty()->GetColor());
    actor->SetTexture(actorSTL->GetTexture());
    renderer->AddActor(actor);
    lstActors.push_back(actor);
    renderer->ResetCamera();
    isChangeInputFile = true;
    //showAxesConner();
    //vtkSmartPointer<vtkInteractorStyleTrackballActor> style = vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void MeshViewer::slotResetCamera()
{
    renderer->ResetCamera();
    //renderer->GetActiveCamera()->SetFocalPoint(actorSTL->GetCenter());
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void MeshViewer::slotShowAxesCenter()
{
    if (ui->actionShowAxesCenter->isChecked()) {
        axesActor->VisibilityOn();
    } else {
        axesActor->VisibilityOff();
    }

    ui->qvtkWidget->update();
}

void MeshViewer::slotAbout()
{
    About *about = new About();
    about->exec();
}

void MeshViewer::slotFullscreen()
{
    if (!isFullScreen()) {
        //ui->qvtkWidget->GetRenderWindow()->SetFullScreen(true);
        QWidget::showFullScreen();
    } else {
        //ui->qvtkWidget->GetRenderWindow()->SetFullScreen(false);
        QWidget::showMaximized();
    }
}

void MeshViewer::slotHelp()
{
    QString path(QApplication::applicationDirPath());
    path.append("/data/MeshMagic.pdf");
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void MeshViewer::listenOcculusRotateX(double angle)
{
    //thread.abort();
    _threadRotate.rotate(renderer->GetActiveCamera(), angle, 0);
    //thread.Rotate(lstActors,angle,0);
    renderer->ResetCameraClippingRange();
}

void MeshViewer::listenOcculusRotateY(double angle)
{
    //thread.abort();
    _threadRotate.rotate(renderer->GetActiveCamera(), angle, 1);
    //thread.Rotate(lstActors,angle,1);
    renderer->ResetCameraClippingRange();
}

void MeshViewer::listenOcculusRotateZ(double angle)
{
    //thread.abort();
    _threadRotate.rotate(renderer->GetActiveCamera(), angle, 2);
    //thread.Rotate(lstActors,angle,2);
    renderer->ResetCameraClippingRange();
}

void MeshViewer::listenOcculusRotateXYZ(double angle)
{
    //thread.abort();
    _threadRotate.rotate(renderer->GetActiveCamera(), angle);
    //thread.Rotate(lstActors,angle);
    renderer->ResetCameraClippingRange();
}

// For each spotlight, add a light frustum wireframe representation
// and a cone wireframe representation, colored with the light color.
void MeshViewer::addShaderViewProps(vtkRenderer *ren)
{
    assert("pre: r_exists" && ren != 0);
    vtkLightCollection *lights = ren->GetLights();
    lights->InitTraversal();
    vtkLight *light = lights->GetNextItem();

    while (light != 0) {
        double angle = light->GetConeAngle();

        if (light->GetPositional() && angle < 180.0) {
            vtkLightActor *lightActor = vtkLightActor::New();
            lightActor->SetLight(light);
            lstLightActors.push_back(lightActor);
            ren->AddViewProp(lightActor);
            lightActor->Delete();
        }

        light = lights->GetNextItem();
    }
}

MeshViewer::~MeshViewer()
{
    delete ui;
    lastProperty->Delete();
}

void MeshViewer::slotDeleteCell()
{
    if (ui->actionDeleteCell->isChecked()) {
        ui->qvtkWidget->GetRenderWindow()->Render();
        // Set the custom type to use for interaction.
        vtkSmartPointer<InteractorStyleDeleteCell> styleDeleteCell = vtkSmartPointer<InteractorStyleDeleteCell>::New();
        styleDeleteCell->SetDefaultRenderer(renderer);
        styleDeleteCell->setPolyData(vtkPolyData::SafeDownCast(actorSTL->GetMapper()->GetInput()));
        styleDeleteCell->setActionCaller(ui->actionDeleteCell);
        styleDeleteCell->GlobalWarningDisplayOff();
        ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(styleDeleteCell);
        isChangeInputFile = true;
    }
}

void MeshViewer::slotResetView()
{
    vtkCamera *cam = vtkCamera::New();
    cam->Azimuth(30);
    cam->Roll(45);
    cam->SetViewUp(0, 1, 0);
    cam->OrthogonalizeViewUp();
    renderer->SetActiveCamera(cam);
    renderer->ResetCamera();
    ui->qvtkWidget->update();
    cam->Delete();
}

void MeshViewer::slotAnaglyph()
{
    isAnaglyph = ui->actionAnaglyph->isChecked();

    if (isAnaglyph) {
        //ui->qvtkWidget->GetRenderWindow()->StereoCapableWindowOn();
        ui->qvtkWidget->GetRenderWindow()->SetStereoTypeToAnaglyph();
        ui->qvtkWidget->GetRenderWindow()->StereoRenderOn();
        ui->qvtkWidget->GetRenderWindow()->SetAnaglyphColorSaturation(1);
        ui->qvtkWidget->GetRenderWindow()->StereoUpdate();
    } else {
        //ui->qvtkWidget->GetRenderWindow()->StereoCapableWindowOff();
        ui->qvtkWidget->GetRenderWindow()->StereoRenderOff();
        ui->qvtkWidget->GetRenderWindow()->StereoUpdate();
    }

    ui->qvtkWidget->GetRenderWindow()->GlobalWarningDisplayOff();
    ui->qvtkWidget->GetRenderWindow()->Render();
    setButtonEnable(!isAnaglyph);
    setExtrasActionEnable(false);
}

void MeshViewer::slotScreenshot()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Screenshot"),
                       QString("MeshMagic3D %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH_mm_ss")),
                       tr("JPEG File (*.jpg *jpeg);;PNG File (*.png)"));

    if (!fileName.isEmpty()) {
        if (!((fileName.endsWith(".jpg", Qt::CaseInsensitive)) || (fileName.endsWith(".jpeg", Qt::CaseInsensitive)) ||
                (fileName.endsWith(".png", Qt::CaseInsensitive)))) {
            fileName += ".png"; // default
        }

        // Screenshot
        vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
        windowToImageFilter->SetInput(ui->qvtkWidget->GetRenderWindow());
        //windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
        //windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
        windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
        windowToImageFilter->Update();
        vtkSmartPointer<vtkImageWriter> writer;

        if ((fileName.endsWith(tr(".jpg"), Qt::CaseInsensitive)) || (fileName.endsWith(tr(".jpeg"), Qt::CaseInsensitive))) {
            writer = vtkSmartPointer<vtkJPEGWriter>::New();
        } else {
            writer = vtkSmartPointer<vtkPNGWriter>::New();
        }

        writer->SetFileName(fileName.toStdString().c_str());
        writer->SetInputConnection(windowToImageFilter->GetOutputPort());
        writer->Write();
    }
}

void MeshViewer::slotRotateCamera(vtkCamera *cam)
{
    renderer->SetActiveCamera(cam);
}

void MeshViewer::slotReadSTLDone(vtkActor *actor)
{
    renderer->RemoveAllViewProps();
    setCheckActionDefault();
    createConnerAnnotation();
    lstActors.clear();
    lstLights.clear();
    lstLightActors.clear();
    actorSTL = actor;
    actorSTL->GetProperty()->SetColor(1, 1, 1);
    actorSTL->GetProperty()->SetRepresentationToSurface();
    actorSTL->SetTexture(NULL);
    lstActors.push_back(actorSTL);

    if (actorSTL) {
        lastProperty->DeepCopy(actorSTL->GetProperty());
    }

    showAxesCenter();
    showAxesConner();
    axesActor->VisibilityOff();
    renderer->AddActor(actorSTL);
    renderer->ResetCamera();
    renderer->GetActiveCamera()->SetFocalPoint(actorSTL->GetCenter());

    if (isAnaglyph) {
        ui->qvtkWidget->GetRenderWindow()->StereoCapableWindowOn();
        ui->qvtkWidget->GetRenderWindow()->StereoRenderOn();
        ui->qvtkWidget->GetRenderWindow()->SetStereoTypeToAnaglyph();
        ui->qvtkWidget->GetRenderWindow()->SetAnaglyphColorSaturation(1);
        ui->qvtkWidget->GetRenderWindow()->StereoUpdate();
        setButtonEnable(false);
        setExtrasActionEnable(false);
    } else {
        setButtonEnable(true);
        setExtrasActionEnable(false);
    }

    ui->qvtkWidget->GetRenderWindow()->Render();
    listLogInfo->addItem(tr("Read finished..."));
    numOfPoints = vtkPolyData::SafeDownCast(actorSTL->GetMapper()->GetInput())->GetNumberOfPoints();
    numOfCells = vtkPolyData::SafeDownCast(actorSTL->GetMapper()->GetInput())->GetNumberOfCells();
    strLog = QString("The model has %1 points.").arg(numOfPoints);
    listLogInfo->addItem(strLog);
    strLog = QString("The model has %1 cells.").arg(numOfCells);
    listLogInfo->addItem(strLog);
    ui->actionAnaglyph->setEnabled(true);
}

void MeshViewer::slotWriteSTLDone()
{
    strLog = QString("Save successful !!!");
    listLogInfo->addItem(strLog);
}

void MeshViewer::slotExtractDone(vtkActor *actor, unsigned int numRegions)
{
    renderer->RemoveAllViewProps();
    renderer->AddViewProp(axesActor);
    createConnerAnnotation();

    if (numRegions == 1) {
        strLog = QString("The model has 1 region.");
        listLogInfo->addItem(strLog);
        renderer->AddActor(actor);
    } else {
        strLog = QString("The model has %1 regions.").arg(numRegions);
        listLogInfo->addItem(strLog);
        renderer->AddActor(actor);
    }
}

void MeshViewer::reallyDeletePoint(vtkSmartPointer<vtkPoints> points, vtkIdType id)
{
    vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();

    for (vtkIdType i = 0; i < points->GetNumberOfPoints(); i++) {
        if (i != id) {
            double p[3];
            points->GetPoint(i, p);
            newPoints->InsertNextPoint(p);
        }
    }

    points->ShallowCopy(newPoints);
}
