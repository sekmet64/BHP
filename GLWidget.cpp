#include "GLWidget.h"
#include <iostream>
#include "core/Materials.h"

using namespace cagd;
using namespace std;

//--------------------------------
// special and default constructor
//--------------------------------
GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::Rgba | QGL::DepthBuffer | QGL::DoubleBuffer),
        parent)
{
}

GLWidget::~GLWidget()
{
    if (_dl)
        delete _dl;
    if (_before_interpolation)
    {
        delete _before_interpolation;
        _before_interpolation = 0;
    }
    if (_after_interpolation)
    {
        delete _after_interpolation;
        _after_interpolation = 0;
    }
}

//--------------------------------------------------------------------------------------
// this virtual function is called once before the first call to paintGL() or resizeGL()
//--------------------------------------------------------------------------------------
void GLWidget::initializeGL()
{
    // creating a perspective projection matrix
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    _aspect = (float)width() / (float)height();
    _z_near = 1.0;
    _z_far = 1000.0;
    _fovy = 45.0;

    gluPerspective(_fovy, _aspect, _z_near, _z_far);

    // setting the model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    _eye[0] = _eye[1] = 0.0, _eye[2] = 6.0;
    _center[0] = _center[1] = _center[2] = 0.0;
    _up[0] = _up[2] = 0.0, _up[1] = 1.0;

    gluLookAt(_eye[0], _eye[1], _eye[2], _center[0], _center[1], _center[2], _up[0], _up[1], _up[2]);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // enabling depth test
    glEnable(GL_DEPTH_TEST);

    // setting the color of background
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // initial values of transformation parameters
    _zoom = 1.0;
    _angle_x = _angle_y = _angle_z = 0.0;
    _trans_x = _trans_y = _trans_z = 0.0;

    glewInit();

    _patch.SetData(0, 0, -1.0, 1.0, 0.0);
    _patch.SetData(0, 1, 1.0, 1.0, 0.0);
    _patch.SetData(0, 2, 0.0, 1.0, 1.0);
    _patch.SetData(0, 3, 0.0, 2.0, 2.0);

    _patch.SetData(1, 0, -1.0, -1.0, 0.0);
    _patch.SetData(1, 1, 1.0, -1.0, 0.0);
    _patch.SetData(1, 2, 0.0, 0.0, 1.0);
    _patch.SetData(1, 3, 0.0, 0.0, -2.0);

    _patch.SetData(2, 0, 1.0, -2.0, 0.0);
    _patch.SetData(2, 1, 1.0, -1.0, 2.0);
    _patch.SetData(2, 2, 1.0, 1.0, 2.0);
    _patch.SetData(2, 3, 1.0, 2.0, 0.0);

    _patch.SetData(3, 0, 2.0, -2.0, 0.0);
    _patch.SetData(3, 1, 2.0, -1.0, 0.0);
    _patch.SetData(3, 2, 2.0, 1.0, 0.0);
    _patch.SetData(3, 3, 2.0, 2.0, 0.0);

    _patch.UpdateVertexBufferObjectsOfDerivatives();

    _before_interpolation = _patch.GenerateImage(30, 30, GL_STATIC_DRAW);

    if (_before_interpolation)
        _before_interpolation->UpdateVertexBufferObjects();

    RowMatrix<GLdouble> u_knot_vector(4);
    u_knot_vector(0) = 0.0;
    u_knot_vector(1) = 1.0 / 3.0;
    u_knot_vector(2) = 2.0 / 3.0;
    u_knot_vector(3) = 1.0;

    ColumnMatrix<GLdouble> v_knot_vector(4);
    v_knot_vector(0) = 0.0;
    v_knot_vector(1) = 1.0 / 3.0;
    v_knot_vector(2) = 2.0 / 3.0;
    v_knot_vector(3) = 1.0;

//    Matrix<DCoordinate3> data_points_to_interpolate(4,4);
//    for (GLuint row=0; row<4; ++row)
//        for (GLuint column = 0; column < 4; ++column)
//            _patch.GetData(row, column, data_points_to_interpolate(row, column));

//    if (_patch.UpdateDataForInterpolation(u_knot_vector, v_knot_vector, data_points_to_interpolate))
//    {
//        _after_interpolation = _patch.GenerateImage(30, 30, GL_STATIC_DRAW);

//        if (_after_interpolation)
//            _after_interpolation->UpdateVertexBufferObjects();
//    }

    _dl = 0;
    _dl = new DirectionalLight(GL_LIGHT0, HCoordinate3(0,0,1,0), Color4(0.4,0.4,0.4,1), Color4(0.8,0.8,0.8,1), Color4(0,0,1,1));

    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    _dl->Enable();
}

//-----------------------
// the rendering function
//-----------------------
void GLWidget::paintGL()
{
    // clears the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // stores/duplicates the original model view matrix
    glPushMatrix();

        // applying transformations
        glRotatef(_angle_x, 1.0, 0.0, 0.0);
        glRotatef(_angle_y, 0.0, 1.0, 0.0);
        glRotatef(_angle_z, 0.0, 0.0, 1.0);

        glTranslated(_trans_x, _trans_y, _trans_z);

        glScaled(_zoom, _zoom, _zoom);

        if (_before_interpolation)
        {
            MatFBRuby.Apply();
            _before_interpolation->Render();
            glDisable(GL_LIGHTING);
            _patch.RenderDerivatives();
            glEnable(GL_LIGHTING);
        }

//        if (_after_interpolation)
//        {
//            glEnable(GL_BLEND);
//            glDepthMask(GL_FALSE);
//            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//               MatFBTurquoise.Apply();
//              //_after_interpolation->Render();
//            glDepthMask(GL_TRUE);
//            glDisable(GL_BLEND);
//        }

    // pops the current matrix stack, replacing the current matrix with the one below it on the stack,
    // i.e., the original model view matrix is restored
    glPopMatrix();
}

//----------------------------------------------------------------------------
// when the main window is resized one needs to redefine the projection matrix
//----------------------------------------------------------------------------
void GLWidget::resizeGL(int w, int h)
{
    // setting the new size of the rendering context
    glViewport(0, 0, w, h);

    // redefining the projection matrix
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    _aspect = (float)w / (float)h;

    gluPerspective(_fovy, _aspect, _z_near, _z_far);

    // switching back to the model view matrix
    glMatrixMode(GL_MODELVIEW);

    updateGL();
}

//-----------------------------------
// implementation of the public slots
//-----------------------------------

void GLWidget::set_angle_x(int value)
{
    if (_angle_x != value)
    {
        _angle_x = value;
        updateGL();
    }
}

void GLWidget::set_angle_y(int value)
{
    if (_angle_y != value)
    {
        _angle_y = value;
        updateGL();
    }
}

void GLWidget::set_angle_z(int value)
{
    if (_angle_z != value)
    {
        _angle_z = value;
        updateGL();
    }
}

void GLWidget::set_zoom_factor(double value)
{
    if (_zoom != value)
    {
        _zoom = value;
        updateGL();
    }
}

void GLWidget::set_trans_x(double value)
{
    if (_trans_x != value)
    {
        _trans_x = value;
        updateGL();
    }
}

void GLWidget::set_trans_y(double value)
{
    if (_trans_y != value)
    {
        _trans_y = value;
        updateGL();
    }
}

void GLWidget::set_trans_z(double value)
{
    if (_trans_z != value)
    {
        _trans_z = value;
        updateGL();
    }
}

