#ifndef MESHVIEWERTABWIDGET_H
#define MESHVIEWERTABWIDGET_H

#include <QWidget>
#include <QObject>

#include "meshviewervtkwidget.h"

namespace Ui {
class MeshViewerTabWidget;
}

namespace MeshViewer3D {

class MeshViewerTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MeshViewerTabWidget(QWidget *parent = 0);
    ~MeshViewerTabWidget();

private:
    Ui::MeshViewerTabWidget *ui;

    MeshViewerVTKWidget *_vtkWidget;
};

}

#endif // MESHVIEWERTABWIDGET_H
