#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    bool valid;
    GLuint ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        valid = 1;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cerr << "ERROR ID: " << e.code() << " ERROR MSG: " << e.what() << std::endl;
            valid = 0;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        GLuint vertex, fragment;
        // vertex shader
#ifdef _DEBUG
        std::clog << "INFO::SHADER::CREATE_SHADER <" << "VERTEX" << ">" << std::endl;
#endif
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
#ifdef _DEBUG
        std::clog << "INFO::SHADER::COMPILE_SHADER <" << "VERTEX" << ">" << std::endl;
#endif
        glCompileShader(vertex);
        if(checkCompileErrors(vertex, "VERTEX"))
        {
            valid = 0;
        }
        // fragment Shader
#ifdef _DEBUG
        std::clog << "INFO::SHADER::CREATE_SHADER <" << "FRAGMENT" << ">" << std::endl;
#endif
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
#ifdef _DEBUG
        std::clog << "INFO::SHADER::COMPILE_SHADER <" << "FRAGMENT" << ">" << std::endl;
#endif
        glCompileShader(fragment);
        if (checkCompileErrors(fragment, "FRAGMENT"))
        {
            valid = 0;
        }
        // shader Program
#ifdef _DEBUG
        std::clog << "INFO::SHADER::CREATE_SHADER_PROGRAM <" << "PROGRAM" << ">" << std::endl;
#endif
        ID = glCreateProgram();
#ifdef _DEBUG
        std::clog << "INFO::SHADER::ATTACH_SHADER_TO_SHADER_PROGRAM <" << "VERTEX" << ">" << std::endl;
#endif
        glAttachShader(ID, vertex);
#ifdef _DEBUG
        std::clog << "INFO::SHADER::ATTACH_SHADER_TO_SHADER_PROGRAM <" << "FRAGMENT" << ">" << std::endl;
#endif
        glAttachShader(ID, fragment);
#ifdef _DEBUG
        std::clog << "INFO::SHADER::LINKING_SHADER_PROGRAM <" << "PROGRAM" << ">" << std::endl;
#endif
        glLinkProgram(ID);
        if (checkCompileErrors(ID, "PROGRAM"))
        {
            valid = 0;
        }
        // delete the shaders as they're linked into our program now and no longer necessary
#ifdef _DEBUG
        std::clog << "INFO::SHADER::DELETE_SHADER <" << "VERTEX" << ">" << std::endl;
#endif
        glDeleteShader(vertex);
#ifdef _DEBUG
        std::clog << "INFO::SHADER::DELETE_SHADER <" << "FRAGMENT" << ">" << std::endl;
#endif
        glDeleteShader(fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
// ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    bool checkCompileErrors(unsigned int shader, std::string type)
    {
        int success, infoLen;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, &infoLen, infoLog);
                if (infoLen > 1024)
                {
                    std::cerr << "WARNING::SHADER::LOG_LEN_TOO_LONG" << std::endl;
                }
                std::cerr << "ERROR::SHADER::SHADER_COMPILATION_FAILED <" << type << ">\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                return 1;
            }
            else
            {
                std::clog << "INFO::SHADER::SHADER_COMPILATION_SUCCUSSFUL <" << type << ">" << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, &infoLen, infoLog);
                if (infoLen > 1024)
                {
                    std::cerr << "WARNING::SHADER::LOG_LEN_TOO_LONG" << std::endl;
                }
                std::cerr << "ERROR::SHADER::PROGRAM_LINKING_FAILED <" << type << ">\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                return 1;
            }
            else
            {
                std::clog << "INFO::SHADER::PROGRAM_LINKING_SUCCUSSFUL <" << type << ">" << std::endl;
            }
        }
        return 0;
    }
};
#endif