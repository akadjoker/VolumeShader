#include "pch.h"
#include "Shader.hpp"
#include "Utils.hpp"




Shader::Shader()
{
    m_program=0;
    m_numAttributes=0;
    m_numUniforms=0;

}

Shader::~Shader()
{
    Release();
}

void Shader::Use() const
{
    glUseProgram(m_program);
}

void Shader::Release()
{
    if (m_program>0)
    {
        LogInfo( "SHADER: [ID %i] Release shader program.", m_program);
        glDeleteProgram(m_program);
    }
    m_program=0;
}

bool Shader::Create(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode)
{
    // 2. compile shaders
    unsigned int vertex, fragment, geometry;
    
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // geometry shader
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    checkCompileErrors(geometry, "GEOMETRY");
    
    // shader Program
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glAttachShader(m_program, geometry);
    glLinkProgram(m_program);
    checkCompileErrors(m_program, "PROGRAM");
    
    if (m_program>0)
        LogInfo("SHADER: [ID %i] Create shader program.", m_program);
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);
    glUseProgram(m_program);
    
    return true;


}


bool Shader::Create(const char *vShaderCode, const char *fShaderCode)
{
     // 2. compile shaders
    unsigned int vertex, fragment;
    
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // shader Program
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);
    checkCompileErrors(m_program, "PROGRAM");

    if (m_program>0)
        LogInfo( "SHADER: [ID %i] Create shader program.", m_program);
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glUseProgram(m_program); 
    
    return true;
}

bool Shader::Load(const char *vShaderCode, const char *fShaderCode)
{
    bool result=false;
    char *shaderCode = LoadTextFile(vShaderCode);
    char *fragCode = LoadTextFile(fShaderCode);
    if (shaderCode && fragCode)
    {
        result = Create(shaderCode, fragCode);
        free(shaderCode);
        free(fragCode);
    }
    
    return result;
}

bool Shader::Load(const char *vShaderCode, const char *fShaderCode, const char *gShaderCode)
{
    bool result=false;
    char *shaderCode = LoadTextFile(vShaderCode);
    char *fragCode = LoadTextFile(fShaderCode);
    char *geomCode = LoadTextFile(gShaderCode);
    if (shaderCode && fragCode && geomCode)
    {
        result = Create(shaderCode, fragCode, geomCode);
        free(shaderCode);
        free(fragCode);
        free(geomCode);
    }
    
    return result;
}

void Shader::checkCompileErrors(unsigned int shader, const char* type)
{
      int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            LogError( "SHADER: Error Compile %s Msg: %s", type.c_str(), infoLog );
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            LogError("SHADER: Error Link %s Msg: %s", type.c_str(), infoLog );
        }
    }
}


int  Shader::getUniform(const char* name)
{
    return getUniformLocation(name);
}
int Shader::getAttribute(const char* name) 
{
    return getAttribLocation(name);
}
int Shader::getUniformLocation(const char* uniformName) const
{
    int location =  glGetUniformLocation(m_program, uniformName.c_str());
      if (location == -1)
         LogError( "SHADER: [ID %i] Failed to find shader uniform: %s", m_program, uniformName.c_str());
    return location;
}
int Shader::getAttribLocation( const char* attribName) const
{
    int location  = glGetAttribLocation(m_program, attribName.c_str());
    if (location == -1)
        LogError( "SHADER: [ID %i] Failed to find shader attribute: %s", m_program, attribName.c_str());
    return location;
}
bool Shader::addUniform(const char* name)
{
    int location = -1;
    location = glGetUniformLocation(m_program, name.c_str());
    if (location == -1)
    {
        LogError( "SHADER: [ID %i] Failed to find shader uniform: %s", m_program, name.c_str());
        return false;
    }
    

    //  LogInfo( "SHADER: [ID %i] shader uniform (%s) set at location: %i", m_program, name.c_str(), location);
    
    return true;
}
bool Shader::addAttribute(const char* name)
{
    int location =  glGetAttribLocation(m_program, name.c_str());
    if (location == -1)
    {
        LogError( "SHADER: [ID %i] Failed to find shader attribute: %s", m_program, name.c_str());
        return false;
    }

    return true;
}




void Shader::SetInt(const char* name, int value) 
{
    int id = getUniform(name);
    if (id != -1) glUniform1i(id, value);
}

void Shader::SetMatrix4(const char* name, const float *value)
{
     int id = getUniform(name);
    if (id != -1)
        glUniformMatrix4fv(id, 1, GL_FALSE, value);
}

void Shader::SetMatrix3(const char* name, const float *value)
{
    int id = getUniform(name);
    if (id != -1)
        glUniformMatrix3fv(id, 1, GL_FALSE, value);
}

void Shader::SetFloat(const Chars& name, float v)
{
    int id = getUniform(name);
    if (id != -1)
        glUniform1f(id,  v);
}
void Shader::SetFloat(const Chars& name, float x, float y)
{
    int id = getUniform(name);
    if (id != -1)
        glUniform2f(id,  x,y);
}
void Shader::SetFloat(const Chars& name,float x, float y, float z)
{
   int id = getUniform(name);
    if (id != -1)
        glUniform3f(id, x, y, z);
}
void Shader::SetFloat(const Chars& name, float x, float y, float z, float w)
{
	int id = getUniform(name);
    if (id != -1)
		glUniform4f(id,  x, y, z, w);
}   


void Shader::print()
{
    LogInfo("[SHADER]  Id(%d) Num Attributes(%d)  Num Uniforms (%d)",m_program, m_numAttributes, m_numUniforms);

   

}
void Shader::LoadDefaults()
{
    GLint numActiveAttribs = 0;
    GLint maxAttribNameLength = 0;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribNameLength);
    m_numAttributes=numActiveAttribs;
    for(GLint attrib = 0; attrib < numActiveAttribs; ++attrib)
    {
        int namelen = -1;
        int num = -1;
        char name[256]; // Assume no variable names longer than 256
        GLenum type = GL_ZERO;
        glGetActiveAttrib(m_program, attrib,  sizeof(name) - 1, &namelen, &num, &type, name);
        name[namelen] = 0;
        addAttribute(Chars((char*)&name[0]));
        glBindAttribLocation(m_program, attrib, (char*)&name[0]);
        LogInfo("SHADER: [ID %i] Active attribute (%s) set at location: %i", m_program, name,attrib);
    }
    // Get available shader uniforms
    int uniformCount = -1;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformCount);
    m_numUniforms=uniformCount;

    for (int i = 0; i < uniformCount; i++)
    {
        int namelen = -1;
        int num = -1;
        char name[256]; // Assume no variable names longer than 256
        GLenum type = GL_ZERO;
        glGetActiveUniform(m_program, i, sizeof(name) - 1, &namelen, &num, &type, name);
        name[namelen] = 0;
        addUniform(Chars((char*)&name[0]));
        LogInfo("SHADER: [ID %i] Active uniform (%s) set at location: %i", m_program, name, glGetUniformLocation(m_program, name));
    }
}