
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "Core.hpp"

int screenWidth = 1024;
int screenHeight = 768;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float sensitivity = 0.5f;

Shader shader;
Shader quadShader;
Shader points;
Quad quad;

bool LoadMainShader()
{
    if (!shader.Load("assets/shaders/main.vs", "assets/shaders/main.fs"))
    {
        return false;
    }

    if (!quadShader.Load("assets/shaders/quad.vs", "assets/shaders/quad.fs"))
    {
        return false;
    }

    if (!points.Load("assets/shaders/points.vs", "assets/shaders/points.fs"))
    {
        return false;
    }

    return true;
}
void startStencil()
{

    glEnable(GL_STENCIL_TEST);
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glEnable(GL_CULL_FACE);
}
void endStencil()
{
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDisable(GL_STENCIL_TEST);
}
void drawStencil()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_STENCIL_TEST);
    glCullFace(GL_BACK);
    glStencilFunc(GL_NOTEQUAL, 0, 0xffffff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

int main()
{

    Device device;

    device.Create(screenWidth, screenHeight, "Hello World");

    LogInfo("Curent aph %s", GetWorkingDirectory());

    Camera camera;
    camera.Init(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
    camera.SetPosition(glm::vec3(0.0f, 0.5f, 3.0f));
    lastX = screenWidth / 2.0f;
    lastY = screenHeight / 2.0f;

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!LoadMainShader())
    {
        LogError("Failed to load shader");
        device.Close();
        return 1;
    }

    Mesh cube = Mesh::CreateCube();
    Mesh plane = Mesh::CreatePlane(50, 50);
                glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    VolumeMesh volume;

    glm::vec3 lightPos(0.0f, 5.0f, -2.0f);



    quad.Init();
   

    Texture2D texture = Texture2D::GetDefaultTexture();

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


        lightPos.x = sin(time) * 5.0f;
        lightPos.z = cos(time) * 5.0f;


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

       

      


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);




        shader.Use();
        shader.SetMatrix("view", view);
        shader.SetMatrix("projection", projection);
        shader.SetVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        shader.SetMatrix("model", model);
        plane.Render();

        model = glm::translate(glm::mat4(1.0f), lightPos);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        shader.SetMatrix("model", model);
        shader.SetVec3("objectColor", glm::vec3(1,1,1));
        cube.Render();


        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.8f, 0.0f));
        shader.SetMatrix("model", model);
        shader.SetVec3("objectColor", glm::vec3(0.5f, 0.1f, 0.3f));
        cube.Render();


   
                    // glDisable(GL_CULL_FACE);
                    // glCullFace(GL_BACK);

            glm::mat4 mvp = projection * view;

            volume.UpdateVolume(lightPos,false,model,&cube);

            // points.Use();
            // points.SetMatrix("mvp", mvp);
            // points.SetVec3("color", glm::vec3(0.2f, 0.2f, 0.2f));
            // volume.Render();

	      
            //startStencil();


            glStencilMask(~0);
	        glStencilFunc(GL_ALWAYS, 0, ~0);
            glDepthFunc(GL_LESS);
	        glDepthMask(GL_FALSE); // no depth buffer writing
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // no color buffer drawing
		    glEnable(GL_STENCIL_TEST);
            glEnable(GL_CULL_FACE);

            points.Use();
            points.SetMatrix("mvp", mvp);
            points.SetVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));

            //zfail
            // glCullFace(GL_FRONT);
            // glStencilOp(GL_KEEP, GL_INCR_WRAP, GL_KEEP);
            // volume.Render();
            // glCullFace(GL_BACK);
            // glStencilOp(GL_KEEP, GL_DECR_WRAP, GL_KEEP);
            // volume.Render();
            //zpass 

            glCullFace(GL_BACK);
            glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP);
            volume.Render();
            glCullFace(GL_FRONT);
            glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP);
            volume.Render();
            
	    glDepthMask(GL_FALSE);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_STENCIL_TEST);
        glCullFace(GL_BACK);
        glStencilFunc(GL_NOTEQUAL, 0, 0xffffff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


        quadShader.Use();
        quadShader.SetInt("useTexture", 0);
        quadShader.SetVec3("color", glm::vec3(0.4f, 0.4f, 0.4f));
        quad.Render();
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, 0, 0xFF); // Reseta o stencil
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);



        device.Flip();
    }


    cube.Release();
    plane.Release();
    quad.Release();

    device.Close();

    return 0;
}