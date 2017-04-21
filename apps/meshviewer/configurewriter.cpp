#include "configurewriter.h"
#include "ui_configurewriter.h"

ConfigureWriter::ConfigureWriter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigureWriter)
{
    ui->setupUi(this);
}

ConfigureWriter::~ConfigureWriter()
{
    delete ui;
}

int ConfigureWriter::fileType()
{
    return ui->cboFileType->currentIndex();
}
