#pragma once

#include "../core/LinearCombination3.h"

namespace cagd{
    class CubicHermiteIsoLine :public LinearCombination3
    {
    public:
        CubicHermiteIsoLine(GLenum usage_flag = GL_STATIC_DRAW);
        GLboolean BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble>& values) const;
        GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives& d) const;
    };
}
