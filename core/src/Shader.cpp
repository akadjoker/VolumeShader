#include "pch.h"
#include "Shader.hpp"

Shader::Shader() : programID(0) {}

Shader::~Shader()
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}

bool Shader::Load(const std::string &vertexPath, const std::string &fragmentPath)
{
    char *vertexCode = LoadTextFile(vertexPath.c_str());
    char *fragmentCode = LoadTextFile(fragmentPath.c_str());

    if (!vertexCode || !fragmentCode)
    {
        LogError("Failed to load shader files: %s, %s", vertexPath.c_str(), fragmentPath.c_str());
        free(vertexCode);
        free(fragmentCode);
        return false;
    }

    bool success = Create(vertexCode, fragmentCode);

    free(vertexCode);
    free(fragmentCode);
    return success;
}

bool Shader::Load(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geometryPath)
{
    char *vertexCode = LoadTextFile(vertexPath.c_str());
    char *fragmentCode = LoadTextFile(fragmentPath.c_str());
    char *geometryCode = LoadTextFile(geometryPath.c_str());

    if (!vertexCode || !fragmentCode || !geometryCode)
    {
        LogError("Failed to load shader files: %s, %s, %s", vertexPath.c_str(), fragmentPath.c_str(), geometryPath.c_str());

        free(vertexCode);
        free(fragmentCode);
        free(geometryCode);
        return false;
    }

    bool success = Create(vertexCode, fragmentCode, geometryCode);

    free(vertexCode);
    free(fragmentCode);
    free(geometryCode);
    return success;
}

bool Shader::Create(const std::string &vertexCode, const std::string &fragmentCode)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    if (!CompileShader(vertexShader, vertexCode.c_str()) || !CompileShader(fragmentShader, fragmentCode.c_str()))
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    return LinkProgram(vertexShader, fragmentShader);
}

bool Shader::Create(const std::string &vertexCode, const std::string &fragmentCode, const std::string &geometryCode)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

    if (!CompileShader(vertexShader, vertexCode.c_str()) ||
        !CompileShader(fragmentShader, fragmentCode.c_str()) ||
        !CompileShader(geometryShader, geometryCode.c_str()))
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(geometryShader);
        return false;
    }

    return LinkProgram(vertexShader, fragmentShader, geometryShader);
}

bool Shader::CompileShader(GLuint shader, const char *source)
{
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        LogShaderError(shader, "COMPILE");
        return false;
    }
    return true;
}

bool Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader)
{
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    if (geometryShader != 0)
    {
        glAttachShader(programID, geometryShader);
    }
    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        LogProgramError();
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryShader != 0)
    {
        glDeleteShader(geometryShader);
    }

    return true;
}

void Shader::LogShaderError(GLuint shader, const std::string &type)
{
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    std::vector<char> log(logLength);
    glGetShaderInfoLog(shader, logLength, nullptr, log.data());

    LogError("Shader %s\n%s", type.c_str(), log.data());
}

void Shader::LogProgramError()
{
    GLint logLength;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

    std::vector<char> log(logLength);
    glGetProgramInfoLog(programID, logLength, nullptr, log.data());

    LogError("Shader Program\n%s", log.data());
}

void Shader::Use() const
{
    glUseProgram(programID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

GLint Shader::GetUniformLocation(const std::string &name) const
{
    GLint location = glGetUniformLocation(programID, name.c_str());
    if (location == -1)
    {
        LogWarning("Uniform %s not found!", name.c_str());
    }
    return location;
}

void Shader::SetInt(const std::string &name, int value) const
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const
{
    glUniform2fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetMatrix(const std::string &name, const glm::mat4 &matrix) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::PrintActiveUniforms() const
{
    GLint numUniforms;
    glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numUniforms);

    LogInfo("ACTIVE UNIFORMS:");
    for (GLint i = 0; i < numUniforms; ++i)
    {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(programID, i, sizeof(name), &length, &size, &type, name);

        LogInfo("Uniform #%d: Name: %s, Type: %d, Size: %d", i, name, type, size);
    }
}

void Shader::PrintActiveAttributes() const
{
    GLint numAttributes;
    glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &numAttributes);

    LogInfo("ACTIVE ATTRIBUTES:");
    for (GLint i = 0; i < numAttributes; ++i)
    {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveAttrib(programID, i, sizeof(name), &length, &size, &type, name);

        LogInfo("Attribute #%d: Name: %s, Type: %d, Size: %d", i, name, type, size);
    }
}