#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "TheCherno OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // glew init must be after creating a valid OpenGL context
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // scope for the instantiated classes be deleted before the gl context ends
    {
        // positions must not have any duplicate coordinates
        // first two values are the positions, the next two are the texture coordinates
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
             0.5f, -0.5f, 1.0f, 0.0f, // 1
             0.5f,  0.5f, 1.0f, 1.0f, // 2
            -0.5f,  0.5f, 0.0f, 1.0f  // 3
        };

        // index buffer for performance. Instead repeating the positions on the positions array,
        // we say to OpenGL to repeat it using the positions indexes as a reference.
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // generating vertex array object
        VertexArray va;
        // creating vertex buffer
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        
        // creating buffer layout and pushing the elements of each index.
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        
        // index buffer object
        IndexBuffer ib(indices, 6);

        // creating projection matrix
        // we'll create an orthographic matrix (oppose of perspective projection)
        // distance of 3 units to the top to bottom
        // 4 units left to right
        // multiplying the first 4 numbers will get the result of a 4x3 matrix
        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        // creating view matrix
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, 0, 0));
        // creating model matrix
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, 0.5f, 0));

        // Actually in OpenGL the order is PVM, not MVP
        // creating the PVM
        glm::mat4 mvp = proj * view * model;

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.5f, 0.0f, 0.5f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/image.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer renderer;

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 0.5f, 1.0f);

            renderer.Draw(va, ib, shader);

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}