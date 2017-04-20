TEMPLATE = aux

CONFIG -= debug_and_release

win32 {
    SOURCEPATH=$$PWD
CONFIG(release, debug|release) {
    BUILDBAT_RELEASE=$$PWD/build_release.bat
    first.commands = $(COPY) $$replace(BUILDBAT_RELEASE,/,\\) && call build_release.bat $$replace(SOURCEPATH,/,\\)
} else {
    BUILDBAT=$$PWD/build.bat
    first.commands = $(COPY) $$replace(BUILDBAT,/,\\) && call build.bat $$replace(SOURCEPATH,/,\\)
}
    clean.depends = realclean

    realclean.targets = clean.bat
    realclean.commands = echo clean it up

    QMAKE_EXTRA_TARGETS = first realclean

    OTHER_FILES += build.bat\
                    build_release.bat
} #/win32

unix {

CONFIG(release) {
    first.commands = mkdir -p install && /usr/local/bin/cmake -Wno-dev -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DVTK_USE_QT=ON -DBUILD_TESTING=OFF -DVTK_QT_VERSION=5 -DQT_QMAKE_EXECUTABLE:STRING=qmake -DVTK_Group_Qt:BOOL=ON -DCMAKE_INSTALL_PREFIX=$$OUT_PWD/install $$PWD && make -j 3 && make install
} else {
    first.commands = mkdir -p install && /usr/local/bin/cmake -Wno-dev -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -DVTK_USE_QT=ON -DBUILD_TESTING=OFF -DVTK_QT_VERSION=5 -DQT_QMAKE_EXECUTABLE:STRING=qmake -DVTK_Group_Qt:BOOL=ON -DCMAKE_INSTALL_PREFIX=$$OUT_PWD/install $$PWD && make -j 3 && make install
}

QMAKE_EXTRA_TARGETS = first

} #/unix
