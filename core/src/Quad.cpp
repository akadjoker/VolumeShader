#include "Quad.hpp"

static const float quadVertices[] = {
    // Posições        // Coordenadas de textura
    -1.0f,  1.0f, 0.0f, 1.0f,  // Vértice superior esquerdo
    -1.0f, -1.0f, 0.0f, 0.0f, // Vértice inferior esquerdo
    1.0f,   1.0f, 1.0f, 1.0f,   // Vértice superior direito
    1.0f, -1.0f, 1.0f, 0.0f   // Vértice inferior direito
};

Quad::Quad() : VAO(0), VBO(0) 
{
   
}

void Quad::Release()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    VAO = VBO = 0;
}

Quad::~Quad()
{
    
}

void Quad::Init()
{
      glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);


    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    glBindVertexArray(0);
}

void Quad::Render()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

