/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      AboutMeshMagic.h
**
** Created: Wed 06. May 16:01:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef ABOUTMESHMAGIC_H
#define ABOUTMESHMAGIC_H

#include "ui_about.h"

class AboutMeshMagic : public QDialog, private Ui::AboutMeshMagic
{
    Q_OBJECT

public:
    explicit AboutMeshMagic(QWidget *parent = 0);
};

#endif // ABOUTMESHMAGIC_H
