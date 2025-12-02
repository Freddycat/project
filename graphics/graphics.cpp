
#include "graphics.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Gfx
{
    void CheckGLError(const char *functionName)
    {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            std::cerr << "OpenGL Error in " << functionName << ": " << error << std::endl;
        }
    }
    GLuint shaderID = 0;
    GLuint InitializeShader(const std::filesystem::path &vertexPath, const std::filesystem::path &fragmentPath)
    {
        std::cout << "Vertex Shader Path: " << vertexPath << std::endl;
        std::cout << "Fragment Shader Path: " << fragmentPath << std::endl;
        // 1. Retrieve the vertex / fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // Ensure ifstream objects can throw exceptions
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        if (!std::filesystem::exists(vertexPath))
        {
            throw std::runtime_error("ERROR::SHADER::VERTEX_FILE_NOT_FOUND: " + vertexPath.string());
        }
        if (!std::filesystem::exists(fragmentPath))
        {
            throw std::runtime_error("ERROR::SHADER::FRAGMENT_FILE_NOT_FOUND: " + fragmentPath.string());
        }
        try
        {
            // Open files
            vShaderFile.open(vertexPath.string());
            fShaderFile.open(fragmentPath.string());
            // Read file's buffer contents into streams
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // Close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ");
        }

        // 2. Compile shaders
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();

        unsigned int vertexShaderId, fragmentShaderId;
        int success;
        char infoLog[512];

        // Create vertex shader
        vertexShaderId = glCreateShader(GL_VERTEX_SHADER);     // Create a vertex shader
        glShaderSource(vertexShaderId, 1, &vShaderCode, NULL); // Attach the vertex shader source code
        glCompileShader(vertexShaderId);                       // Compile the vertex shader
        // Check for vertex shader compile errors
        glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
        }

        // Create fragment shader
        fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);   // Create a fragment shader
        glShaderSource(fragmentShaderId, 1, &fShaderCode, NULL); // Attach the fragment shader source code
        glCompileShader(fragmentShaderId);                       // Compile the fragment shader
        // Check for fragment shader compile errors
        glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            glDeleteShader(vertexShaderId);
            throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
        }

        // Shader program
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShaderId);
        glAttachShader(programID, fragmentShaderId);
        glLinkProgram(programID);
        // Check for shader program linking errors
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programID, 512, NULL, infoLog);
            std::cout << infoLog << std::endl;
            glDeleteShader(vertexShaderId);
            glDeleteShader(fragmentShaderId);
            throw std::runtime_error("ERROR::PROGRAM::LINKING_FAILED\n");
        }

        GLint count;
        glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &count);
        std::cout << "Active attributes: " << count << std::endl;

        // Delete vertex and fragment shader instances as they have been linked
        glDetachShader(programID, vertexShaderId);
        glDetachShader(programID, fragmentShaderId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);

        return programID;
    }

    void CheckGLError(const char *functionName);
    extern GLuint shaderID;
    GLuint InitializeShader(const std::string &vertexPath, const std::string &fragmentPath);

    void Use(GLuint shaderID)
    {
        glUseProgram(shaderID);
    }

    void SetBool(GLuint shaderID, const std::string &name, bool value)
    {
        glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
    }

    void SetInt(GLuint shaderID, const std::string &name, int value)
    {
        glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
    }
    
    void SetFloat(GLuint shaderID, const std::string &name, float value)
    {
        glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
    }
    
    void SetMat3(GLuint shaderID, const std::string &name, const GLfloat *mat)
    {
        glUniformMatrix3fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, mat);
    }
    
    void SetMat4(GLuint shaderID, const std::string &name, const GLfloat *mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, mat);
    }
    
    void SetVec2(GLuint shaderID, const std::string &name, glm::vec2 vec)
    {
        glUniform2f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y);
    }
    
    void SetVec3(GLuint shaderID, const std::string &name, glm::vec3 vec)
    {
        glUniform3f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y, vec.z);
    }
    
    void SetVec4(GLuint shaderID, const std::string &name, glm::vec4 vec)
    {
        glUniform4f(glGetUniformLocation(shaderID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
    }
    
    GLuint CreateVAO()
    {
        unsigned int vaoID;
        glGenVertexArrays(1, &vaoID);
        return vaoID;
    }

    GLuint CreateVBO(GLsizeiptr verticesByteSize, const void *vertices, GLenum usage)
    {
        GLuint vboID;
        glGenBuffers(1, &vboID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, verticesByteSize, vertices, usage);
        return vboID;
    }

    void AddVertexAttrib(GLuint location, GLuint attribLen, GLuint stride, GLuint offset)
    {
        glVertexAttribPointer(location, attribLen, GL_FLOAT, GL_FALSE, stride, (void *)(uintptr_t)offset);
        glEnableVertexAttribArray(location);
    }

    GLuint CreateEBO(GLsizeiptr indicesByteSize, GLuint *indices, GLenum usage)
    {
        GLuint eboID;
        glGenBuffers(1, &eboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesByteSize, indices, usage);
        return eboID;
    }

    void UseVAO(GLuint vaoID)
    {
        glBindVertexArray(vaoID);
    }

    void UnbindVAO()
    {
        glBindVertexArray(0);
    }
}
