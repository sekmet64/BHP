#include "GLWidget.h"
#include <iostream>
#include "core/Materials.h"
#include "extenddialog.h"
#include "Core/Exceptions.h"
#include <QMessageBox>
#include <QToolButton>
#include <QWheelEvent>

using namespace cagd;
using namespace std;

//--------------------------------
// special and default constructor
//--------------------------------
GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::Rgba | QGL::DepthBuffer | QGL::DoubleBuffer),
        parent), _show_derivatives(false),
    _tl_enabled(false),
    _t_enabled(false),
    _tr_enabled(false),
    _r_enabled(false),

    _br_enabled(false),
    _b_enabled(false),
    _bl_enabled(false),
    _l_enabled(false),

    _show_u_iso_lines(false),
    _show_v_iso_lines(false),

    _show_u_iso_derivates(false),
    _show_v_iso_derivates(false),

    _show_surface(true),
    _u_isoline_count(5),
    _v_isoline_count(5)
{
}

GLWidget::~GLWidget()
{    
    if (_patch_mesh)
    {
        delete _patch_mesh;
        _patch_mesh = 0;
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

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

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


    //initialize u_iso_lines
    _patch.GenerateUIsoLines(_u_isoline_count, 30);

    //initialize v_iso_lines
    _patch.GenerateVIsoLines(_v_isoline_count, 30);

    _patch_mesh = _patch.GenerateImage(30, 30, GL_STATIC_DRAW);

    if (_patch_mesh)
        _patch_mesh->UpdateVertexBufferObjects();

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

    try
    {
        // directional ligth
        if (!_shader.InstallShaders("Shaders/directional_light.vert",
                                    "Shaders/directional_light.frag",
                                    GL_TRUE))
        {
            QMessageBox::critical(this, "Shader error",
                                  "Error installing shaders");
        }
    }
    catch (Exception &e)
    {
    }




    _dl = new DirectionalLight(GL_LIGHT0, HCoordinate3(0,0,1,0),
                               Color4(0.4,0.4,0.4,1),
                               Color4(0.8,0.8,0.8,1),
                               Color4(0,0,1,1));
    _dl->Enable();
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    _shader.Enable(GL_TRUE);
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

        if (_patch_mesh)
        {
            if (_show_surface)
            {
                MatFBRuby.Apply();
                _patch_mesh->Render();

                MatFBSilver.Apply();

                if (_tl_enabled)
                    _tl_mesh->Render();
                if (_t_enabled)
                    _t_mesh->Render();
                if (_tr_enabled)
                    _tr_mesh->Render();
                if (_r_enabled)
                    _r_mesh->Render();
                if (_br_enabled)
                    _br_mesh->Render();
                if (_b_enabled)
                    _b_mesh->Render();
                if (_bl_enabled)
                    _bl_mesh->Render();
                if (_l_enabled)
                    _l_mesh->Render();
            }



            MatFBEmerald.Apply();

            //rendering u_iso_lines
            if (_show_u_iso_lines)
            {
                _patch.RenderUIsoLines(0);
                if (_tl_enabled)
                    _patch.GetTL()->RenderUIsoLines(0);
                if (_t_enabled)
                    _patch.GetT()->RenderUIsoLines(0);
                if (_tr_enabled)
                    _patch.GetTR()->RenderUIsoLines(0);
                if (_r_enabled)
                    _patch.GetR()->RenderUIsoLines(0);

                if (_br_enabled)
                    _patch.GetBR()->RenderUIsoLines(0);
                if (_b_enabled)
                    _patch.GetB()->RenderUIsoLines(0);
                if (_bl_enabled)
                    _patch.GetBL()->RenderUIsoLines(0);
                if (_l_enabled)
                    _patch.GetL()->RenderUIsoLines(0);
            }

            //rendering v_iso_lines
            if (_show_v_iso_lines)
            {
                _patch.RenderVIsoLines(0);
                if (_tl_enabled)
                    _patch.GetTL()->RenderVIsoLines(0);
                if (_t_enabled)
                    _patch.GetT()->RenderVIsoLines(0);
                if (_tr_enabled)
                    _patch.GetTR()->RenderVIsoLines(0);
                if (_r_enabled)
                    _patch.GetR()->RenderVIsoLines(0);

                if (_br_enabled)
                    _patch.GetBR()->RenderVIsoLines(0);
                if (_b_enabled)
                    _patch.GetB()->RenderVIsoLines(0);
                if (_bl_enabled)
                    _patch.GetBL()->RenderVIsoLines(0);
                if (_l_enabled)
                    _patch.GetL()->RenderVIsoLines(0);
            }

            MatFBBrass.Apply();

            if (_show_u_iso_derivates)
            {
                _patch.RenderUIsoLines(1);
                if (_tl_enabled)
                    _patch.GetTL()->RenderUIsoLines(1);
                if (_t_enabled)
                    _patch.GetT()->RenderUIsoLines(1);
                if (_tr_enabled)
                    _patch.GetTR()->RenderUIsoLines(1);
                if (_r_enabled)
                    _patch.GetR()->RenderUIsoLines(1);

                if (_br_enabled)
                    _patch.GetBR()->RenderUIsoLines(1);
                if (_b_enabled)
                    _patch.GetB()->RenderUIsoLines(1);
                if (_bl_enabled)
                    _patch.GetBL()->RenderUIsoLines(1);
                if (_l_enabled)
                    _patch.GetL()->RenderUIsoLines(1);
            }

            MatFBGold.Apply();

            if (_show_v_iso_derivates)
            {
                _patch.RenderVIsoLines(1);
                if (_tl_enabled)
                    _patch.GetTL()->RenderVIsoLines(1);
                if (_t_enabled)
                    _patch.GetT()->RenderVIsoLines(1);
                if (_tr_enabled)
                    _patch.GetTR()->RenderVIsoLines(1);
                if (_r_enabled)
                    _patch.GetR()->RenderVIsoLines(1);

                if (_br_enabled)
                    _patch.GetBR()->RenderVIsoLines(1);
                if (_b_enabled)
                    _patch.GetB()->RenderVIsoLines(1);
                if (_bl_enabled)
                    _patch.GetBL()->RenderVIsoLines(1);
                if (_l_enabled)
                    _patch.GetL()->RenderVIsoLines(1);
            }

            // rendering derivatives
            if (_show_derivatives)
            {
                glDisable(GL_LIGHTING);
                _patch.RenderDerivatives();

                if (_tl_enabled)
                    _patch.GetTL()->RenderDerivatives();
                if (_t_enabled)
                    _patch.GetT()->RenderDerivatives();
                if (_tr_enabled)
                    _patch.GetTR()->RenderDerivatives();
                if (_r_enabled)
                    _patch.GetR()->RenderDerivatives();

                if (_br_enabled)
                    _patch.GetBR()->RenderDerivatives();
                if (_b_enabled)
                    _patch.GetB()->RenderDerivatives();
                if (_bl_enabled)
                    _patch.GetBL()->RenderDerivatives();
                if (_l_enabled)
                    _patch.GetL()->RenderDerivatives();

                glEnable(GL_LIGHTING);
            }


        }

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

void GLWidget::toggle_derivatives(bool enabled)
{
    _show_derivatives = enabled;
    repaint();
}

void GLWidget::toggle_tl(bool checked)
{
    if (checked)
    {
        DCoordinate3 d_00, d_01, d_02, d_03,
                d_10, d_11, d_12, d_13,
                d_20, d_21, d_22, d_23,
                d_30, d_31, d_32, d_33;

        _patch.GetData(0, 0, d_11);
        _patch.GetData(0, 2, d_13);
        _patch.GetData(2, 0, d_31);
        _patch.GetData(2, 2, d_33);


        if (_t_enabled)
        {
            HermitePatch *t = _patch.GetT();
            t->GetData(0, 0, d_01);
            t->GetData(0, 2, d_03);
            t->GetData(2, 0, d_21);
            t->GetData(2, 2, d_23);
        }

        if (_l_enabled)
        {
            HermitePatch *l = _patch.GetL();
            l->GetData(0, 0, d_10);
            l->GetData(0, 2, d_12);
            l->GetData(2, 0, d_30);
            l->GetData(2, 2, d_32);
        }

        ExtendDialog *dialog = new ExtendDialog(
                    d_00, d_01, d_02, d_03,
                    d_10, d_11, d_12, d_13,
                    d_20, d_21, d_22, d_23,
                    d_30, d_31, d_32, d_33);


        dialog->setup_tl(_l_enabled, _t_enabled);

        if (dialog->exec() == QDialog::Accepted)
        {
            Matrix<DCoordinate3> m = dialog->getData();
            _patch.ExtendTL(m);

            HermitePatch* tl = _patch.GetTL();

            tl->UpdateVertexBufferObjectsOfDerivatives();
            _tl_mesh = tl->GenerateImage(30, 30, GL_STATIC_DRAW);
            _tl_mesh->UpdateVertexBufferObjects();
            tl->GenerateUIsoLines(_u_isoline_count, 30);
            tl->GenerateVIsoLines(_v_isoline_count, 30);
            _tl_enabled = true;
            repaint();
        }
        else
        {
            dynamic_cast<QToolButton*>(sender())->setChecked(false);
        }
    }
    else
    {
        if (_tl_enabled)
        {
            _tl_mesh->DeleteVertexBufferObjects();
            delete _tl_mesh;
            _patch.DeleteTL();
            _tl_enabled = false;
            repaint();
        }
    }
}

void GLWidget::toggle_t(bool checked)
{
    if (checked)
    {
        DCoordinate3 d_00, d_01, d_02, d_03,
                d_10, d_11, d_12, d_13,
                d_20, d_21, d_22, d_23,
                d_30, d_31, d_32, d_33;


        _patch.GetData(0, 0, d_10);
        _patch.GetData(0, 1, d_11);
        _patch.GetData(0, 2, d_12);
        _patch.GetData(0, 3, d_13);

        _patch.GetData(2, 0, d_30);
        _patch.GetData(2, 1, d_31);
        _patch.GetData(2, 2, d_32);
        _patch.GetData(2, 3, d_33);

        if (_tl_enabled)
        {
            HermitePatch *tl = _patch.GetTL();
            tl->GetData(0, 1, d_00);
            tl->GetData(0, 3, d_02);
            tl->GetData(2, 1, d_20);
            tl->GetData(2, 3, d_22);
        }

        if (_tr_enabled)
        {
            HermitePatch *tr = _patch.GetTR();
            tr->GetData(0, 0, d_01);
            tr->GetData(0, 2, d_03);
            tr->GetData(2, 0, d_21);
            tr->GetData(2, 2, d_23);
        }


        ExtendDialog *dialog = new ExtendDialog(
                    d_00, d_01, d_02, d_03,
                    d_10, d_11, d_12, d_13,
                    d_20, d_21, d_22, d_23,
                    d_30, d_31, d_32, d_33);

        dialog->setup_t(_tl_enabled, _tr_enabled);

        if (dialog->exec() == QDialog::Accepted)
        {
            Matrix<DCoordinate3> m = dialog->getData();
            _patch.ExtendT(m);

            HermitePatch* t = _patch.GetT();

            t->UpdateVertexBufferObjectsOfDerivatives();
            t->GenerateUIsoLines(_u_isoline_count, 30);
            t->GenerateVIsoLines(_v_isoline_count, 30);
            _t_mesh = t->GenerateImage(30, 30, GL_STATIC_DRAW);
            _t_mesh->UpdateVertexBufferObjects();
            _t_enabled = true;
            repaint();
        }
        else
        {
            dynamic_cast<QToolButton*>(sender())->setChecked(false);
        }
    }
    else
    {
        if (_t_enabled)
        {
            _t_mesh->DeleteVertexBufferObjects();
            delete _t_mesh;
            _patch.DeleteT();
            _t_enabled = false;
            repaint();
        }
    }
}

void GLWidget::toggle_tr(bool checked)
{
    if (checked)
    {
        DCoordinate3 d_00, d_01, d_02, d_03,
                d_10, d_11, d_12, d_13,
                d_20, d_21, d_22, d_23,
                d_30, d_31, d_32, d_33;


        _patch.GetData(0, 1, d_10);
        _patch.GetData(0, 3, d_12);
        _patch.GetData(2, 1, d_30);
        _patch.GetData(2, 3, d_32);

        if (_t_enabled)
        {
            HermitePatch *t = _patch.GetT();
            t->GetData(0, 1, d_00);
            t->GetData(0, 3, d_02);
            t->GetData(2, 1, d_20);
            t->GetData(2, 3, d_22);
        }

        if (_r_enabled)
        {
            HermitePatch *r = _patch.GetR();
            r->GetData(0, 1, d_11);
            r->GetData(0, 3, d_13);
            r->GetData(2, 1, d_31);
            r->GetData(2, 3, d_33);
        }


        ExtendDialog *dialog = new ExtendDialog(
                    d_00, d_01, d_02, d_03,
                    d_10, d_11, d_12, d_13,
                    d_20, d_21, d_22, d_23,
                    d_30, d_31, d_32, d_33);

        dialog->setup_tr(_t_enabled, _r_enabled);

        if (dialog->exec() == QDialog::Accepted)
        {
            Matrix<DCoordinate3> m = dialog->getData();
            _patch.ExtendTR(m);

            HermitePatch* tr = _patch.GetTR();

            tr->UpdateVertexBufferObjectsOfDerivatives();
            tr->GenerateUIsoLines(_u_isoline_count, 30);
            tr->GenerateVIsoLines(_v_isoline_count, 30);
            _tr_mesh = tr->GenerateImage(30, 30, GL_STATIC_DRAW);
            _tr_mesh->UpdateVertexBufferObjects();
            _tr_enabled = true;
            repaint();
        }
        else
        {
            dynamic_cast<QToolButton*>(sender())->setChecked(false);
        }
    }
    else
    {
        if (_tr_enabled)
        {
            _tr_mesh->DeleteVertexBufferObjects();
            delete _tr_mesh;
            _patch.DeleteTR();
            _tr_enabled = false;
            repaint();
        }
    }
}

void GLWidget::toggle_r(bool checked)
{
    if (checked)
    {
        DCoordinate3 d_00, d_01, d_02, d_03,
                d_10, d_11, d_12, d_13,
                d_20, d_21, d_22, d_23,
                d_30, d_31, d_32, d_33;


        _patch.GetData(0, 1, d_00);
        _patch.GetData(0, 3, d_02);
        _patch.GetData(2, 1, d_20);
        _patch.GetData(2, 3, d_22);

        _patch.GetData(1, 1, d_10);
        _patch.GetData(1, 3, d_12);
        _patch.GetData(3, 1, d_30);
        _patch.GetData(3, 3, d_32);

        if (_tr_enabled)
        {
            HermitePatch *tr = _patch.GetTR();
            tr->GetData(1, 1, d_01);
            tr->GetData(1, 3, d_03);
            tr->GetData(3, 1, d_21);
            tr->GetData(3, 3, d_23);
        }

        if (_br_enabled)
        {
            HermitePatch *br = _patch.GetBR();
            br->GetData(0, 1, d_11);
            br->GetData(0, 3, d_13);
            br->GetData(2, 1, d_31);
            br->GetData(2, 3, d_33);
        }


        ExtendDialog *dialog = new ExtendDialog(
                    d_00, d_01, d_02, d_03,
                    d_10, d_11, d_12, d_13,
                    d_20, d_21, d_22, d_23,
                    d_30, d_31, d_32, d_33);

        dialog->setup_r(_tr_enabled, _br_enabled);

        if (dialog->exec() == QDialog::Accepted)
        {
            Matrix<DCoordinate3> m = dialog->getData();
            _patch.ExtendR(m);

            HermitePatch* r = _patch.GetR();

            r->UpdateVertexBufferObjectsOfDerivatives();
            r->GenerateUIsoLines(_u_isoline_count, 30);
            r->GenerateVIsoLines(_v_isoline_count, 30);
            _r_mesh = r->GenerateImage(30, 30, GL_STATIC_DRAW);
            _r_mesh->UpdateVertexBufferObjects();
            _r_enabled = true;
            repaint();
        }
        else
        {
            dynamic_cast<QToolButton*>(sender())->setChecked(false);
        }
    }
    else
    {
        if (_r_enabled)
        {
            _r_mesh->DeleteVertexBufferObjects();
            delete _r_mesh;
            _patch.DeleteR();
            _r_enabled = false;
            repaint();
        }
    }
}

void GLWidget::toggle_br(bool checked)
{
    if (checked)
    {
        DCoordinate3 d_00, d_01, d_02, d_03,
                d_10, d_11, d_12, d_13,
                d_20, d_21, d_22, d_23,
                d_30, d_31, d_32, d_33;


        _patch.GetData(1, 1, d_00);
        _patch.GetData(1, 3, d_02);
        _patch.GetData(3, 1, d_20);
        _patch.GetData(3, 3, d_22);

        if (_r_enabled)
        {
            HermitePatch *r = _patch.GetR();
            r->GetData(1, 1, d_01);
            r->GetData(1, 3, d_03);
            r->GetData(3, 1, d_21);
            r->GetData(3, 3, d_23);
        }

        if (_b_enabled)
        {
            HermitePatch *b = _patch.GetB();
            b->GetData(1, 1, d_10);
            b->GetData(1, 3, d_12);
            b->GetData(3, 1, d_30);
            b->GetData(3, 3, d_32);
        }


        ExtendDialog *dialog = new ExtendDialog(
                    d_00, d_01, d_02, d_03,
                    d_10, d_11, d_12, d_13,
                    d_20, d_21, d_22, d_23,
                    d_30, d_31, d_32, d_33);

        dialog->setup_br(_r_enabled, _b_enabled);

        if (dialog->exec() == QDialog::Accepted)
        {
            Matrix<DCoordinate3> m = dialog->getData();
            _patch.ExtendBR(m);

            HermitePatch* br = _patch.GetBR();

            br->UpdateVertexBufferObjectsOfDerivatives();
            br->GenerateUIsoLines(_u_isoline_count, 30);
            br->GenerateVIsoLines(_v_isoline_count, 30);
            _br_mesh = br->GenerateImage(30, 30, GL_STATIC_DRAW);
            _br_mesh->UpdateVertexBufferObjects();
            _br_enabled = true;
            repaint();
        }
        else
        {
            dynamic_cast<QToolButton*>(sender())->setChecked(false);
        }
    }
    else
    {
        if (_br_enabled)
        {
            _br_mesh->DeleteVertexBufferObjects();
            delete _br_mesh;
            _patch.DeleteBR();
            _br_enabled = false;
            repaint();
        }
    }
}

void GLWidget::toggle_b(bool checked)
{
    if (checked)
    {
        DCoordinate3 d_00, d_01, d_02, d_03,
                d_10, d_11, d_12, d_13,
                d_20, d_21, d_22, d_23,
                d_30, d_31, d_32, d_33;

        _patch.GetData(1, 0, d_00);
        _patch.GetData(1, 2, d_02);
        _patch.GetData(3, 0, d_20);
        _patch.GetData(3, 2, d_22);

        _patch.GetData(1, 1, d_01);
        _patch.GetData(1, 3, d_03);
        _patch.GetData(3, 1, d_21);
        _patch.GetData(3, 3, d_23);

        if (_br_enabled)
        {
            HermitePatch *br = _patch.GetBR();
            br->GetData(1, 0, d_11);
            br->GetData(1, 2, d_13);
            br->GetData(3, 0, d_31);
            br->GetData(3, 2, d_33);
        }

        if (_bl_enabled)
        {
            HermitePatch *bl = _patch.GetBL();
            bl->GetData(1, 1, d_10);
            bl->GetData(1, 3, d_12);
            bl->GetData(3, 1, d_30);
            bl->GetData(3, 3, d_32);
        }


        ExtendDialog *dialog = new ExtendDialog(
                    d_00, d_01, d_02, d_03,
                    d_10, d_11, d_12, d_13,
                    d_20, d_21, d_22, d_23,
                    d_30, d_31, d_32, d_33);

        dialog->setup_b(_br_enabled, _bl_enabled);

        if (dialog->exec() == QDialog::Accepted)
        {
            Matrix<DCoordinate3> m = dialog->getData();
            _patch.ExtendB(m);

            HermitePatch* b = _patch.GetB();

            b->UpdateVertexBufferObjectsOfDerivatives();
            b->GenerateUIsoLines(_u_isoline_count, 30);
            b->GenerateVIsoLines(_v_isoline_count, 30);
            _b_mesh = b->GenerateImage(30, 30, GL_STATIC_DRAW);
            _b_mesh->UpdateVertexBufferObjects();
            _b_enabled = true;
            repaint();
        }
        else
        {
            dynamic_cast<QToolButton*>(sender())->setChecked(false);
        }
    }
    else
    {
        if (_b_enabled)
        {
            _b_mesh->DeleteVertexBufferObjects();
            delete _b_mesh;
            _patch.DeleteB();
            _b_enabled = false;
            repaint();
        }
    }
}

void GLWidget::toggle_bl(bool checked)
{
    if (checked)
    {
        DCoordinate3 d_00, d_01, d_02, d_03,
                d_10, d_11, d_12, d_13,
                d_20, d_21, d_22, d_23,
                d_30, d_31, d_32, d_33;

        _patch.GetData(1, 0, d_01);
        _patch.GetData(1, 2, d_03);
        _patch.GetData(3, 0, d_21);
        _patch.GetData(3, 2, d_23);

        if (_b_enabled)
        {
            HermitePatch *b = _patch.GetB();
            b->GetData(1, 0, d_11);
            b->GetData(1, 2, d_13);
            b->GetData(3, 0, d_31);
            b->GetData(3, 2, d_33);
        }

        if (_l_enabled)
        {
            HermitePatch *l = _patch.GetL();
            l->GetData(1, 0, d_00);
            l->GetData(1, 2, d_02);
            l->GetData(3, 0, d_20);
            l->GetData(3, 2, d_22);
        }


        ExtendDialog *dialog = new ExtendDialog(
                    d_00, d_01, d_02, d_03,
                    d_10, d_11, d_12, d_13,
                    d_20, d_21, d_22, d_23,
                    d_30, d_31, d_32, d_33);

        dialog->setup_bl(_b_enabled, _l_enabled);

        if (dialog->exec() == QDialog::Accepted)
        {
            Matrix<DCoordinate3> m = dialog->getData();
            _patch.ExtendBL(m);

            HermitePatch* bl = _patch.GetBL();

            bl->UpdateVertexBufferObjectsOfDerivatives();
            bl->GenerateUIsoLines(_u_isoline_count, 30);
            bl->GenerateVIsoLines(_v_isoline_count, 30);
            _bl_mesh = bl->GenerateImage(30, 30, GL_STATIC_DRAW);
            _bl_mesh->UpdateVertexBufferObjects();
            _bl_enabled = true;
            repaint();
        }
        else
        {
            dynamic_cast<QToolButton*>(sender())->setChecked(false);
        }
    }
    else
    {
        if (_bl_enabled)
        {
            _bl_mesh->DeleteVertexBufferObjects();
            delete _bl_mesh;
            _patch.DeleteBL();
            _bl_enabled = false;
            repaint();
        }
    }
}

void GLWidget::toggle_l(bool checked)
{
    if (checked)
    {
        DCoordinate3 d_00, d_01, d_02, d_03,
                d_10, d_11, d_12, d_13,
                d_20, d_21, d_22, d_23,
                d_30, d_31, d_32, d_33;

        _patch.GetData(0, 0, d_01);
        _patch.GetData(0, 2, d_03);
        _patch.GetData(2, 0, d_21);
        _patch.GetData(2, 2, d_23);

        _patch.GetData(1, 0, d_11);
        _patch.GetData(1, 2, d_13);
        _patch.GetData(3, 0, d_31);
        _patch.GetData(3, 2, d_33);

        if (_bl_enabled)
        {
            HermitePatch *bl = _patch.GetBL();
            bl->GetData(0, 0, d_10);
            bl->GetData(0, 2, d_12);
            bl->GetData(2, 0, d_30);
            bl->GetData(2, 2, d_32);
        }

        if (_tl_enabled)
        {
            HermitePatch *tl = _patch.GetTL();
            tl->GetData(1, 0, d_00);
            tl->GetData(1, 2, d_02);
            tl->GetData(3, 0, d_20);
            tl->GetData(3, 2, d_22);
        }


        ExtendDialog *dialog = new ExtendDialog(
                    d_00, d_01, d_02, d_03,
                    d_10, d_11, d_12, d_13,
                    d_20, d_21, d_22, d_23,
                    d_30, d_31, d_32, d_33);

        dialog->setup_l(_bl_enabled, _tl_enabled);

        if (dialog->exec() == QDialog::Accepted)
        {
            Matrix<DCoordinate3> m = dialog->getData();
            _patch.ExtendL(m);

            HermitePatch* l = _patch.GetL();

            l->UpdateVertexBufferObjectsOfDerivatives();
            l->GenerateUIsoLines(_u_isoline_count, 30);
            l->GenerateVIsoLines(_v_isoline_count, 30);
            _l_mesh = l->GenerateImage(30, 30, GL_STATIC_DRAW);
            _l_mesh->UpdateVertexBufferObjects();
            _l_enabled = true;
            repaint();
        }
        else
        {
            dynamic_cast<QToolButton*>(sender())->setChecked(false);
        }
    }
    else
    {
        if (_l_enabled)
        {
            _l_mesh->DeleteVertexBufferObjects();
            delete _l_mesh;
            _patch.DeleteL();
            _l_enabled = false;
            repaint();
        }
    }
}


void GLWidget::toggle_iso_u(bool checked)
{
    _show_u_iso_lines = checked;
    repaint();
}

void GLWidget::toggle_iso_v(bool checked)
{
    _show_v_iso_lines = checked;
    repaint();
}

void GLWidget::set_iso_u_div_count(int value)
{
    _u_isoline_count = value;
    _patch.GenerateUIsoLines(_u_isoline_count, 30);
    if (_tl_enabled)
        _patch.GetTL()->GenerateUIsoLines(_u_isoline_count, 30);
    if (_t_enabled)
        _patch.GetT()->GenerateUIsoLines(_u_isoline_count, 30);
    if (_tr_enabled)
        _patch.GetTR()->GenerateUIsoLines(_u_isoline_count, 30);
    if (_r_enabled)
        _patch.GetR()->GenerateUIsoLines(_u_isoline_count, 30);

    if (_br_enabled)
        _patch.GetBR()->GenerateUIsoLines(_u_isoline_count, 30);
    if (_b_enabled)
        _patch.GetB()->GenerateUIsoLines(_u_isoline_count, 30);
    if (_bl_enabled)
        _patch.GetBL()->GenerateUIsoLines(_u_isoline_count, 30);
    if (_l_enabled)
        _patch.GetL()->GenerateUIsoLines(_u_isoline_count, 30);
    repaint();
}

void GLWidget::set_iso_v_div_count(int value)
{
    _v_isoline_count = value;
    _patch.GenerateVIsoLines(_v_isoline_count, 30);
    if (_tl_enabled)
        _patch.GetTL()->GenerateVIsoLines(_v_isoline_count, 30);
    if (_t_enabled)
        _patch.GetT()->GenerateVIsoLines(_v_isoline_count, 30);
    if (_tr_enabled)
        _patch.GetTR()->GenerateVIsoLines(_v_isoline_count, 30);
    if (_r_enabled)
        _patch.GetR()->GenerateVIsoLines(_v_isoline_count, 30);

    if (_br_enabled)
        _patch.GetBR()->GenerateVIsoLines(_v_isoline_count, 30);
    if (_b_enabled)
        _patch.GetB()->GenerateVIsoLines(_v_isoline_count, 30);
    if (_bl_enabled)
        _patch.GetBL()->GenerateVIsoLines(_v_isoline_count, 30);
    if (_l_enabled)
        _patch.GetL()->GenerateVIsoLines(_v_isoline_count, 30);
    repaint();
}

void GLWidget::toggle_iso_u_der(bool checked)
{
    _show_u_iso_derivates = checked;
    repaint();
}

void GLWidget::toggle_iso_v_der(bool checked)
{
    _show_v_iso_derivates = checked;
    repaint();
}

void GLWidget::toggle_surface(bool checked)
{
    _show_surface = checked;
    repaint();
}

void GLWidget::set_shader(int index)
{
    switch (index)
    {
    case 0:
        _shader.InstallShaders("Shaders/directional_light.vert",
                               "Shaders/directional_light.frag",
                               GL_TRUE);
        break;
    case 1:
        _shader.InstallShaders("Shaders/toon.vert",
                               "Shaders/toon.frag",
                               GL_TRUE);
        _shader.SetUniformVariable4f("default_outline_color", 1, 0, 0, 1);

        break;
    case 2:
        _shader.InstallShaders("Shaders/two_sided_lighting.vert",
                               "Shaders/two_sided_lighting.frag",
                               GL_TRUE);
        break;
    }
    _shader.Enable(GL_TRUE);
    repaint();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    _zoom *= event->delta() < 0 ? 0.9 : 1.1;
    repaint();
}
