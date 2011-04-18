#pragma once

#include <cmath>
#include <GL/glew.h>
#include <iostream>

namespace cagd
{
    //--------------------------------------
    // 3-dimensional homogeneous coordinates
    //--------------------------------------
    class HCoordinate3
    {
    protected:
        GLfloat _data[4]; // x, y, z, w;

    public:
        // default constructor
        HCoordinate3();

        // special constructor
        HCoordinate3(GLfloat x, GLfloat y, GLfloat z = 0.0, GLfloat w = 1.0);

        // get components by value
        const GLfloat operator[](GLuint rhs) const;
        const GLfloat x() const;
        const GLfloat y() const;
        const GLfloat z() const;
        const GLfloat w() const;

        // get components by reference
        GLfloat& operator[](GLuint rhs);
        GLfloat& x();
        GLfloat& y();
        GLfloat& z();
        GLfloat& w();

        // add
        const HCoordinate3 operator +(const HCoordinate3& rhs) const;

        // add to this
        HCoordinate3& operator +=(const HCoordinate3& rhs);

        // subtract
        const HCoordinate3 operator -(const HCoordinate3& rhs) const;

        //subtract from this
        HCoordinate3& operator -=(const HCoordinate3& rhs);

        // dot product
        const GLfloat operator *(const HCoordinate3& rhs) const;

        // cross product
        const HCoordinate3 operator ^(const HCoordinate3& rhs) const;

        // cross product with this
        HCoordinate3& operator ^=(const HCoordinate3& rhs);

        // multiplicate with scalar from right
        const HCoordinate3 operator *(GLfloat rhs) const;

        // multiplicate this with a scalar
        HCoordinate3& operator *=(GLfloat rhs);

        // divide with scalar
        const HCoordinate3 operator /(GLfloat rhs) const;

        // divide this with a scalar
        HCoordinate3& operator /=(GLfloat rhs);

        // length of vector represented by this homogeneous coordinate
        const GLfloat length() const;

        // normalize
        HCoordinate3& normalize();
    };

    // default constructor
    inline HCoordinate3::HCoordinate3()
    {
        _data[0] = _data[1] = _data[2] = 0.0;
        _data[3] = 1.0;
    }

