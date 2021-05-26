# OpenGL

## Set up
Note: I'm using static linking.
### GLFW
First you need to set up something to open a window. In this repo is being used the lib GLFW.

Steps:
- Paste in the Dependencies folder the include and lib folders needed for using the library.
- Project Settings > C/C++ > Additional Include Directories: $(SolutionDir)Dependencies/GLFW/include
- Project Settings > Linker > General > Additional Library Directories: $(SolutionDir)Dependencies/GLFW/lib-vc2019
- Project Settings > C/C++ > Input > Additional Dependencies: glfw3.lib;opengl32.lib;

opengl32.lib is for being able to use basic opengl (legacy opengl) functions.

After that, you can write this boilerplate code to open a window:
```cpp
#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
```

### GLEW
Legacy OpenGL is not very great, so to being able to use the modern OpenGL we need to use another library. We'll use GLEW for this task. What this lib does is to get the binary which has the OpenGL implementation of the GPU manufactor and transform in functions we can use through the lib.

Steps to add GLEW into the project:
- Paste the GLEW folder into the Dependencies folder
- Project Settings > C/C++ > Additional Include Directories: $(SolutionDir)Dependencies\GLEW\include
- Project Settings > Linker > General > Additional Library Directories: $(SolutionDir)Dependencies\GLEW\lib\Release\Win32
- Project Settings > Linker > Input > Additional Dependencies: glew32s.lib
- Project Settings > C/C++ > Preprocessor > Preprocessor Definitions: GLEW_STATIC

In order to the build process works we need to define the GLEW_STATIC preprocessor token when linking staticly and GLEW_BUILD when linking dynamicly.

After that, we need to initialize GLEW. After having a valid OpenGL context created, we use the function ```glewInit()```. To make sure everything is working, we can call a basic function.

```cpp
#include <GL/glew.h>
...
glfwMakeContextCurrent(window);

if (glewInit() != GLEW_OK)
    std::cout << "Error!" << std::endl;
```

If the function returns GLEW_OK, then you have access to all modern OpenGL features.

## Concepts
Best doc reference for OpenGL: [docs.GL](http://docs.gl/)

```Vertex buffer```: array of bytes of memory. Blob of memory which we can push bytes into it. Instead of being in the CPU, is buffer on the GPU VRAM. 
```Shader```: Code which runs on the GPU.

vertex: point on the geometry

OpenGL is a state machine.

Select a buffer and a shader, then draw a triangle. Based on which buffer and shader it will determine where the triangle will be draw and how.

Whenever you want to use a buffer, you will use the id you passed in the buffer creation function.

```Vertex shader```: Called for each vertex. Used for defining position.
 
```Fragment shader```: Called for every pixel. Used for defining colors.

## Code
```glGetString(GL_VERSION)```
Gets the current OpenGL version.

```glVertexAttribPointer(index, size, type, normalized, stride, pointer)```
`size`:  How much indexes each vertex has
`stride`: Specifies the offset of each vertex. (vertex 1 starts at 0 bytes, vertex 2 starts at 4 bytes...)
`pointer`: pointer to an offset of positions inside the vertex (when starts the position, when the texture starts...)

