SOURCES += main.cpp \
    MainWindow.cpp \
    SideWidget.cpp \
    GLWidget.cpp \
    core/RealSquareMatrices.cpp \
    core/GenericCurves3.cpp \
    parametric/ParametricCurves3.cpp \
    test/TestFunctions.cpp \
    core/LinearCombination3.cpp \
    core/Lights.cpp \
    core/Materials.cpp \
    core/TriangulatedMeshes3.cpp \
    core/TensorProductSurfaces3.cpp \
    hermite/Hermite.cpp
FORMS += \
    SideWidget.ui \
    MainWindow.ui
HEADERS += MainWindow.h \
    SideWidget.h \
    GLWidget.h \
    core/DCoordinates3.h \
    core/Matrices.h \
    core/RealSquareMatrices.h \
    core/GenericCurves3.h \
    parametric/ParametricCurves3.h \
    test/TestFunctions.h \
    core/LinearCombination3.h \
    core/Exceptions.h \
    core/HCoordinates3.h \
    core/TCoordinates4.h \
    core/TriangularFaces.h \
    core/Colors4.h \
    core/Lights.h \
    core/Materials.h \
    core/TriangulatedMeshes3.h \
    core/TensorProductSurfaces3.h \
    hermite/Hermite.h

INCLUDEPATH += ./Dependencies/Include

LIBS += ./Dependencies/Lib/GL/glew32.lib \
        ./Dependencies/Lib/GL/glew32.dll

QT += opengl
