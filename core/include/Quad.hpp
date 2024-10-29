#pragma once

#include "Config.hpp"
#include "glad/glad.h"
class Quad
{
public:
    Quad();
    ~Quad();

    void Init();
    void Render();
    void Release();

private:
    GLuint VAO, VBO;
  
  
};