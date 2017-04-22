/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      AboutMeshMagic.cpp
**
** Created: Wed 06. May 16:01:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#include "about.h"
#include "ui_about.h"

#include "version.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
//    QString text = tr("Mesh 3D Viewer\n" \
//                      "Version %1\n" \
//                      "%2 2017 Eric Lee.\n" \
//                      "All rights reserved.").arg(APP_VERSION_FULL).arg(QString::fromUtf8("®"));
    //ui->lblCompany->setText(text);
}

About::~About()
{
    delete ui;
}
