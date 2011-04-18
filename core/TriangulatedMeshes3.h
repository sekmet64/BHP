#pragma once

#include "DCoordinates3.h"
#include <GL/glew.h>
#include <iostream>
#include <string>
#include "TriangularFaces.h"
#include "TCoordinates4.h"
#include <vector>

namespace cagd
{
    class TriangulatedMesh3
    {
        friend class TensorProductSurface3;

        //output to stream:
        friend std::ostream& operator <<(std::ostream& lhs, const TriangulatedMesh3& rhs);

        //input from stream: inverse of the ostream operator
        friend std::istream& operator >>(std::istream& lhs, TriangulatedMesh3& rhs);

    protected:
        // vertex buffer object identifiers
        GLenum                      _usage_flag;
        GLuint                      _vbo_vertices;
        GLuint                      _vbo_normals;
        GLuint                      _vbo_tex_coordinates;
        GLuint                      _vbo_indices;

        // corners of bounding box
        DCoordinate3                 _leftmost_vertex;
        DCoordinate3                 _rightmost_vertex;

        // geometry
        std::vector<DCoordinate3>    _vertex;
        std::vector<DCoordinate3>    _normal;
        std::vector<TCoordinate4>    _tex;
        std::vector<TriangularFace>  _face;

    public:
        // special and default constructor
        TriangulatedMesh3(GLuint vertex_count = 0, GLuint face_count = 0, GLenum usage_flag = GL_STATIC_DRAW);

        // copy constructor
        TriangulatedMesh3(const TriangulatedMesh3& mesh);

        // assignment operator
        TriangulatedMesh3& operator =(const TriangulatedMesh3& rhs);

        // deletes all vertex buffer objects
        GLvoid DeleteVertexBufferObjects();

        // renders the geometry
        GLboolean Render(GLenum render_mode = GL_TRIANGLES) const;

        // updates all vertex buffer objects
        GLboolean UpdateVertexBufferObjects(GLenum usage_flag = GL_STATIC_DRAW);

        // loads the geometry (i.e. the array of vertices and faces) stored in an OFF file
        // at the same time calculates the unit normal vectors associated with vertices
        GLboolean LoadFromOFF(const std::string& file_name, GLboolean translate_and_scale_to_unit_cube = GL_FALSE);

        // saves the geometry into an OFF file
        GLboolean SaveToOFF(const std::string& file_name) const;

        // mapping vertex buffer objects
        GLfloat* MapVertexBuffer(GLenum access_flag = GL_READ_ONLY) const;
        GLfloat* MapNormalBuffer(GLenum access_flag = GL_READ_ONLY) const;
        GLfloat* MapTextureBuffer(GLenum access_flag = GL_READ_ONLY) const;

        // unmapping vertex buffer objects
        GLvoid UnmapVertexBuffer() const;
        GLvoid UnmapNormalBuffer() const;
        GLvoid UnmapTextureBuffer() const;

        // get properties of geometry
        GLuint VertexCount() const;
        GLuint FaceCount() const;

        // destructor
        virtual ~TriangulatedMesh3();
    };
}
