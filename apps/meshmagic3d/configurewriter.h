#ifndef CONFIGUREWRITER_H
#define CONFIGUREWRITER_H

#include <QDialog>

namespace Ui {
    class ConfigureWriter;
}

class ConfigureWriter : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureWriter(QWidget *parent = 0);
    ~ConfigureWriter();

    int fileType();

private:
    Ui::ConfigureWriter *ui;
};

#endif // CONFIGUREWRITER_H
