SOURCES += Core/GenericCurves3.cpp \
    Core/Lights.cpp \
    Core/LinearCombination3.cpp \
    Core/Materials.cpp \
    Core/RealSquareMatrices.cpp \
    Core/TensorProductSurfaces3.cpp \
    Core/TriangulatedMeshes3.cpp \
    GLWidget.cpp \
    Hermite/Hermite.cpp \
    main.cpp \
    MainWindow.cpp \
    Parametric/ParametricCurves3.cpp \
    SideWidget.cpp \
    Test/TestFunctions.cpp \
    extenddialog.cpp \
    Hermite/CubicHermiteIsoLine.cpp \
    Shader/ShaderPrograms.cpp

FORMS += \
    SideWidget.ui \
    MainWindow.ui \
    extenddialog.ui

HEADERS += Core/Colors4.h \
    Core/DCoordinates3.h \
    Core/Exceptions.h \
    Core/GenericCurves3.h \
    Core/HCoordinates3.h \
    Core/Lights.h \
    Core/LinearCombination3.h \
    Core/Materials.h \
    Core/Matrices.h \
    Core/RealSquareMatrices.h \
    Core/TCoordinates4.h \
    Core/TensorProductSurfaces3.h \
    Core/TriangularFaces.h \
    Core/TriangulatedMeshes3.h \
    GLWidget.h \
    Hermite/Hermite.h \
    MainWindow.h \
    Parametric/ParametricCurves3.h \
    SideWidget.h \
    Test/TestFunctions.h \
    extenddialog.h \
    Hermite/CubicHermiteIsoLine.h \
    Shader/ShaderPrograms.h

INCLUDEPATH += Dependencies/Include

LIBS += Dependencies/Lib/GL/glew32.lib \
        Dependencies/Lib/GL/glew32.dll

QT += opengl
