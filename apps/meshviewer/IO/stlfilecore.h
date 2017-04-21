/********************************************************************************
** Program:   Mesh Magic 3D v1.0.0
** File:      RotateThread.h
**
** Created: Wed 06. May 08:16:00 2015
**      by: Hong Le Van
**      in: Ho Chi Minh City, Vietnam
**
**
** Copyright (c) Hong Le Van (Eric Lee)
** All rights reserved.
********************************************************************************/

#ifndef STLFILECORE_H
#define STLFILECORE_H

#include <QMutex>
#include <QThread>
#include <QString>

#include <vtkActor.h>
#include <vtkSmartPointer.h>

#include <vector>

class STLFileCore : public QThread
{
    Q_OBJECT
public:
    STLFileCore(QObject *parent = 0);
    ~STLFileCore();

    void STLReading(QString fileName, int fileType = 0, int typeAction = 0);
    void STLSaving(std::vector<vtkSmartPointer<vtkActor> > lstActors, QString fileName, int fileType = 0, int encode = 0, int typeAction = 0);
    void abort();

protected:
    virtual void ReadSTLFile(QString fileName);
    virtual void SaveSTLFile(QString fileName);

signals:
    void emitReadSTLDone(vtkActor *actor);
    void emitWriteSTLDone();

protected:
    void run();

private:
    QMutex mutex;

    int m_Type;             //0: Read       1: Save
    int m_FileSupport;      //0: STL        1: PLY          2: VTK          3: VTP
    int m_EncodeSupport;    //0: ANSCII     1: Binary

    bool m_abort;
    bool m_working;

    QString m_FileName;
    vtkActor *m_Actor;
    std::vector<vtkSmartPointer<vtkActor> > lstActors;
};

#endif // STLFILECORE_H
