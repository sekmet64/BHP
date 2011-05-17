#include "Hermite.h"

using namespace cagd;

HermitePatch::HermitePatch() : TensorProductSurface3(0.0, 1.0, 0.0, 1.0, 4, 4)
{
}

GLboolean HermitePatch::UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble> &blending_values) const
{
    if (u_knot<0.0 || u_knot>1.0)
        return GL_FALSE;
    blending_values.ResizeColumns(4);

    GLdouble u=u_knot, u2=u*u, u3=u2*u;
    blending_values(0) = 2*u3 - 3*u2 + 1;
    blending_values(1) = -2*u3 + 3*u2;
    blending_values(2) = u3 - 2*u2 + u;
    blending_values(3) = u3 - u2;

    return GL_TRUE;
}

GLboolean HermitePatch::VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble> &blending_values) const
{
    if (v_knot<0.0 || v_knot>1.0)
        return GL_FALSE;
    blending_values.ResizeColumns(4);

    GLdouble v=v_knot, v2=v*v, v3=v2*v;
    blending_values(0) = 2*v3 - 3*v2 + 1;
    blending_values(1) = -2*v3 + 3*v2;
    blending_values(2) = v3 - 2*v2 + v;
    blending_values(3) = v3 - v2;

    return GL_TRUE;
}

GLboolean HermitePatch::CalculatePartialDerivatives(GLdouble u, GLdouble v, PartialDerivatives &pd) const
{
   if (u<0.0 || u>1.0 || v<0.0 || v>1.0)
       return GL_FALSE;

   RowMatrix<GLdouble> u_blending_values(4), d1_u_blending_values(4);

   GLdouble u2=u*u, u3=u2*u;

   u_blending_values(0) = 2*u3 - 3*u2 + 1;
   u_blending_values(1) = -2*u3 + 3*u2;
   u_blending_values(2) = u3 - 2*u2 + u;
   u_blending_values(3) = u3 - u2;

   d1_u_blending_values(0) = 6*u2 - 6*u;
   d1_u_blending_values(1) = -6*u2 + 6*u;
   d1_u_blending_values(2) = 3*u2 -4*u + 1;
   d1_u_blending_values(3) = 3*u2 - 2*u;

   RowMatrix<GLdouble> v_blending_values(4), d1_v_blending_values(4);

   GLdouble v2=v*v, v3=v2*v;

   v_blending_values(0) = 2*v3 - 3*v2 + 1;
   v_blending_values(1) = -2*v3 + 3*v2;
   v_blending_values(2) = v3 - 2*v2 + v;
   v_blending_values(3) = v3 - v2;

   d1_v_blending_values(0) = 6*v2 - 6*v;
   d1_v_blending_values(1) = -6*v2 + 6*v;
   d1_v_blending_values(2) = 3*v2 -4*v + 1;
   d1_v_blending_values(3) = 3*v2 - 2*v;

   pd.LoadNullVectors();
   for (GLuint row=0; row<4; ++row)
   {
       DCoordinate3 aux_d0_v, aux_d1_v;
       for (GLuint column = 0; column < 4; ++column)
       {
           aux_d0_v += _data(row, column) * v_blending_values(column);
           aux_d1_v += _data(row, column) * d1_v_blending_values(column);
       }
       pd.point += aux_d0_v * u_blending_values(row);
       pd.diff1u += aux_d0_v * d1_u_blending_values(row);
       pd.diff1v += aux_d1_v * u_blending_values(row);
   }

    return GL_TRUE;
}

GLvoid    HermitePatch::DeleteVertexBufferObjectsOfData()
{
    if (_vbo_data)
    {
        glDeleteBuffers(1, &_vbo_data);
        _vbo_data = 0;
    }
}

GLboolean HermitePatch::RenderData(GLenum render_mode) const
{
    return GL_TRUE;
}

GLboolean HermitePatch::UpdateVertexBufferObjectsOfData(GLenum usage_flag)
{
    return GL_TRUE;
}

GLvoid HermitePatch::DeleteVertexBufferObjectsOfDerivatives()
{
    if (_vbo_derivatives)
    {
        glDeleteBuffers(1, &_vbo_derivatives);
        _vbo_derivatives = 0;
    }

    if (_ibo_derivatives)
    {
        glDeleteBuffers(1, &_ibo_derivatives);
        _ibo_derivatives = 0;
    }
}

