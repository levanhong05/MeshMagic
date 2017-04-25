#include "meshviewertabwidget.h"
#include "ui_meshviewertabwidget.h"

#include <QVBoxLayout>

using namespace MeshViewer3D;

MeshViewerTabWidget::MeshViewerTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeshViewerTabWidget)
{
    ui->setupUi(this);

    _vtkWidget = new MeshViewerVTKWidget(this);
    QVBoxLayout *vtkLayout = new QVBoxLayout();
    ui->frameVTK->setLayout(vtkLayout);
    vtkLayout->addWidget(_vtkWidget);
    vtkLayout->setContentsMargins(0,0,0,0);

    connect(ui->btnZoomIn, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotZoomIn()));
    connect(ui->btnResetCamera, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotResetCamera()));
    connect(ui->btnZoomOut, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotZoomOut()));

    connect(ui->btnX, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotPositiveAxisX()));
    connect(ui->btnX_, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotNegativeAxisX()));

    connect(ui->btnY, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotPositiveAxisY()));
    connect(ui->btnY_, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotNegativeAxisY()));

    connect(ui->btnZ, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotPositiveAxisZ()));
    connect(ui->btnZ_, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotNegativeAxisZ()));

    connect(ui->btnShowAxes, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotShowOrientationAxes(bool)));
    connect(ui->btnScreenShot, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotScreenShot()));
    connect(ui->btnLight, SIGNAL(clicked(bool)), _vtkWidget, SLOT(slotShowLight(bool)));
}

MeshViewerTabWidget::~MeshViewerTabWidget()
{
    delete ui;
}
