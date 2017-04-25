#ifndef MESHVIEWERVTKWIDGET_H
#define MESHVIEWERVTKWIDGET_H

#include <QWidget>

namespace Ui {
class MeshViewerVTKWidget;
}

class MeshViewerVTKWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MeshViewerVTKWidget(QWidget *parent = 0);
    ~MeshViewerVTKWidget();

private:
    Ui::MeshViewerVTKWidget *ui;
};

#endif // MESHVIEWERVTKWIDGET_H
