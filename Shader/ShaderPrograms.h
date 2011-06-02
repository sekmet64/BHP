#pragma once

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <string>

namespace cagd
{
    class ShaderProgram
    {
    protected:
        // handles of objects
        GLuint      _vertex_shader;
        GLuint      _fragment_shader;
        GLuint      _program;

        // file names of sources
        std::string _vertex_shader_file_name;
        std::string _fragment_shader_file_name;

        // sources
        std::string _vertex_shader_source;
        std::string _fragment_shader_source;

        // status values
        GLint       _vertex_shader_compiled;
        GLint       _fragment_shader_compiled;
        GLint       _linked;

        // log
        GLboolean   _ListOpenGLErrors(const char *file_name, GLint line, std::ostream& output = std::cout) const;  // returns GL_TRUE if an OpenGL error occurred, GL_FALSE otherwise
        GLvoid      _ListVertexShaderInfoLog(std::ostream& output = std::cout) const;
        GLvoid      _ListFragmentShaderInfoLog(std::ostream& output = std::cout) const;
        GLvoid      _ListProgramInfoLog(std::ostream& output = std::cout) const;
        GLvoid      _ListValidateInfoLog(std::ostream& output = std::cout) const;

    public:
        // default constructor
        ShaderProgram();

        GLboolean InstallShaders(const std::string &vertex_shader_file_name, const std::string &fragment_shader_file_name, GLboolean logging_is_enabled = GL_FALSE, std::ostream &output = std::cout);

        GLboolean SetUniformVariable1i(const GLchar *name, GLint parameter) const;
        GLboolean SetUniformVariable2i(const GLchar *name,
            GLint parameter_1,
            GLint parameter_2) const;
        GLboolean SetUniformVariable3i(const GLchar *name,
            GLint parameter_1,
            GLint parameter_2,
            GLint paramater_3) const;
        GLboolean SetUniformVariable4i(const GLchar *name,
            GLint parameter_1,
            GLint parameter_2,
            GLint paramater_3,
            GLint parameter_4) const;

        GLboolean SetUniformVariable1f(const GLchar *name, GLfloat parameter) const;
        GLboolean SetUniformVariable2f(const GLchar *name,
            GLfloat parameter_1,
            GLfloat parameter_2) const;
        GLboolean SetUniformVariable3f(const GLchar *name,
            GLfloat parameter_1,
            GLfloat parameter_2,
            GLfloat parameter_3) const;
        GLboolean SetUniformVariable4f(const GLchar *name,
            GLfloat parameter_1,
            GLfloat parameter_2,
            GLfloat parameter_3,
            GLfloat parameter_4) const;


        GLint GetUniformVariableLocation(const GLchar *name, GLboolean logging_is_enabled = GL_FALSE, std::ostream& output = std::cout) const;

        GLvoid Disable() const;
        GLvoid Enable(GLboolean logging_is_enabled = GL_FALSE, std::ostream& output = std::cout) const;

        virtual ~ShaderProgram();
    };
}
