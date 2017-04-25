#ifndef MESHVIEWERTABWIDGET_H
#define MESHVIEWERTABWIDGET_H

#include <QWidget>

namespace Ui {
class MeshViewerTabWidget;
}

class MeshViewerTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MeshViewerTabWidget(QWidget *parent = 0);
    ~MeshViewerTabWidget();

private:
    Ui::MeshViewerTabWidget *ui;
};

#endif // MESHVIEWERTABWIDGET_H
