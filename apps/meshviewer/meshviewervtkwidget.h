#ifndef MESHVIEWERVTKWIDGET_H
#define MESHVIEWERVTKWIDGET_H

#include <QWidget>

#include <vtkLight.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkCornerAnnotation.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkInteractorStyleTrackballCamera.h>

namespace Ui {
class MeshViewerVTKWidget;
}

namespace MeshViewer3D {

class MeshViewerVTKWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MeshViewerVTKWidget(QWidget *parent = 0);
    ~MeshViewerVTKWidget();

public slots:
    //Right widget slot
    void slotZoomIn();

    void slotZoomOut();

    void slotResetCamera();

    void slotShowLight(bool checked);

    void slotScreenShot();

    void slotShowOrientationAxes(bool checked);

    void slotPositiveAxisX();

    void slotNegativeAxisX();

    void slotPositiveAxisY();

    void slotNegativeAxisY();

    void slotPositiveAxisZ();

    void slotNegativeAxisZ();

private:
    void initRender();
    void displayAxes(vtkSmartPointer<vtkOrientationMarkerWidget> &widget, vtkRenderWindowInteractor *renWindow);

private:
    Ui::MeshViewerVTKWidget *ui;

    vtkSmartPointer<vtkOrientationMarkerWidget> _widget;
    vtkSmartPointer<vtkCornerAnnotation> _cornerAnnotation;

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> _styleCamera;

    vtkSmartPointer<vtkLight> _lightMain;
    vtkSmartPointer<vtkRenderer> _renderer;

};

}

#endif // MESHVIEWERVTKWIDGET_H
