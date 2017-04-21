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

#ifndef ABOUT_H
#define ABOUT_H

#include "ui_about.h"

namespace Ui
{
class About;
}

class About : public QDialog
{
    Q_OBJECT
public:
    explicit About(QWidget *parent = 0);

private:
    Ui::About *ui;
};

#endif // ABOUT_H
