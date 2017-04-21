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
#include "version.h"

About::About(QWidget *parent) :
    QDialog(parent)
{
    ui->setupUi(this);
    QString text = tr("Mesh Magic 3D\n" \
                      "Version %1\n" \
                      "%2 2015 DFM-Engineering.\n" \
                      "All rights reserved.").arg(APP_VERSION_FULL).arg(QString::fromUtf8("®"));
    ui->lblCompany->setText(text);
}
