#pragma once

#include <iostream>
#include <GL/glew.h>
#include <vector>

namespace cagd
{
    // forward declaration of template class Matrix
    template <typename T>
    class Matrix;

    // forward declaration of template class RowMatrix
    template <typename T>
    class RowMatrix;

    // forward declaration of template class ColumnMatrix
    template <typename T>
    class ColumnMatrix;

    // forward declarations of overloaded and templated input/output from/to stream operators
    template <typename T>
    std::ostream& operator << (std::ostream& lhs, const Matrix<T>& rhs);

    template <typename T>
    std::istream& operator >>(std::istream& lhs, Matrix<T>& rhs);

    //----------------------
    // template class Matrix
    //----------------------
    template <typename T>
    class Matrix
    {
        friend std::ostream& cagd::operator << <T>(std::ostream&, const Matrix<T>& rhs);
        friend std::istream& cagd::operator >> <T>(std::istream&, Matrix<T>& rhs);

    protected:
        unsigned int                    _row_count;
        unsigned int                    _column_count;
        std::vector< std::vector<T> >   _data;
    public:
        // special constructor (can also be used as a default constructor)
        Matrix(unsigned int row_count = 1, unsigned int column_count = 1);

        // copy constructor
        Matrix(const Matrix& m);

        // assignment operator
        Matrix& operator =(const Matrix& m);

        // get element by reference
        T& operator ()(unsigned int row, unsigned int column);

        // get copy of an element
        T operator ()(unsigned int row, unsigned int column) const;

        // get dimensions
        unsigned int GetRowCount() const;
        unsigned int GetColumnCount() const;

        // set dimensions
        virtual GLboolean ResizeRows(unsigned int row_count);
        virtual GLboolean ResizeColumns(unsigned int column_count);

        // update
        GLboolean SetRow(unsigned int index, const RowMatrix<T>& row);
        GLboolean SetColumn(unsigned int index, const ColumnMatrix<T>& column);

        // destructor
        virtual ~Matrix();
    };

    //-------------------------
    // template class RowMatrix
    //-------------------------
    template <typename T>
    class RowMatrix: public Matrix<T>
    {
    public:
        // special constructor (can also be used as a default constructor)
        RowMatrix(unsigned int column_count = 1);

        // get element by reference
        T& operator ()(unsigned int column);
        T& operator [](unsigned int column);

        // get copy of an element
        T operator ()(unsigned int column) const;
        T operator [](unsigned int column) const;

        // a row matrix consists of a single row
        GLboolean ResizeRows(unsigned int row_count);
    };

    //----------------------------
    // template class ColumnMatrix
    //----------------------------
    template <typename T>
    class ColumnMatrix: public Matrix<T>
    {
    public:
        // special constructor (can also be used as a default constructor)
        ColumnMatrix(unsigned int row_count = 1);

        // get element by reference
        T& operator ()(unsigned int row);
        T& operator [](unsigned int row);

        // get copy of an element
        T operator ()(unsigned int row) const;
        T operator [](unsigned int row) const;

        // a column matrix consists of a single column
        GLboolean ResizeColumns(unsigned int column_count);
    };

    //--------------------------------------------------
    // implementation of template class Matrix
    //--------------------------------------------------

    template <typename T>
    Matrix<T>::Matrix(unsigned int row_count, unsigned int column_count):_row_count(row_count),
       _column_count(column_count)
    {
        _data.resize(_row_count);
        for (GLuint i=0;i<_row_count;++i)
            _data[i].resize(_column_count);
    }

    template <typename T>
    Matrix<T>::Matrix(const Matrix<T> &m):_row_count(m._row_count), _column_count(m._column_count),
            _data(m._data)
    {
    }

    template <typename T>
    Matrix<T>& Matrix<T>::operator =(const Matrix<T>& m)
    {
        if (this!=&m)
        {
            _row_count=m.GetRowCount();
            _column_count=m.GetColumnCount();
            _data=m._data;
        }
        return *this;
    }

    template <typename T>
    T& Matrix<T>::operator ()(unsigned int row, unsigned int column)
    {
        return _data[row][column];
    }

    template <typename T>
    T Matrix<T>::operator ()(unsigned int row, unsigned int column) const
    {
        return _data[row][column];
    }

    template <typename T>
    unsigned int Matrix<T>::GetRowCount() const
    {
        return _row_count;
    }

    template <typename T>
    unsigned int Matrix<T>::GetColumnCount() const
    {
        return _column_count;
    }

