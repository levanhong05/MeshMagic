/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      main.cpp
**
** Created: Wed 06. May 08:16:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#include "meshmagic3d.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MeshMagic3D meshMagic3D;
    meshMagic3D.show();

    return a.exec();
}
