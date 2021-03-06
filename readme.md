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

`Vertex:` point on the geometry.

`Uniform`: another way of setting data from the CPU to the GPU.

OpenGL is a state machine.

Select a buffer and a shader, then draw a triangle. Based on which buffer and shader it will determine where the triangle will be draw and how.

Whenever you want to use a buffer, you will use the id you passed in the buffer creation function.

```Vertex shader```: Called for each vertex. Used for defining position.
 
```Fragment shader```: Called for every pixel. Used for defining colors.

Object name - unsigned int that identify a buffer.

**Texture slots**: OpenGL has 32 slots (actually it depends on the platform, but the maximum is 32) for binding textures more than one texture at once.

## Code
### glGetString(GL_VERSION)
Gets the current OpenGL version.

### void glGenBuffers(GLsizei n, GLuint* buffers)
#### Params
`n`: Specifies the number of buffer objects names to be generated.
`buffers`: Specifies an array which the generated buffer object names are stored.

#### Description
Returns `n` buffer object names in `buffers`. Buffers object names returned by a call to `glGenBuffers` are note returned by subsequent calls, unless they are first deleted with `glDeleteBuffers`.
No buffer objects are associated with returned buffer object names until they are first bound by calling `glBindBuffer`.

### void glBindBuffer(GLenum target, GLuint buffer)
#### Params
`target`: Specifies the target to which the buffer object is bound, which must be one of the buffer binding targets in the following table:

`buffer`: Specifies the name of a buffer object.

#### Description
Binds a buffer object to the specified buffer binding point. Calling `glBindBuffer` with `target` set to one of the accepted symbolic constants and `buffer` set to the name of a buffer object binds that buffer object name to the target. If no buffer object with name `buffer` exists, one is created with that name. When a buffer object is bound to a target, the previous binding for that target is automatically borken.
The State of a buffer object immediately fter it is first bound is an unmapped zero-sized memory buffer wit *GL_READ_WRITE* access and *GL_STATIC_DRAW* usage.

When a non-zero buffer object is bound to the *GL_ARRAY_BUFFER* target, the vertex array pointer param is interpreted as an offset within the buffer object mesaured in basic machine units.

### void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
#### Params
- `target`: Specifies the target to which the buffer object is bound for `glBufferData`, which must be one of the buffer binding targets of the table (same options as the glBindBuffer function).
- `buffer`: Specifies the name of the buffer object for `glNamedBufferData` function.
- `size`: Specifies the size in bytes of the buffer object's new data store.
- `data`: Specifies a pointer to data that will be copied into the data sotre for initialization, or NULL if no data is to be copied.
- `usage`: Specifies the expected pattern of the data store, The symbolic constant must be *GL_STREAM_DRAW*, *GL_STREAM_READ*, *GL_STREAM_COPY*, *GL_STATIC_DRAW*, *GL_STATIC_READ*, *GL_STATIC_COPY*, *GL_DYNAMIC_DRAW*, *GL_DYNAMIC_READ*, or *GL_DYNAMIC_COPY*.

#### Description
Create a new data store for a buffer object. In case of `glBufferData`, the buffer object currently bound to `target` is used.
While creating the new storage, any pre-existing data store is deleted. The new data sotre is created with the specified `size` in bytes and `usage`. If `data` is not NULL, the data sotre is initialized with data from this pointer. In its initial state, the new data sotre is not mapped, it has a NULL, mapped pointer, and its mapped access is *GL_READ_WRITE*.

`usage` is a hint to the GL implementation as to how a buffer object's data store will be accessed. This enables the GL implementation to make more intelligent decisions that may significantly impact buffer object performance. It does not, however, constrain the actual usage of the data store. It does not, however, constrain the actual usage of the data store. `usage` can be brokken down into two parts: first, the frequency of access (modification and usage), and second, the nature of that access. The frequency of access may be one of these:

**STREAM**: The data store contents will be modified once and used at most a few times.

**STATIC**: The data store contents will be modified once and used many times.

**DYNAMIC**: The data store contents will be modified repatedly and used many times.

The nature of access may be one of these:

**DRAW**: The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.

**READ**: The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.

**COPY**: The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.

#### void glEnableVertexAttribArray(GLuint index)
#### Parameters
- `index`: Specifies the index of the generic vertex attribute to be enabled or disabled.