    // special constructor
    inline HCoordinate3::HCoordinate3(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
    {
        _data[0] = x;
        _data[1] = y;
        _data[2] = z;
        _data[3] = w;
    }

    // get components by value
    inline const GLfloat HCoordinate3::operator[](GLuint rhs) const
    {
        return _data[rhs];
    }

    inline const GLfloat HCoordinate3::x() const
    {
        return _data[0];
    }

    inline const GLfloat HCoordinate3::y() const
    {
        return _data[1];
    }

    inline const GLfloat HCoordinate3::z() const
    {
        return _data[2];
    }

    inline const GLfloat HCoordinate3::w() const
    {
        return _data[3];
    }

    // get components by reference
    inline GLfloat& HCoordinate3::operator[](GLuint rhs)
    {
        return _data[rhs];
    }

    inline GLfloat& HCoordinate3::x()
    {
        return _data[0];
    }

    inline GLfloat& HCoordinate3::y()
    {
        return _data[1];
    }

    inline GLfloat& HCoordinate3::z()
    {
        return _data[2];
    }

    inline GLfloat& HCoordinate3::w()
    {
        return _data[3];
    }

    // add
    inline const HCoordinate3 HCoordinate3::operator +(const HCoordinate3& rhs) const
    {
        return HCoordinate3(
                rhs.w() * x() + w() * rhs.x(),
                rhs.w() * y() + w() * rhs.y(),
                rhs.w() * z() + w() * rhs.z(),
                w() * rhs.w());
    }

    // add to this
    inline HCoordinate3& HCoordinate3::operator +=(const HCoordinate3& rhs)
    {
        _data[0]=rhs.w() * x() + w() * rhs.x();
        _data[1]=rhs.w() * y() + w() * rhs.y();
        _data[2]=rhs.w() * z() + w() * rhs.z();
        _data[3]=w() * rhs.w();
        return *this;
    }

    // subtract
    inline const HCoordinate3 HCoordinate3::operator -(const HCoordinate3& rhs) const
    {
        return HCoordinate3(
                rhs.w() * x() - w() * rhs.x(),
                rhs.w() * y() - w() * rhs.y(),
                rhs.w() * z() - w() * rhs.z(),
                w() * rhs.w());
    }

    //subtract from this
    inline HCoordinate3& HCoordinate3::operator -=(const HCoordinate3& rhs)
    {
        _data[0]=rhs.w() * x() - w() * rhs.x();
        _data[1]=rhs.w() * y() - w() * rhs.y();
        _data[2]=rhs.w() * z() - w() * rhs.z();
        _data[3]=w() * rhs.w();
        return *this;
    }

    // dot product
    inline const GLfloat HCoordinate3::operator *(const HCoordinate3& rhs) const
    {
        if ((w()==0) || (rhs.w()==0))
              return 1000000;
        return (x()*rhs.x()+y()*rhs.y()+z()*rhs.z())/(w()*rhs.w());
    }

    // cross product
    inline const HCoordinate3 HCoordinate3::operator ^(const HCoordinate3& rhs) const
    {
        return HCoordinate3(y()*rhs.z()-z()*rhs.y(),z()*rhs.x()-x()*rhs.z(),x()*rhs.y()-y()*rhs.x(),w()*rhs.w());
    }

    // cross product with this
    inline HCoordinate3& HCoordinate3::operator ^=(const HCoordinate3& rhs)
    {
        GLfloat _x=x(),_y=y(),_z=z();
        _data[0]= _y*rhs.z()-_z*rhs.y();
        _data[1]= _z*rhs.x()- _x*rhs.z();
        _data[2]= _x*rhs.y()- _y*rhs.x();
        _data[3]=w() * rhs.w();
        return *this;
    }

    // multiplicate with scalar from right
    inline const HCoordinate3 HCoordinate3::operator *(GLfloat rhs) const
    {
        return HCoordinate3(x()*rhs,y()*rhs,z()*rhs,w());
    }

    // multiplicate this with a scalar
    inline HCoordinate3& HCoordinate3::operator *=(GLfloat rhs)
    {
        _data[0] *= rhs;
        _data[1] *= rhs;
        _data[2] *= rhs;
        return *this;
    }

    // divide with scalar
    inline const HCoordinate3 HCoordinate3::operator /(GLfloat rhs) const
    {
        return HCoordinate3(x()/rhs,y()/rhs,z()/rhs,w());
    }

    // divide this with a scalar
    inline HCoordinate3& HCoordinate3::operator /=(GLfloat rhs)
    {
        _data[0] /= rhs;
        _data[1] /= rhs;
        _data[2] /= rhs;
        return *this;
    }

    // length of vector represented by this homogeneous coordinate
    inline const GLfloat HCoordinate3::length() const
    {
        return std::sqrt((*this) * (*this));
    }

    // normalize
    inline HCoordinate3& HCoordinate3::normalize()
    {
        GLfloat l = length();

        if (l && l != 1.0)
            *this /= l;

        return *this;
    }

    // scale from left with a scalar
    inline const HCoordinate3 operator *(GLfloat lhs, const HCoordinate3& rhs)
    {
        return HCoordinate3(lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z(), rhs.w());
    }

    // output to stream
    inline std::ostream& operator <<(std::ostream& lhs, const HCoordinate3& rhs)
    {
        return lhs<<rhs.x()<<" "<<rhs.y()<<" "<<rhs.z()<< " "<<rhs.w();
    }

    // input from stream
    inline std::istream& operator >>(std::istream& lhs, HCoordinate3& rhs)
    {
        return lhs>>rhs.x()>>rhs.y()>>rhs.z()>>rhs.w();
    }
}
