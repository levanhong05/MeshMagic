@echo off

echo building VTK 6 for windows

echo setting environment variables;

set INSTALLDIR=%CD%\install

echo "%QTDIR%/bin/qmake.exe"

cmake -Wno-dev -G "Visual Studio 12 2013 Win64" -DBUILD_SHARED_LIBS=ON -DVTK_USE_QT=ON -DBUILD_TESTING=OFF -DCMAKE_CXX_MP_FLAG=ON -DCMAKE_CXX_MP_NUM_PROCESSORS=4 -DVTK_QT_VERSION=5 -DQT_QMAKE_EXECUTABLE:STRING=qmake.exe -DVTK_Group_Qt:BOOL=ON -DCMAKE_INSTALL_PREFIX="%INSTALLDIR%" "%*"

msbuild /m /p:TreatWarningsAsErrors=false;Configuration=Release ALL_BUILD.vcxproj

msbuild /m /p:TreatWarningsAsErrors=false;Configuration=Release INSTALL.vcxproj

copy install\bin\*.dll ..\..\dist\bin\