#### Description
Enable the generic vertex attribute array specified by `index`. It uses currently bound vertex array object for the operation.

### void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
#### Parameters
- `index`: Specifies the index of the generic vertex attribute to be modified.
- `size`:  How much indexes each vertex has.
- `type`: Specifies the data type of each components in the array.
- `stride`: Specifies the offset of each vertex. (vertex 1 starts at 0 bytes, vertex 2 starts at 4 bytes...).
- `pointer`: pointer to an offset of positions inside the vertex (when starts the position, when the texture starts...).

#### Description
Specify the location and data format of the array of generic vertex attributes at index `index` to use when rendering. `size` specifies the number of components per attribute and must be 1, 2, 3, 4 or *GL_BGRA*. `type` specifies the data type of each component, and `stride` specifies the byte stride from one attribute to the next, allowing vertices and attributes to be packed into a single array or stored in separarate arrays.

If `pointer` is not NULL, a non-zero named buffer object must be bound to the *GL_ARRAY_BUFFER* target, otherwise an error is generated. `pointer` is treated as a byte offset into the buffer object's data store. The buffer object binding (*GL_ARRAY_BUFFER_BINDING*) is saved as generic vertex attribute array state(*GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING*) for index `index`.

### void glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* lenght)
#### Parameters
- `shader`: Specfies the handle of the shader object whose source code is to be replaced.
- `count`: Specifies the number of elements in the `string` and `length` arrays.
- `string`: Specifies an array of pointers to strings containing the source code to be loaded into the shader.
- `length`: Specifies an array of string lengths.

#### Description
Sets the soruce code in `shader` to the source code in the array of strings specified by `string`. Any source code previously stored in the shader object is completely replaced. The number of strings in the array is specified by `count`. If `length` is NULL, each string is assumed to be null terminated. If `length` is a value other than NULL, it points to an array containing a string length for each of the corresponding string (the null character is not counted as part of the string length) or a value less than 0 to indicate that the string is null terminated. The source code strings are note scanned or pased at this time; they are simply copied into the specified shader object.

### void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
#### Parameters
- `mode`: Specifies what kind of primitives to render. Symbolic constants *GL_POINTS*, *GL_LINE_STRIP*, *GL_LINE_LOOP*, *GL_LINES*, *GL_LINE_STRIP_ADJACENCY*, *GL_LINES_ADJACENCY*, *GL_TRIANGLE_STRIP*,*GL_TRIANGLE_FAN*, *GL_TRIANGLES*, *GL_TRIANGLE_STRIP_ADJACENCY*, *GL_TRIANGLES_ADJACENCY* and *GL_PATCHES* are accepted.
- `count`: Specifies the number of elements to be rendered.
- `type`: Specifies the type of the values in `indices`. Must be one of *GL_UNSIGNED_BYTE*, *GL_UNSIGNED_SHORT* or *GL_UNSIGNED_INT*.
- `indices`: Specifies an offest of the first index in the array in the data store of the buffer currently bound to the GL_ELEMENT_ARRAY_BUFFER target.

#### Description
Specifies multiple geometric primitives with very few subroutine calls. Instead of calling a GL function to pass each individual vertex, normal, texture coordinate, edge flag or color, you can prespecify  separate arrays of vertices, normals, and so on, and use them to construct a sequence of primitives with a single call to `glDrawElements`.

When `glDrawElements` is called, it uses `count` sequencial elements from an enabled array, starting at `indicies` to construct a sequence of geometric primitives. `mode` specifies what kind of primitives are constructed and how the array elements construct these primitives. If more than one array is enabled, each is used.

Vertex attributes that are modified by `glDrawElements` have an unspecified value after `glDrawElements` returns. Attributes that aren't modified maintain their previous values.

### void glTextParameteri(GLenum target, GLenum pname, GLint param)
#### Parameters
- `target`: Specifies the target to which the texture is bound for `glTexParameter`functions.
- `texture`: Specifies the texture object name for `glTextureParameters` functions.
- `pname (parameter name)`: Specifies the symbolic name of a single-valued texture parameter.
- `param`: For the scalar commands, specifies the value of `pname`

