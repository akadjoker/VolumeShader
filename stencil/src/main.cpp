
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "Core.hpp"

int screenWidth = 1024;
int screenHeight = 768;
float lastX = 400, lastY = 300; 
bool firstMouse = true;
float sensitivity = 0.5f;

Shader shader;
Shader outline;

bool LoadMainShader()
{
    if (!shader.Load("assets/shaders/main.vs", "assets/shaders/main.fs"))
    {
        return false;
    }
       // exit(1);
    if (!outline.Load("assets/shaders/main.vs", "assets/shaders/main.fs"))
    {
        return  false;
    }

    return true;
}

int main()
{

    Device device;

    device.Create(screenWidth, screenHeight, "Hello World");

    LogInfo("Curent aph %s",GetWorkingDirectory());

    Camera camera;
    camera.Init(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
    camera.SetPosition(glm::vec3(0.0f, 0.5f, 3.0f));
    lastX = screenWidth / 2.0f;
    lastY = screenHeight / 2.0f;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);  
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    if (!LoadMainShader())
    {
        LogError("Failed to load shader");
        device.Close();
        return 1;
    }

    Mesh cube = Mesh::CreateCube();
    Mesh plane = Mesh::CreatePlane(10, 10);

    Texture2D texture = Texture2D::GetDefaultTexture();

    glm::vec3 light = glm::vec3(1.0f, 5.0f, -5.0f);

    while (device.Run())
    {

        int MouseX, MouseY;

        Uint32 button = SDL_GetRelativeMouseState(&MouseX, &MouseY);
        if (button & SDL_BUTTON(SDL_BUTTON_LEFT))
        {

            camera.MouseView(MouseX, -MouseY);
        }
        float deltaTime = 5.0f * device.GetFrameTime();
        float time = device.GetTime();
        if (Keyboard::Down(KEY_W))
        {
            camera.MoveFront(deltaTime);
        }
        if (Keyboard::Down(KEY_S))
        {
            camera.MoveBack(deltaTime);
        }
        if (Keyboard::Down(KEY_A))
        {
            camera.StrafeLeft(deltaTime);
        }
        if (Keyboard::Down(KEY_D))
        {
            camera.StrafeRight(deltaTime);
        }

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        light.x = sin(time) * 5.0f;
        light.z = cos(time) * 5.0f;

        shader.Use();
        shader.SetMatrix("model", model);
        shader.SetMatrix("view", view);
        shader.SetMatrix("projection", projection);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
// 1. Renderiza o Plano (Chão) sem gravar no stencil buffer
    glStencilMask(0x00);              // Desativa a gravação no stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Garante que o chão seja sempre desenhado


        shader.SetVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        shader.SetMatrix("model", model);
        plane.Render();

 // 2. Renderiza o Cubo e grava no stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF); //  escreve 1 no stencil buffer
    glStencilMask(0xFF);                // Ativa a gravação no stencil buffer

        model = glm::mat4(1.0f);
        shader.SetMatrix("model", model);
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
        shader.SetVec3("objectColor", glm::vec3(0.5f, 0.1f, 0.3f));
        cube.Render();

   // 3. Renderiza o Contorno do Cubo usando o stencil buffer
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Desenha onde o stencil não for 1
    glStencilMask(0x00);                 // Desativa a gravação no stencil buffer para o contorno
    glDisable(GL_DEPTH_TEST);            // Desativa o depth test para desenhar o contorno


            outline.Use();
            outline.SetMatrix("view", view);
            outline.SetMatrix("projection", projection);
            outline.SetVec3("objectColor", glm::vec3(1.0f, 1.0f, 0.0f));
           
            model = glm::mat4(1.0f);
            model = glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f));
            model = glm::scale(model,glm::vec3(1.1f, 1.1f, 1.1f));
            outline.SetMatrix("model", model);
            cube.Render();


        // Restaura as configurações padrão
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

        device.Flip();
    }

    device.Close();

    return 0;
}