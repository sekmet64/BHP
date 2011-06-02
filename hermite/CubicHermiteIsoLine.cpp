#include "hermite/CubicHermiteIsoLine.h"
#include <iostream>

using namespace cagd;
using namespace std;

CubicHermiteIsoLine::CubicHermiteIsoLine(GLenum usage_flag): LinearCombination3(0.0,1.0,4,usage_flag)
{
}

GLboolean CubicHermiteIsoLine::BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble> &values)const
{
    if (u<0.0 || u>1.0)
        return GL_FALSE;

    values.ResizeColumns(4);
    GLdouble u2=u*u, u3=u2*u;
    values(0) = 2*u3 - 3*u2 + 1;
    values(1) = -2*u3 + 3*u2;
    values(2) = u3 - 2*u2 + u;
    values(3) = u3 - u2;

    return GL_TRUE;
}

GLboolean CubicHermiteIsoLine::CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const
{
    if (u<0.0 || u>1.0)
        return GL_FALSE;

    if (max_order_of_derivatives<0 || max_order_of_derivatives>3)
        return GL_FALSE;

    d.ResizeRows(max_order_of_derivatives + 1);

    DCoordinate3 sum;
    GLdouble val[4];
    GLdouble u2=u*u, u3=u2*u;
    val[0] = 2*u3 - 3*u2 + 1;
    val[1] = -2*u3 + 3*u2;
    val[2] = u3 - 2*u2 + u;
    val[3] = u3 - u2;
    for (GLuint j=0; j<4; j++)
        sum += _data[j]*val[j];
    d(0) = sum;

    if (max_order_of_derivatives > 0)
    {
        sum[0] = sum[1] = sum[2] = 0.0;
        val[0] = 6*u2 - 6*u;
        val[1] = -6*u2 + 6*u;
        val[2] = 3*u2 -4*u +1;
        val[3] = 3*u2-2*u;
        for (GLuint j=0; j<4; j++)
            sum += _data[j]*val[j];
        d(1) = sum;
    }

    if (max_order_of_derivatives > 1)
    {
        sum[0] = sum[1] = sum[2] = 0.0;
        val[0] = 12*u - 6;
        val[1] = -12*u+6;
        val[2] = 6*u-4;
        val[3] = 6*u-2;
        for (GLuint j=0; j<4; j++)
            sum += _data[j]*val[j];
        d(2) = sum;
    }

    if (max_order_of_derivatives == 3)
    {
        sum[0] = sum[1] = sum[2] = 0.0;
        val[0] = 12;
        val[1] = -12;
        val[2] = 6;
        val[3] = 6;
        for (GLuint j=0; j<4; j++)
            sum += _data[j]*val[j];
        d(3) = sum;
    }

    return GL_TRUE;
}