    template <typename T>
    GLboolean Matrix<T>::SetRow(unsigned int index, const RowMatrix<T>& row)
    {
        if ((index<0)||(index>=_row_count))
            return GL_FALSE;
        if (row.GetColumnCount()!=_column_count)
            return GL_FALSE;
        for (unsigned int i=0;i<_column_count;++i)
            _data[index][i]=row._data[0][i];
        return GL_TRUE;
    }

    template <typename T>
    GLboolean Matrix<T>::SetColumn(unsigned int index, const ColumnMatrix<T>& column)
    {
        if ((index<0)||(index>=_column_count))
            return GL_FALSE;
        if (column.GetRowCount()!=_row_count)
            return GL_FALSE;
        for (unsigned int i=0; i< _row_count; ++i)
            _data[i][index]=column._data[i][0];
        return GL_TRUE;
    }

    template <typename T>
    Matrix<T>::~Matrix()
    {
        _data.clear();
    }

    template <typename T>
    GLboolean Matrix<T>::ResizeRows(unsigned int row_count)
    {
        _data.resize(row_count);
        if (_row_count < row_count)
        {
            for (GLuint i = _row_count; i < row_count; ++i)
                _data[i].resize(_column_count);
        }
        _row_count = row_count;
        return GL_TRUE;
    }

    template <typename T>
    GLboolean Matrix<T>::ResizeColumns(unsigned int column_count)
    {
        for (GLuint i=0;i<_row_count;++i)
            _data[i].resize(column_count);
        _column_count = column_count;
        return GL_TRUE;
    }

    //-----------------------------------------------------
    // implementation of template class RowMatrix
    //-----------------------------------------------------

    template <typename T>
    RowMatrix<T>::RowMatrix(unsigned int column_count): Matrix<T>(1, column_count)
    {
    }

    template <typename T>
    T& RowMatrix<T>::operator ()(unsigned int column)
    {
        return this->_data[0][column];
    }

    template <typename T>
    T RowMatrix<T>::operator ()(unsigned int column) const
    {
        return this->_data[0][column];
    }

    template <typename T>
    T& RowMatrix<T>::operator [](unsigned int column)
    {
        return this->_data[0][column];
    }

    template <typename T>
    T RowMatrix<T>::operator [](unsigned int column) const
    {
        return this->_data[0][column];
    }

    template <typename T>
    GLboolean RowMatrix<T>::ResizeRows(unsigned int row_count)
    {
        return GL_FALSE;
    }

    //--------------------------------------------------------
    // implementation of template class ColumnMatrix
    //--------------------------------------------------------

    template <typename T>
    ColumnMatrix<T>::ColumnMatrix(unsigned int row_count):
    Matrix<T>(row_count, 1)
    {
    }

    template <typename T>
    T& ColumnMatrix<T>::operator ()(unsigned int row)
    {
        return this->_data[row][0];
    }

    template <typename T>
    T ColumnMatrix<T>::operator ()(unsigned int row) const
    {
        return this->_data[row][0];
    }

    template <typename T>
    T& ColumnMatrix<T>::operator [](unsigned int row)
    {
        return this->_data[row][0];
    }

    template <typename T>
    T ColumnMatrix<T>::operator [](unsigned int row) const
    {
        return this->_data[row][0];
    }

    template <typename T>
    GLboolean ColumnMatrix<T>::ResizeColumns(unsigned int column_count)
    {
        return GL_FALSE;
    }

    //------------------------------------------------------------------------------
    // definitions of overloaded and templated input/output from/to stream operators
    //------------------------------------------------------------------------------

    // output to stream
    template <typename T>
    std::ostream& operator <<(std::ostream& lhs, const Matrix<T>& rhs)
    {
        lhs << rhs._row_count << " " << rhs._column_count << std::endl;
        for (typename std::vector< std::vector<T> >::const_iterator row = rhs._data.begin();
             row != rhs._data.end(); ++row)
        {
            for (typename std::vector<T>::const_iterator column = row->begin();
                 column != row->end(); ++column)
                    lhs << *column << " ";
            lhs << std::endl;
        }
        return lhs;
    }

    // input from stream
    template <typename T>
    std::istream& operator >>(std::istream& lhs, Matrix<T>& rhs)
    {
        lhs >> rhs._row_count >> rhs._column_count;
        for (unsigned int i=0; i<rhs._row_count; ++i)
            for (unsigned int j=0; i<rhs._column_count; ++j)
                    lhs >> rhs._data[i][j];
        return lhs;
    }
}
