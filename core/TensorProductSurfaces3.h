#pragma once

#include "DCoordinates3.h"
#include <GL/glew.h>
#include <iostream>
#include "Matrices.h"
#include "TriangulatedMeshes3.h"
#include <vector>

namespace cagd
{
    class TensorProductSurface3
    {
    public:
        class PartialDerivatives
        {
        public:
            // point of surface
            DCoordinate3 point;

            // first ordered partial derivates of the surface
            DCoordinate3 diff1u, diff1v;

            // second ordered partial derivates of the surface
            //DCoordinate3 diff2uu, diff2uv, diff2vv;

            // third ordered partial derivates of the surface
            //DCoordinate3 diff3uuu, diff3uuv, diff3uvv, diff3vvv;

           // initializes all information above to null-vector
            GLvoid LoadNullVectors();
        };

    protected:
        GLboolean            _is_closed;
        GLuint               _vbo_data;
        GLdouble             _u_min, _u_max;
        GLdouble             _v_min, _v_max;
        Matrix<DCoordinate3> _data;

    public:
        //special constructor
        TensorProductSurface3(
                GLdouble u_min, GLdouble u_max,
                GLdouble v_min, GLdouble v_max,
                GLuint row_count = 4, GLuint column_count = 4,
                GLboolean is_closed = GL_FALSE);

        // copy constructor
        TensorProductSurface3(const TensorProductSurface3& surface);

        // assignment operator
        TensorProductSurface3& operator =(const TensorProductSurface3& surface);

        // set/get the definition domain of the surface
        GLvoid SetUInterval(GLdouble u_min, GLdouble u_max);
        GLvoid SetVInterval(GLdouble v_min, GLdouble v_max);

        GLvoid GetUInterval(GLdouble& u_min, GLdouble& u_max) const;
        GLvoid GetVInterval(GLdouble& v_min, GLdouble& v_max) const;

        // set coordinates of a selected data point
        GLboolean SetData(GLuint row, GLuint column, GLdouble x, GLdouble y, GLdouble z);
        GLboolean SetData(GLuint row, GLuint column, const DCoordinate3& point);

        // get coordinates of a selected data point
        GLboolean GetData(
                GLuint row, GLuint column,
                GLdouble& x, GLdouble& y, GLdouble& z) const;
        GLboolean GetData(GLuint row, GLuint column, DCoordinate3& point) const;

        // get data by value
        DCoordinate3 operator ()(GLuint row, GLuint column) const;

        // get data by reference
        DCoordinate3& operator ()(GLuint row, GLuint column);

        // blending function values in directions u and v
        virtual GLboolean UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const = 0;
        virtual GLboolean VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const = 0;

        // calculates the point and higher (mixed) ordered partial derivatives of the
        // tensor product surface
        //
        // s(u, v) = \sum_{i=0}^{row_count} \sum{j = 0}^{column_count} p_{ij} F_i(u) G_j(v),
        //
        // where(u, v) \in [u_{\min}, u_{\max}] \times [v_{\min}, v_{\max}]
        virtual GLboolean CalculatePartialDerivatives(GLdouble u, GLdouble v, PartialDerivatives& pd) const = 0;

        // generates a triangulated mesh that approximates the shape of the surface above
        virtual TriangulatedMesh3* GenerateImage(GLuint u_div_point_count, GLuint v_div_point_count, GLenum usage_flag = GL_STATIC_DRAW) const;

        // ensures interpolation, i.e.
        //
        // s(u_i, v_j) = d_{ij}, i = 0,1,...,row_count, j = 0,1,...,column_count
        GLboolean UpdateDataForInterpolation(const RowMatrix<GLdouble>& u_knot_vector, const ColumnMatrix<GLdouble>& v_knot_vector, Matrix<DCoordinate3>& data_points_to_interpolate);

        // VBO handling methods (assume that Matrix<DCoordinate3> _data corresponds to a control net!)
        virtual GLvoid    DeleteVertexBufferObjectsOfData();
        virtual GLboolean RenderData(GLenum render_mode = GL_LINE_STRIP) const;
        virtual GLboolean UpdateVertexBufferObjectsOfData(GLenum usage_flag = GL_STATIC_DRAW);

        // destructor
        virtual ~TensorProductSurface3();
    };
}
