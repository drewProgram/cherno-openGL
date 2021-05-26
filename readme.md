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
...
glfwMakeContextCurrent(window);

if (glewInit() != GLEW_OK)
    std::cout << "Error!" << std::endl;
```

If the function returns GLEW_OK, then you have access to all modern OpenGL features.

## Code
```glGetString(GL_VERSION)```: gets the current OpenGL version.
