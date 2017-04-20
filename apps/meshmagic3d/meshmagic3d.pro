#-------------------------------------------------
#
# Project created by QtCreator 2017-04-20 T13:56:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix:TARGET = ../../bin/meshmagic3d
win32:TARGET = ../../../dist/bin/meshmagic3d

TEMPLATE = app

SOURCES += 	main.cpp\
			about.cpp \
			meshmagic3d.cpp \
			stlfilecore.cpp \
			rotatethread.cpp \
			configurewriter.cpp \
			extractionregionthread.cpp

HEADERS  += \
			about.h \
			branding.h \
			meshmagic3d.h \
			stlfilecore.h \
			rotatethread.h \
			globalvariable.h \
			configurewriter.h \
			extractionregionthread.h \
			interactor/interactorstyledeletecell.h \
			interactor/keyinteractorextractregions.h \
			interactor/mouseinteractorhighlightcell.h \
			interactor/mouseinteractorhighlightline.h \
			interactor/mouseinteractorhighlightactor.h \
			interactor/mouseinteractorhighlightpoint.h \
			interactor/mouseinteractorhighlighttriangle.h \
			interactor/mouseinteractorhighlightcellneighbors.h \
			interactor/mouseinteractorhighlightpointneighbors.h

FORMS    += \
			meshmagic3d.ui \
			about.ui \
			configurewriter.ui

RESOURCES += \
    meshmagic3d_res.qrc

OTHER_FILES += \
    data/MeshMagic.pdf

# vtk6 lib install dir
win32 {
    LIBS+= -L$$OUT_PWD/../../dist/bin
    LIBS+= -L$$OUT_PWD/../../3rdparty/vtk6/install/lib
    LIBS+= -L$$OUT_PWD/../../3rdparty/vtk6/install/bin
    INCLUDEPATH+= $$OUT_PWD/../../3rdparty/vtk6/install/include/vtk-6.3
}

unix {
    LIBS+= -L$$OUT_PWD/../../3rdparty/vtk6/install/lib
    INCLUDEPATH+= $$OUT_PWD/../../3rdparty/vtk6/install/include/vtk-6.3
}

LIBS += -lvtkCommonCore-6.3 \
        -lvtkCommonDataModel-6.3 \
        -lvtkCommonExecutionModel-6.3 \
        -lvtkCommonTransforms-6.3 \
        -lvtkChartsCore-6.3 \
        -lvtkFiltersCore-6.3 \
        -lvtkFiltersExtraction-6.3 \
        -lvtkFiltersGeneral-6.3 \
        -lvtkFiltersGeometry-6.3 \
        -lvtkFiltersHybrid-6.3 \
        -lvtkFiltersModeling-6.3 \
        -lvtkFiltersSelection-6.3 \
        -lvtkFiltersSources-6.3 \
        -lvtkGUISupportQt-6.3 \
        -lvtkGUISupportQtOpenGL-6.3 \
        -lvtkImagingCore-6.3 \
        -lvtkInfovisCore-6.3 \
        -lvtkInfovisLayout-6.3 \
        -lvtkInteractionImage-6.3 \
        -lvtkInteractionStyle-6.3 \
        -lvtkInteractionWidgets-6.3 \
        -lvtkIOCore-6.3 \
        -lvtkIOImage-6.3 \
        -lvtkIOGeometry-6.3 \
        -lvtkIOInfovis-6.3 \
        -lvtkParallelCore-6.3 \
        -lvtkRenderingAnnotation-6.3 \
        -lvtkRenderingContext2D-6.3 \
        -lvtkRenderingContextOpenGL-6.3 \
        -lvtkRenderingCore-6.3 \
        -lvtkRenderingFreeType-6.3 \
        -lvtkRenderingLIC-6.3 \
        -lvtkRenderingLOD-6.3 \
        -lvtkRenderingOpenGL-6.3 \
        -lvtkRenderingQt-6.3 \
        -lvtkRenderingVolume-6.3 \
        -lvtkRenderingVolumeOpenGL-6.3 \
        -lvtkRenderingImage-6.3 \
        -lvtkViewsCore-6.3 \
        -lvtkViewsQt-6.3
