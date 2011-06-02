#pragma once

#include "../core/TensorProductSurfaces3.h"
#include "../core/GenericCurves3.h"
#include <vector>

namespace cagd
{
    class HermitePatch: public TensorProductSurface3
    {
    private:
        GLuint  _vbo_derivatives;
        GLuint  _ibo_derivatives;

		//iso-lines
        std::vector<cagd::GenericCurve3*> u_iso_lines, v_iso_lines;
        GLuint u_iso_lines_count, v_iso_lines_count;
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
		
		GenericCurve3* GenerateVIsoLine(GLdouble u_t, GLuint div_point_count, GLuint max_order_of_derivates=2, GLenum usage_flag = GL_STATIC_DRAW);
        GenericCurve3* GenerateUIsoLine(GLdouble v_t, GLuint div_point_count, GLuint max_order_of_derivates=2, GLenum usage_flag = GL_STATIC_DRAW);





        GLvoid GenerateUIsoLines(GLuint line_count,GLuint div_point_count, GLuint max_order_of_derivates=2, GLenum usage_flag = GL_STATIC_DRAW);
        GLvoid GenerateVIsoLines(GLuint line_count,GLuint div_point_count, GLuint max_order_of_derivates=2, GLenum usage_flag = GL_STATIC_DRAW);
        GLvoid RenderUIsoLines(GLuint order);
        GLvoid RenderVIsoLines(GLuint order);


        ~HermitePatch();
    };
}
