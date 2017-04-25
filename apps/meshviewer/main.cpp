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

#include "meshviewer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    //qputenv("QT_SCALE_FACTOR", "1");

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);
    MeshViewer meshMagic3D;
    meshMagic3D.show();

    return a.exec();
}
