#include "ConfigureWriter.h"
#include "ui_ConfigureWriter.h"

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
