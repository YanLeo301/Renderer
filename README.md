# Renderer

This is an OpenGL Renderer with clean, reusable abstractions for components (Mesh, Shader, Texture, Skybox).

Procedural grass field rendered using Instanced Rendering:
![](demo/grass_demo.gif)

## Usage

In this snippet you can see how Mesh, Shader and Texture for an object can be created and rendered:

```cpp
#include "shader.h"
#include "mesh.h"
#include "texture.h"

Mesh cubeMesh(cubeVertices, cubeIndices, PositionTexture);
glm::vec3 cubeSize = glm::vec3(1.0f);
glm::vec3 cubePosition = glm::vec3(1.0f, 5.0f, 1.0f);
glm::mat4 cubeModel = glm::scale(glm::translate(glm::mat4(1.0f), cubePosition), cubeSize);
Shader cubeShader("res/shaders/basic.shader");
Texture cubeTexture("res/textures/wood/wood.png");

while (!glfwWindowShouldClose(window))
{
    cubeShader.Use();
    cubeShader.SetMat4("model", cubeModel);
    cubeShader.SetMat4("view", view);
    cubeShader.SetMat4("projection", projection);
    cubeTexture.Bind();
    cubeShader.SetInt("uTexture", 0);
    cubeMesh.Draw();
}
```

The resulting rendered cube then looks like this:

## Controls

- Look around using the mouse
- Move around using WASD
- Close window pressing Esc