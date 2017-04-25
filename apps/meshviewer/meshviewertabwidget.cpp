#include "meshviewertabwidget.h"
#include "ui_meshviewertabwidget.h"

MeshViewerTabWidget::MeshViewerTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeshViewerTabWidget)
{
    ui->setupUi(this);
}

MeshViewerTabWidget::~MeshViewerTabWidget()
{
    delete ui;
}