GLboolean HermitePatch::UpdateVertexBufferObjectsOfDerivatives()
{
    DeleteVertexBufferObjectsOfDerivatives();

    glGenBuffers(1, &_vbo_derivatives);

    if (!_vbo_derivatives)
    {
        DeleteVertexBufferObjectsOfDerivatives();
        return GL_FALSE;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivatives);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16 * 3, 0, GL_STATIC_DRAW);

    GLfloat *coordinate = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if (!coordinate)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        DeleteVertexBufferObjectsOfDerivatives();
        return GL_FALSE;
    }

    // corner points
    push3f(coordinate, _data(0, 0));
    push3f(coordinate, _data(0, 1));
    push3f(coordinate, _data(1, 0));
    push3f(coordinate, _data(1, 1));


    // first order derivatives in v direction
    push3f(coordinate, _data(0, 0) + _data(0, 2));
    push3f(coordinate, _data(0, 1) + _data(0, 3));
    push3f(coordinate, _data(1, 0) + _data(1, 2));
    push3f(coordinate, _data(1, 1) + _data(1, 3));

    // first order derivatives in u direction
    push3f(coordinate, _data(0, 0) + _data(2, 0));
    push3f(coordinate, _data(0, 1) + _data(2, 1));
    push3f(coordinate, _data(1, 0) + _data(3, 0));
    push3f(coordinate, _data(1, 1) + _data(3, 1));

    // twist vectors
    push3f(coordinate, _data(0, 0) + _data(2, 2));
    push3f(coordinate, _data(0, 1) + _data(2, 3));
    push3f(coordinate, _data(1, 0) + _data(3, 2));
    push3f(coordinate, _data(1, 1) + _data(3, 3));


    if (!glUnmapBuffer(GL_ARRAY_BUFFER))
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        DeleteVertexBufferObjectsOfDerivatives();
        return GL_FALSE;
    }

    // index buffer

    glGenBuffers(1, &_ibo_derivatives);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_derivatives);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 24, 0,
        GL_STATIC_DRAW);

    GLushort *ind = (GLushort*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

    // v
    push1us(ind, 0);
    push1us(ind, 4);

    push1us(ind, 1);
    push1us(ind, 5);

    push1us(ind, 2);
    push1us(ind, 6);

    push1us(ind, 3);
    push1us(ind, 7);

    // u
    push1us(ind, 0);
    push1us(ind, 8);

    push1us(ind, 1);
    push1us(ind, 9);

    push1us(ind, 2);
    push1us(ind, 10);

    push1us(ind, 3);
    push1us(ind, 11);

    // t
    push1us(ind, 0);
    push1us(ind, 12);

    push1us(ind, 1);
    push1us(ind, 13);

    push1us(ind, 2);
    push1us(ind, 14);

    push1us(ind, 3);
    push1us(ind, 15);

    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    return GL_TRUE;
}

GLboolean HermitePatch::RenderDerivatives()
{

//GL_COLOR_ARRAY


    glBegin(GL_POINTS);
        glColor3f(0.94f, 0.16f, 0.16f);
//        glPointSize(7.0f);

//        DCoordinate3 c = _data(0, 0) + _data(0, 2);
//        glVertex3f(c.x(), c.y(), c.z());

    glEnd();

    glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivatives);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_derivatives);
            glVertexPointer(3, GL_FLOAT, 0, (const GLvoid *)0);

            glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);


    glBegin(GL_POINTS);
        glColor3f(0.45f, 0.82f, 0.09f);
    glEnd();

    glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivatives);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_derivatives);
            glVertexPointer(3, GL_FLOAT, 0, (const GLvoid *)0);

            glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (char *)NULL + 8 * sizeof(GLushort));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);

    glBegin(GL_POINTS);
        glColor3f(1.0f, 1.0f, 1.0f);
    glEnd();

    glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo_derivatives);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo_derivatives);
            glVertexPointer(3, GL_FLOAT, 0, (const GLvoid *)0);

            glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (char *)NULL + 16 * sizeof(GLushort));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
	
    return GL_TRUE;
}


HermitePatch::~HermitePatch()
{
    //DeleteVertexBufferObjectsOfData();
    DeleteVertexBufferObjectsOfDerivatives();
}

void HermitePatch::push3f(GLfloat *&coordinate, const DCoordinate3 &data)
{
    coordinate[0] = data.x();
    coordinate[1] = data.y();
    coordinate[2] = data.z();
    coordinate += 3;
}

void HermitePatch::push1us(GLushort *&coordinate, const GLushort data)
{
    *coordinate = data;
    ++coordinate;
}
