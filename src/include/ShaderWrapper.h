#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>


class ShaderWrapper
{
private:
    GLuint ShaderProgramID = -1;

public:
    ShaderWrapper(std::string VertexShaderPath, std::string FragmentShaderPath);
    ShaderWrapper(std::string VertexShaderPath, std::string FragmentShaderPath, std::string GeometryShaderPath);

    void Activate() const;

    void SetBool(const std::string& Name, bool Value) const;
    void SetInt(const std::string& Name, int Value) const;
    void SetFloat(const std::string& Name, float Value) const;
    void SetVec4F(const std::string& Name, glm::vec4 Value) const;
    void SetMat4F(const std::string& Name, glm::mat4 Value) const;

    GLint TrySetVec4f(const std::string& Name, glm::vec4 Value) const;

    GLuint GetShaderProgramId() const;

private:
    [[nodiscard]] GLint GetUniformLocation(const std::string& Name) const;
    static void LoadShader(std::string& ShaderPath, std::string& ShaderCodeOut);

    static GLuint CompileVertexShader(std::string& VertexShaderPath);
    static GLuint CompileFragmentShader(std::string& FragmentShaderPath);
    static GLuint CompileGeometryShader(std::string& GeometryShaderPath);
    void LinkProgram(GLuint VertexShader, GLuint FragmentShader, GLuint GeometryShader);

    static void CompileShader(std::string& ShaderPath, GLuint Shader);
    static void LogShaderError(GLuint GeometryShader, const std::string& Message);
};

