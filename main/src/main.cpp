
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "Core.hpp"

int screenWidth = 1024;
int screenHeight = 768;
float lastX = 400, lastY = 300; 
bool firstMouse = true;
float sensitivity = 0.5f;

int main()
{

    Device device;

    device.Create(screenWidth, screenHeight, "Hello World");

    Camera camera;
    camera.Init(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
    camera.SetPosition(glm::vec3(0.0f, 0.5f, 3.0f));
    lastX = screenWidth / 2.0f;
    lastY = screenHeight / 2.0f;


    const char *vertexShaderSource = GLSL(
        
            layout(location = 0) in vec3 aPos;
            layout(location = 1) in vec2 aTexCoord;
            layout(location = 2) in vec3 aNormal;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            out vec2 TexCoord;
            out vec3 Normal;
            out vec3 FragPos;

            void main() 
            {
                FragPos = vec3(model * vec4(aPos, 1.0));
                Normal = mat3(transpose(inverse(model))) * aNormal;
                TexCoord = aTexCoord;
                gl_Position = projection * view * vec4(FragPos, 1.0);
            }
            );


    const char *fragmentShaderSource = GLSL(
        
            out vec4 FragColor;

            in vec2 TexCoord;
            in vec3 Normal;
            in vec3 FragPos;

            uniform vec3 lightColor;
            uniform vec3 objectColor;
            uniform vec3 lightPos;
  

            void main() 
            {
                vec3 norm = normalize(Normal);
                vec3 lightDir = normalize(lightPos - FragPos);
                float diff = max(dot(norm, lightDir), 0.0);

                vec3 result = (lightColor * diff) * objectColor;
                FragColor = vec4(result, 1.0);
            }
        );

        Shader shader;
        shader.Create(vertexShaderSource, fragmentShaderSource);
        shader.PrintActiveUniforms();

        Mesh cube = Mesh::CreateCube();
        Mesh plane = Mesh::CreatePlane(10,10);

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
            shader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
            shader.SetVec3("lightPos",  light);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.SetVec3("objectColor", glm::vec3(1.0f, 0.0f, 1.0f));
            cube.Render();

            shader.SetVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
            plane.Render();

            device.Flip();
    }

    device.Close();

    return 0;
}