#### Description
Assign the value or values in `params` to the texture parameter specified as `pname`. For `glTexParameter`, `target` defines the target texture, either `GL_TEXTURE_1D`, `GL_TEXTURE_1D_ARRAY`, `GL_TEXTURE_2D`, `GL_TEXTURE_2D_ARRAY`, `GL_TEXTURE_2D_MULTISAMPLE`, `GL_TEXTURE_2D_MULTISAMPLE_ARRAY`, `GL_TEXTURE_3D`, `GL_TEXTURE_CUBE_MAP`, `GL_TEXTURE_CUBE_MAP_ARRAY`, or `GL_TEXTURE_RECTANGLE`. The following symbols are accepted in `pname` (not all are here, only the ones I already used):

*GL_TEXTURE_MIN_FILTER*

The texture minifying function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified. There are six defined minifying functions. Two of them use either the nearest texture elements or a weighted average of multiple texture elements to compute the texture value. The other four use mipmaps.

A mipmap is an ordered set of arrays representing the same image at progressively lower resolutions. If the texture has dimensions 2 ^ n * 2 ^ m, there are *max(n,m) + 1* mipmaps. The first mipmap is the original texture, with dimensions 2 ^ n * 2 ^ m. Each subsequent mipmap has dimensions 2 ^ (k???1) * 2 ^ (l???1), where 2 ^ k * 2 ^ l are the dimensions of the previous mipmap, until either k = 0 or l = 0. At that point, subsequent mipmaps have dimension 1 * 2 ^ (k-1) * 1 until the final mipmap, which has dimension 1 * 1. To define the mipmaps, call `glTexImage1D`, `glTexImage2D`, `glTexImage3D`, `glCopyTexImage1D`, or `glCopyTexImage2D` with the *level* argument indicating the order of the mipmaps. Level 0 is the original texture; level max(n,m) is the final 1 * 1 mipmap.

`params` supplies a function for minifying the texture as one of the following:
- *GL_LINEAR*: Returns the weighted avarage of the four texture elements that are closest to the specified texture coordinates. These can include items wrapped or repeated from other parts of a texture, depending on the values of *GL_TEXTURE_WRAP_S* and *GL_TEXTURE_WRAP_T*, and on the exact mapping. (maintain proportion?)

*GL_TEXTURE_WRAP_S*

Sets the wrap parameter for texture coordinate s to either *GL_CLAMP_TO_EDGE*, *GL_CLAMP_TO_BORDER*, *GL_MIRRORED_REPEAT*, *GL_REPEAT*, or *GL_MIRROR_CLAMP_TO_EDGE*. *GL_CLAMP_TO_EDGE* causes s coordinates to be clamped to the range [1/2N,1 - 1/2N], where N is the size of the texture in the direction of clamping. *GL_CLAMP_TO_BORDER* evaluates s coordinates in a similar manner to *GL_CLAMP_TO_EDGE*

### void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data)
#### Parameters
- `target`: Specifies the target texture.
- `level`: Specifies the level-of-detail number. Level 0 is the base image level. Level *n* is the *n*th mipmap reduction image. If `target` is *GL_TEXTURE_RECTANGLE* or *GL_PROXY_TEXTURE_RECTANGLE*, `level` must be 0.
- `internalFormat`: Specifies the number of color components in the texture. Must be onde of base internal formats given in Table 1, one of the sized internal formats given in Table 2, or one of the compressed internal formats given in Table 3.
- `width`: Specifies the width of the texture image. All implementations support texture images that are at least 1024 texels wide.
- `height`: Specifies the height of the texture image, or the number of layers in a texture array, in the case of the *GL_TEXTURE_1D_ARRAY* and *GL_PROXY_TEXTURE_1D_ARRAY* targets. All implementations support 2D texture images that are at least 1024 texels high, and texture arrays that are at least 256 layers deep.
- `border`: this value must be 0. (?????)
- `format`: Specifies the format of the pixel data. The following symbolic values are accepcted: GL_RED, GL_RG, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_BGR_INTEGER, GL_RGBA_INTEGER, GL_BGRA_INTEGER, GL_STENCIL_INDEX, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL.
- `type`: Specifies the data type of the pixel data.
- `data`: Specifies a pointer to the image data in memory.

#### Description
Texturing allows elements of an image array to be read by shaders.

To define texture images, call `glTexImage2D`. The arguments describe the parameters of the texture image, such as height, width, width of the border, LOD number (see glTexParameter), and number of color components provided. The last three arguments describe how the image is represented in memory.