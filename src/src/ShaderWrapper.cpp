#include "ShaderWrapper.h"

#include <fstream>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <glm/ext.hpp>
#include <utility>

void ShaderWrapper::SetFloat(const std::string& Name, float Value) const
{
    GLint UniformLocation = GetUniformLocation(Name);
    glUniform1f(UniformLocation, Value);
}

void ShaderWrapper::SetInt(const std::string& Name, int Value) const
{
    GLint UniformLocation = GetUniformLocation(Name);
    glUniform1i(UniformLocation, Value);
}

void ShaderWrapper::SetBool(const std::string& Name, bool Value) const
{
    GLint UniformLocation = GetUniformLocation(Name);
    glUniform1i(UniformLocation, static_cast<GLint>(Value));
}

void ShaderWrapper::SetVec4F(const std::string& Name, glm::vec4 Value) const
{
    GLint UniformLocation = GetUniformLocation(Name);
    glUniform4f(UniformLocation, Value.x, Value.y, Value.z, Value.w);
}

void ShaderWrapper::SetMat4F(const std::string& Name, glm::mat4 Value) const
{
    GLint UniformLocation = GetUniformLocation(Name);
    glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
}

GLint ShaderWrapper::GetUniformLocation(const std::string& Name) const
{
    GLint UniformLocation = glGetUniformLocation(ShaderProgramID, Name.c_str());
    if (UniformLocation == -1)
    {
        spdlog::warn(Name + " not found");
    }

    return UniformLocation;
}

GLuint ShaderWrapper::GetShaderProgramId() const
{
    return ShaderProgramID;
}

void ShaderWrapper::Activate() const
{
    glUseProgram(ShaderProgramID);
}

void ShaderWrapper::LoadShader(std::string& ShaderPath, std::string& ShaderCodeOut)
{
    std::ifstream ShaderFile;

    ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        ShaderFile.open(ShaderPath);
        std::stringstream ShaderStream;

        ShaderStream << ShaderFile.rdbuf();
        ShaderFile.close();

        ShaderCodeOut = ShaderStream.str();
    } catch (std::ifstream::failure& e)
    {
        SPDLOG_ERROR("Shader file loading failure");
    }
}

ShaderWrapper::ShaderWrapper(std::string VertexShaderPath, std::string FragmentShaderPath) : ShaderWrapper(
        std::move(VertexShaderPath), std::move(FragmentShaderPath), "${-1}")
{

}

ShaderWrapper::ShaderWrapper(std::string VertexShaderPath, std::string FragmentShaderPath,
                             std::string GeometryShaderPath)
{
    GLuint VertexShader, FragmentShader;

    VertexShader = CompileVertexShader(VertexShaderPath);
    FragmentShader = CompileFragmentShader(FragmentShaderPath);
    if (GeometryShaderPath != "${-1}")
    {
        GLuint GeometryShader = CompileGeometryShader(GeometryShaderPath);
        LinkProgram(VertexShader, FragmentShader, GeometryShader);
    }
    else
    {
        LinkProgram(VertexShader, FragmentShader, 0);
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

}


void ShaderWrapper::LinkProgram(GLuint VertexShader, GLuint FragmentShader, GLuint GeometryShader = 0)
{
    ShaderProgramID = glCreateProgram();
    glAttachShader(ShaderProgramID, VertexShader);
    glAttachShader(ShaderProgramID, FragmentShader);

    if (GeometryShader != 0)
    {
        glAttachShader(ShaderProgramID, GeometryShader);
    }

    glLinkProgram(ShaderProgramID);

    GLint ProgramLinkingResult;
    glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &ProgramLinkingResult);
    if (!ProgramLinkingResult)
    {
        char Log[512];
        glGetProgramInfoLog(ShaderProgramID, 512, nullptr, Log);
        SPDLOG_ERROR("Program linking failed: " + std::string(Log));
    }
}

GLuint ShaderWrapper::CompileFragmentShader(std::string& FragmentShaderPath)
{
    GLuint FragmentShader;
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    CompileShader(FragmentShaderPath, FragmentShader);
    LogShaderError(FragmentShader, "Fragment Shader compilation failed: ");

    return FragmentShader;
}

GLuint ShaderWrapper::CompileVertexShader(std::string& VertexShaderPath)
{
    GLuint VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);

    CompileShader(VertexShaderPath, VertexShader);
    LogShaderError(VertexShader, "Vertex Shader compilation failed: ");

    return VertexShader;
}

GLuint ShaderWrapper::CompileGeometryShader(std::string& GeometryShaderPath)
{
    GLuint GeometryShader;
    GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);

    CompileShader(GeometryShaderPath, GeometryShader);
    LogShaderError(GeometryShader, "Geometry Shader compilation failed: ");

    return GeometryShader;
}

void ShaderWrapper::LogShaderError(GLuint GeometryShader, const std::string& Message)
{
    GLint ShaderCompilationResult;
    glGetShaderiv(GeometryShader, GL_COMPILE_STATUS, &ShaderCompilationResult);
    if (!ShaderCompilationResult)
    {
        char Log[512] = "Error";
        glGetShaderInfoLog(GeometryShader, 512, nullptr, Log);
        SPDLOG_ERROR(Message + std::string(Log));
    }
}

void ShaderWrapper::CompileShader(std::string& ShaderPath, GLuint Shader)
{
    std::string ShaderCode;
    LoadShader(ShaderPath, ShaderCode);
    const GLchar* ConstCharPtrShaderCode = ShaderCode.c_str();
    glShaderSource(Shader, 1, &ConstCharPtrShaderCode, nullptr);
    glCompileShader(Shader);
}

GLint ShaderWrapper::TrySetVec4f(const std::string& Name, glm::vec4 Value) const
{
    GLint UniformLocation = glGetUniformLocation(ShaderProgramID, Name.c_str());
    glUniformMatrix4fv(UniformLocation, 1, GL_FALSE, glm::value_ptr(Value));
    return UniformLocation;
}




