#pragma once

#include <GL/glew.h>

namespace cagd
{
    class Color4
    {
    protected:
        GLfloat _data[4]; // (r, g, b, a)

    public:
        Color4();

        Color4(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f);

        //get components by value
        const GLfloat operator [](GLuint rhs) const;
        const GLfloat r() const;
        const GLfloat g() const;
        const GLfloat b() const;
        const GLfloat a() const;

        //get components by reference
        GLfloat& operator [](GLuint rhs);
        GLfloat& r();
        GLfloat& g();
        GLfloat& b();
        GLfloat& a();
    };

    inline Color4::Color4()
    {
        _data[0] = _data[1] = _data[2] = 0.0;
        _data[3] = 1.0;
    }

    inline Color4::Color4(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {
        _data[0] = r;
        _data[1] = g;
        _data[2] = b;
        _data[3] = a;
    }

    //get components by value
    inline const GLfloat Color4::operator [](GLuint rhs) const
    {
        return _data[rhs];
    }

    inline const GLfloat Color4::r() const
    {
        return _data[0];
    }

    inline const GLfloat Color4::g() const
    {
        return _data[1];
    }

    inline const GLfloat Color4::b() const
    {
        return _data[2];
    }

    inline const GLfloat Color4::a() const
    {
        return _data[3];
    }

    //get components by reference
    inline GLfloat& Color4::operator [](GLuint rhs)
    {
        return _data[rhs];
    }

    inline GLfloat& Color4::r()
    {
        return _data[0];
    }
    inline GLfloat& Color4::g()
    {
        return _data[1];
    }

    inline GLfloat& Color4::b()
    {
        return _data[2];
    }

    inline GLfloat& Color4::a()
    {
       return  _data[3];
    }
}
