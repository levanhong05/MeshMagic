#include "meshviewervtkwidget.h"
#include "ui_meshviewervtkwidget.h"

MeshViewerVTKWidget::MeshViewerVTKWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeshViewerVTKWidget)
{
    ui->setupUi(this);
}

MeshViewerVTKWidget::~MeshViewerVTKWidget()
{
    delete ui;
}
