#pragma once
#include "Config.hpp"




class  Shader 
{
    public:
           Shader();
            virtual ~Shader();
        

            bool Create(const char* vShaderCode, const char* fShaderCode);
            bool Create(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode);
            bool Load(const char* vShaderCode, const char* fShaderCode);
            bool Load(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode);
            bool operator ==(const Shader&      other) const { return m_program == other.m_program; }
            bool operator !=(const Shader&      other) const { return m_program != other.m_program; }


   

        u32 GetID()    {        return m_program;    }

        void Use() const;



   

        void SetInt(const char* name, int value) ;
 
        void SetMatrix4(const char* name, const float *value) ;
        void SetMatrix3(const char* name, const float *value) ;

        void SetFloat(const char* name, float v);
        void SetFloat(const char* name, float x, float y);
        void SetFloat(const char* name, float x, float y, float z);
        void SetFloat(const char* name, float x, float y, float z,float w);



        void Release();

        


    void print();



    void LoadDefaults();

private:
        u32 m_program;
        int m_numAttributes;
        int m_numUniforms;


    private:
         void checkCompileErrors(unsigned int shader, const char* type);
 
          
};