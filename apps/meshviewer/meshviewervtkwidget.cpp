#include "meshviewervtkwidget.h"
#include "ui_meshviewervtkwidget.h"

#include <QObject>
#include <QDateTime>
#include <QFileDialog>

#include <vtkMath.h>
#include <vtkCamera.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>
#include <vtkPNGWriter.h>
#include <vtkLightActor.h>
#include <vtkJPEGWriter.h>
#include <vtkImageWriter.h>
#include <vtkTextProperty.h>
#include <vtkOutlineFilter.h>
#include <vtkCaptionActor2D.h>
#include <vtkDistanceWidget.h>
#include <vtkPickingManager.h>
#include <vtkPolyDataMapper.h>
#include <vtkWindowToImageFilter.h>
#include <vtkDistanceRepresentation.h>
#include <vtkTransformPolyDataFilter.h>

#include "version.h"

using namespace MeshViewer3D;

MeshViewerVTKWidget::MeshViewerVTKWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeshViewerVTKWidget)
{
    ui->setupUi(this);

    initRender();
}

MeshViewerVTKWidget::~MeshViewerVTKWidget()
{
    delete ui;
}

void MeshViewerVTKWidget::initRender()
{
    _renderer = vtkSmartPointer<vtkRenderer>::New();
    _renderer->GradientBackgroundOn();
    _renderer->SetBackground(1.0, 1.0, 1.0);
    _renderer->SetBackground2(0.2745, 0.5529, 1.0);

    ui->meshviewerRenderer->GetRenderWindow()->AddRenderer(_renderer);

    //corner text
    //Annotate the image with window/level and mouse over pixel information
    _cornerAnnotation = vtkSmartPointer<vtkCornerAnnotation>::New();
    _cornerAnnotation->SetLinearFontScaleFactor(2);
    _cornerAnnotation->SetNonlinearFontScaleFactor(1);
    _cornerAnnotation->SetMaximumFontSize(10);

    _cornerAnnotation->SetText(3, tr("Version").toUtf8() + " " + QString(APP_VERSION_SHORT).toUtf8());
    _cornerAnnotation->SetText(2, tr("DMesh").toUtf8());

    _cornerAnnotation->GetTextProperty()->SetColor(1, 1, 1);
    _renderer->AddViewProp(_cornerAnnotation);

    //set style
    _styleCamera = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    _styleCamera->SetDefaultRenderer(_renderer);
    _styleCamera->GlobalWarningDisplayOff();

    // Create a light
    _lightMain = vtkSmartPointer<vtkLight>::New();
    _lightMain->SetLightTypeToCameraLight();
    _lightMain->SetPosition(this->_renderer->GetCenter());
    _lightMain->SetPositional(true); // required for vtkLightActor below
    _lightMain->SetConeAngle(20);
    _lightMain->SetFocalPoint(this->_renderer->GetViewPoint());
    _lightMain->SetDiffuseColor(0.6705882352941176, 0.596078431372549, 0.8509803921568627);
    _lightMain->SetAmbientColor(0, 1, 0);
    _lightMain->SetSpecularColor(0, 0, 1);

    //draw axes on corner
    _widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    displayAxes(_widget, ui->meshviewerRenderer->GetRenderWindow()->GetInteractor());

    _renderer->ResetCamera();

    ui->meshviewerRenderer->GetRenderWindow()->GetInteractor()->SetInteractorStyle(_styleCamera);
    ui->meshviewerRenderer->GetRenderWindow()->GetInteractor()->GetPickingManager()->EnabledOn();
    ui->meshviewerRenderer->GetRenderWindow()->GlobalWarningDisplayOff();

    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}

void MeshViewerVTKWidget::displayAxes(vtkSmartPointer<vtkOrientationMarkerWidget> &widget, vtkRenderWindowInteractor *renWindow)
{
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);
    axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 1, 0);
    axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 1);
    axes->SetShaftTypeToCylinder();
    axes->SetCylinderRadius(0.05);

    widget->SetOutlineColor(0.9300, 0.5700, 0.1300);
    widget->SetOrientationMarker(axes);
    widget->SetInteractor(renWindow);
    widget->SetViewport(0.0, 0.0, 0.2, 0.2);
    widget->EnabledOn();
    widget->InteractiveOn();
}

void MeshViewerVTKWidget::slotZoomIn()
{
    if (this->_renderer == NULL) {
        return;
    }
    this->_renderer->GetActiveCamera()->Zoom(1.5);
    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}

void MeshViewerVTKWidget::slotZoomOut()
{
    if (this->_renderer == NULL ) {
        return;
    }
    this->_renderer->GetActiveCamera()->Zoom(0.5);

    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}

void MeshViewerVTKWidget::slotResetCamera()
{
    this->_renderer->ResetCamera();

    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}

void MeshViewerVTKWidget::slotShowLight(bool checked)
{
    if (checked) {
        this->_renderer->AddLight(this->_lightMain);
    } else {
        this->_renderer->RemoveLight(this->_lightMain);
    }
    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}

void MeshViewerVTKWidget::slotScreenShot()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Screenshot"),
                                                    QString("MeshViewer %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH_mm_ss")),
                                                    tr("JPEG File (*.jpg *jpeg);;PNG File (*.png)"));
    if (!fileName.isEmpty()) {
        if (!((fileName.endsWith(".jpg", Qt::CaseInsensitive)) || (fileName.endsWith(".jpeg", Qt::CaseInsensitive)) ||
              (fileName.endsWith(".png", Qt::CaseInsensitive)))) {
            fileName += ".png"; // default
        }
        // Screenshot
        vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
        windowToImageFilter->SetInput(ui->meshviewerRenderer->GetRenderWindow());
        windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
        windowToImageFilter->Update();

        vtkSmartPointer<vtkImageWriter> writer;
        if ((fileName.endsWith(tr(".jpg"), Qt::CaseInsensitive)) || (fileName.endsWith(tr(".jpeg"), Qt::CaseInsensitive))) {
            writer = vtkSmartPointer<vtkJPEGWriter>::New();
        } else {
            writer = vtkSmartPointer<vtkPNGWriter>::New();
        }

        writer->SetFileName(fileName.toUtf8());
        writer->SetInputConnection(windowToImageFilter->GetOutputPort());
        writer->Write();
    }
}

