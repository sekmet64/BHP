#pragma once

#include "../core/TensorProductSurfaces3.h"

namespace cagd
{
    class HermitePatch: public TensorProductSurface3
    {
    private:
        GLuint  _vbo_derivative_u;
        //GLuint  _vbo_derivative_v;
        //GLuint  _vbo_derivative_t;
    public:
        HermitePatch();

        GLboolean UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const;
        GLboolean VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const;
        GLboolean CalculatePartialDerivatives(GLdouble u, GLdouble v, PartialDerivatives& pd) const;

        GLvoid    DeleteVertexBufferObjectsOfData();
        GLboolean RenderData(GLenum render_mode = GL_LINE_STRIP) const;
        GLboolean UpdateVertexBufferObjectsOfData(GLenum usage_flag = GL_STATIC_DRAW) ;

        GLvoid    DeleteVertexBufferObjectsOfDerivatives();
        GLboolean UpdateVertexBufferObjectsOfDerivatives();
        GLboolean RenderDerivatives();

        ~HermitePatch();
    };
}
