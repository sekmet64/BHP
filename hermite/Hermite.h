#pragma once

#include "../core/TensorProductSurfaces3.h"

namespace cagd
{
    class HermitePatch: public TensorProductSurface3
    {
    private:
        GLuint  _vbo_derivatives;
        GLuint  _ibo_derivatives;

        // copy 3 float to a pointer location
        void    push3f(GLfloat *&coordinate, const DCoordinate3 &data);

        // copy 1 unsigned short int to a pointer location
        void    push1us(GLushort *&coordinate, const GLushort data);

        HermitePatch *_tl;
        HermitePatch *_t;
        HermitePatch *_tr;
        HermitePatch *_r;

        HermitePatch *_br;
        HermitePatch *_b;
        HermitePatch *_bl;
        HermitePatch *_l;
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

        HermitePatch* GetTL();
        HermitePatch* GetT();
        HermitePatch* GetTR();
        HermitePatch* GetR();

        HermitePatch* GetBR();
        HermitePatch* GetB();
        HermitePatch* GetBL();
        HermitePatch* GetL();

        void DeleteTL();
        void DeleteT();
        void DeleteTR();
        void DeleteR();

        void DeleteBR();
        void DeleteB();
        void DeleteBL();
        void DeleteL();


        void    ExtendTL(Matrix<DCoordinate3> data);
        void    ExtendT(Matrix<DCoordinate3> data);
        void    ExtendTR(Matrix<DCoordinate3> data);
        void    ExtendR(Matrix<DCoordinate3> data);

        void    ExtendBR(Matrix<DCoordinate3> data);
        void    ExtendB(Matrix<DCoordinate3> data);
        void    ExtendBL(Matrix<DCoordinate3> data);
        void    ExtendL(Matrix<DCoordinate3> data);

        ~HermitePatch();
    };
}
