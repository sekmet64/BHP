#pragma once


#include <GL/glew.h>
#include <QGLWidget>
#include <QGLFormat>
#include <QTimer>
#include "core/TriangulatedMeshes3.h"
#include "core/Lights.h"
#include "hermite/Hermite.h"

class GLWidget: public QGLWidget
{
    Q_OBJECT

private:
    // specifies the properties of the rendering window
    QGLFormat   _format;

    // variables defining the projection matrix
    float       _aspect;            // aspect ratio of the rendering window
    float       _fovy;              // field of view in direction y
    float       _z_near, _z_far;    // distance of near and far clipping planes

    // variables defining the model-view matrix
    float       _eye[3], _center[3], _up[3];

    // variables needed by transformations
    int         _angle_x, _angle_y, _angle_z;
    double      _zoom;
    double      _trans_x, _trans_y, _trans_z;

    cagd::HermitePatch _patch;

    cagd::TriangulatedMesh3 *_before_interpolation, *_after_interpolation;
    cagd::DirectionalLight *_dl;

public:
    // special and default constructor
    GLWidget(QWidget* parent = 0);

    // redeclared virtual functions
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    virtual ~GLWidget();

public slots:
    // public event handling methods/slots
    void set_angle_x(int value);
    void set_angle_y(int value);
    void set_angle_z(int value);

    void set_zoom_factor(double value);

    void set_trans_x(double value);
    void set_trans_y(double value);
    void set_trans_z(double value);
};
