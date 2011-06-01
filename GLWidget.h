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

    bool    _show_derivatives;

    cagd::TriangulatedMesh3 *_tl_mesh;
    cagd::TriangulatedMesh3 *_t_mesh;
    cagd::TriangulatedMesh3 *_tr_mesh;
    cagd::TriangulatedMesh3 *_r_mesh;

    cagd::TriangulatedMesh3 *_br_mesh;
    cagd::TriangulatedMesh3 *_b_mesh;
    cagd::TriangulatedMesh3 *_bl_mesh;
    cagd::TriangulatedMesh3 *_l_mesh;

    bool    _tl_enabled;
    bool    _t_enabled;
    bool    _tr_enabled;
    bool    _r_enabled;

    bool    _br_enabled;
    bool    _b_enabled;
    bool    _bl_enabled;
    bool    _l_enabled;

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

    void toggle_derivatives(bool enabled);

    void toggle_tl(bool checked);
    void toggle_t(bool checked);
    void toggle_tr(bool checked);
    void toggle_r(bool checked);

    void toggle_br(bool checked);
    void toggle_b(bool checked);
    void toggle_bl(bool checked);
    void toggle_l(bool checked);

};