void MeshViewerVTKWidget::slotShowOrientationAxes(bool checked)
{
    if (this->_widget == NULL) {
        return;
    }

    if (!checked) {
        this->_widget->EnabledOff();
    } else {
        this->_widget->EnabledOn();
    }

    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}

void MeshViewerVTKWidget::slotPositiveAxisX()
{
    if (_renderer == NULL) {
        return;
    }

    double wc1[3];
    double fp[3];

    _renderer->GetActiveCamera()->GetFocalPoint(fp);
    _renderer->GetActiveCamera()->GetPosition(wc1);

    float dist = sqrt((wc1[0] - fp[0]) * (wc1[0] - fp[0]) + (wc1[1] - fp[1]) * (wc1[1] - fp[1]) + (wc1[2] - fp[2]) * (wc1[2] - fp[2]));

    _renderer->GetActiveCamera()->SetPosition(-(fp[0] + dist), fp[1], fp[2]);
    _renderer->GetActiveCamera()->SetViewUp(0.0, 0.0, 1.0);

    _renderer->ResetCamera();

    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}

void MeshViewerVTKWidget::slotNegativeAxisX()
{
    if (_renderer == NULL) {
        return;
    }

    double wc1[3];
    double fp[3];

    _renderer->GetActiveCamera()->GetFocalPoint(fp);
    _renderer->GetActiveCamera()->GetPosition(wc1);

    float dist = sqrt((wc1[0] - fp[0]) * (wc1[0] - fp[0]) + (wc1[1] - fp[1]) * (wc1[1] - fp[1]) + (wc1[2] - fp[2]) * (wc1[2] - fp[2]));

    _renderer->GetActiveCamera()->SetPosition((fp[0] + dist), fp[1], fp[2]);
    _renderer->GetActiveCamera()->SetViewUp(0.0, 0.0, 1.0);

    _renderer->ResetCamera();

    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}

void MeshViewerVTKWidget::slotPositiveAxisY()
{
    if (_renderer == NULL) {
        return;
    }

    double wc1[3];
    double fp[3];

    _renderer->GetActiveCamera()->GetFocalPoint(fp);
    _renderer->GetActiveCamera()->GetPosition(wc1);

    float dist = sqrt((wc1[0] - fp[0]) * (wc1[0] - fp[0]) + (wc1[1] - fp[1]) * (wc1[1] - fp[1]) + (wc1[2] - fp[2]) * (wc1[2] - fp[2]));

    _renderer->GetActiveCamera()->SetPosition(fp[0], -(fp[1] + dist), fp[2]);
    _renderer->GetActiveCamera()->SetViewUp(0.0, 0.0, 1.0);

    _renderer->ResetCamera();

    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}

void MeshViewerVTKWidget::slotNegativeAxisY()
{
    if (_renderer == NULL) {
        return;
    }

    double wc1[3];
    double fp[3];

    _renderer->GetActiveCamera()->GetFocalPoint(fp);
    _renderer->GetActiveCamera()->GetPosition(wc1);

    float dist = sqrt((wc1[0] - fp[0]) * (wc1[0] - fp[0]) + (wc1[1] - fp[1]) * (wc1[1] - fp[1]) + (wc1[2] - fp[2]) * (wc1[2] - fp[2]));

    _renderer->GetActiveCamera()->SetPosition(fp[0], (fp[1] + dist), fp[2]);
    _renderer->GetActiveCamera()->SetViewUp(0.0, 0.0, 1.0);

    _renderer->ResetCamera();

    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}

void MeshViewerVTKWidget::slotPositiveAxisZ()
{
    if (_renderer == NULL) {
        return;
    }

    double wc1[3];
    double fp[3];

    _renderer->GetActiveCamera()->GetFocalPoint(fp);
    _renderer->GetActiveCamera()->GetPosition(wc1);

    float dist = sqrt((wc1[0] - fp[0]) * (wc1[0] - fp[0]) + (wc1[1] - fp[1]) * (wc1[1] - fp[1]) + (wc1[2] - fp[2]) * (wc1[2] - fp[2]));

    _renderer->GetActiveCamera()->SetPosition(fp[0], fp[1], -(fp[2] + dist));
    _renderer->GetActiveCamera()->SetViewUp(0.0, 1.0, 0.0);

    _renderer->ResetCamera();

    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}

void MeshViewerVTKWidget::slotNegativeAxisZ()
{
    if (_renderer == NULL) {
        return;
    }

    double wc1[3];
    double fp[3];

    _renderer->GetActiveCamera()->GetFocalPoint(fp);
    _renderer->GetActiveCamera()->GetPosition(wc1);

    float dist = sqrt((wc1[0] - fp[0]) * (wc1[0] - fp[0]) + (wc1[1] - fp[1]) * (wc1[1] - fp[1]) + (wc1[2] - fp[2]) * (wc1[2] - fp[2]));

    _renderer->GetActiveCamera()->SetPosition(fp[0], fp[1], (fp[2] + dist));
    _renderer->GetActiveCamera()->SetViewUp(0.0, 1.0, 0.0);

    _renderer->ResetCamera();

    //ui->meshviewerRenderer->GetRenderWindow()->Render();
    ui->meshviewerRenderer->update();
}
