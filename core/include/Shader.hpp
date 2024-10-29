#pragma once

#include <string>
#include "Config.hpp"
#include "glad/glad.h"
#include "Utils.hpp"

class Shader
{
public:
    Shader();
    ~Shader();

    bool Load(const std::string &vertexPath, const std::string &fragmentPath);
    bool Load(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geometryPath);
    bool Create(const std::string &vertexCode, const std::string &fragmentCode);
    bool Create(const std::string &vertexCode, const std::string &fragmentCode, const std::string &geometryCode);

    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec2(const std::string &name, const glm::vec2 &value) const;
    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetMatrix(const std::string &name, const glm::mat4 &matrix) const;

    GLint GetUniformLocation(const std::string &name) const;

    void PrintActiveUniforms() const;
    void PrintActiveAttributes() const;

    void Use() const;
    void Unbind() const;
    GLuint GetID() const { return programID; }

private:
    GLuint programID;
    bool CompileShader(GLuint shader, const char *source);
    bool LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0);

    void LogShaderError(GLuint shader, const std::string &type);
    void LogProgramError();
};
