# Computer Graphics Concepts
Study of main concepts of CG in order to learn better how to use OpenGL.
## Textures and Sampling
In order to get more detail in the object's color, a texture is used to pick color values from. Instead of creating multiple vertices to define various set of colors, it's much better to have an image mapped to our object and pick the colors from that image. Images can be regarded as mathematical functions and in fact you do not have to use an image, but could just sample some interesting function.

Definitions:
- Sampling: process of getting a finite number of values from a function, map, image.
- Texture: an image meant for the storage of some information that is later mapped to an object.
- UV Mapping: the mapping between texture coordinates (uv) and vertex coordinates (xyz).
- Interpolation: process of finding a previously unknown value between a number of known values.
- Linear Interpolation: an interpolation technique that assumes there is a straight line between the known values, the unknown is taken from that line.
- Nearest Neighbour Interpolation: an interpolation technique that takes the nearest known value to be the unknown value.

### Graphics Rendering Pipeline
It's the most important concept in real time graphics. For geometry to be displayed to the screen it must go through each stage of the graphics pipeline. Pipelines operate in a first in first out fashion. Elements entering the first stage of a pipeline cannot move to the second stage until the elements already processed by the second stage move to the third and so on. Like the pipelines in the physical world, the graphics rendering pipeline processes geometry passes through various stages until this geometry exits the pipeline in the form of a pixel ready to be output to the screen of the device.

The graphics rendering pipeline can be divided into three conceptual categories, the application stage, the geometry stage and the rasterizer stage. Each one of these three conceptual stages can be implemented as a pipeline itself containing functional stages and each functional stage can be divided into pipeline stages, Functional stages perform certain tasks but do not define the way they are executed in the pipeline. Pipeline stages are executed in parallel with other pipeline stages and can also be parallelized. How the task of a functional stage is executed depends on the implementation, for example the geometry stage can be divided into five funcional stages, but the implementation decides how many pipeline stages will exist in each of the functional stages and how many of them will run in parallel.

### Level of detail (LOD)
Refers to the complexity of a 3D model representation. LOD can be decreased as the model moves away from the viewer or according to other metrics such as object importance, viewpoint-relative speed or position. LOD techniques increase the efficiency of rendering by decreasing the workload on graphics pipeline stages, usually vertex transformations. The reduced visual quality of the model is often unnoticed because of the small effect on object appearance when distant or moving fast.

### Mipmap
Are used for:
- Level of Detail (LOD);
- Improving image quality. Rendering from large textures where only small, discontiguous subsets of texels are used can easily produce Moiré patterns;
- Speeding up rendering times, either by reducing the number of texels sampled to render each pixel, or increasing the memory locality of the samples taken;
- Reducing stress on the GPU or CPU.

Each bitmap image of the mipmap set is a downsized duplicate of the main texture, but at a certain reduced level of detail. Although the main texture would still be used when the view is sufficient to render it in full detail, the renderer will switch to a suitable mipmap image (or in fact, interpolate between the two nearest, if trillinear filtering is activated), when the texture is viewed from a distance or at a small size. Rendering speed increases since the number of texture pixels (texels) being processed per display pixel can be much lower for similar results with the simpler mipmap textures. If using a limited number of texture samples per display pixel (as in the case with bilinear filtering) then artifacts are reduced since the mipmap images are effectively already anti-aliased. Scaling down and up is made more efficient with mipmaps as well.

## Projection
How we go from having an arbitrary coordinate system in our 3D world to map it to our window, maths that converts 3D points in the space into something that is in a 2D window.

Convert the positions we have into normalized device coordinates.
Normalized space: coordinate system between -1 and 1.

Projection let's u decide what ur coordinate space is.

### Ortographic
Usually used for 2D games or UI.
In OpenGL, we have the coordinate system between -1 and 1. To convert it to a better system, we use some matrix, as is shown bellow that simulates what happens in the GPU side (GLSL)
```cpp
// matrix with x, y and z values wanted for the new resolution scale
glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
// coordinates where we want to draw the vertice (values of x, y, z and w)
glm::vec4 positions(100.0f, 100.0f, 0.0f, 1.0f);

// now multiplying to convert the values to a scale OpenGL understands (-1 to 1)
glm::vec4 result = projection * positions;
```
This operation will result in something between -1 and 1.

### Perspective
Usually 3D.

## Maths: Basics
Mahts are used a lot for mainly transformations on Computer Graphics.
For C++ with OpenGL, we can use the lib GLM (OpenGL Mathematics) for handling with maths.
### Vectors

### Matrices
**Model View Projection Matrices**: Model * View * Projection. The product of this operation is a matrix that will be used in the multiplication of the positions given by the vertex.

Each matrix has its use. The projection we already covered, but here is the definition of the Model and View matrices:
Transform - Position, rotation, translation.
- View matrix (a.k.a eye matrix): simulates a camera with transform properties.
- Model matrix: simulate the model (vertex) we're drawing in a matrix. Transform properties.
## Maths: Distances
### Manhattan distance
The manhattan distance between two vectors is equal to the one-norm of the distance between the vectors. The distance function (also called a "metric") involved is also called the "taxi cab" metric.

## Rendering
### Blending
Determines how we combine our output color with what is already in our target buffer.
- Output: the color we output from our fragment shader (known as source);
- Target buffer: the buffer our fragment shader is drawing to (known as destination).

How the blending is made:
**OpenGL**:
```cpp
// enable the blending
glEnable(GL_BLEND);
/*
** src = how the src RGBA factor is computed (default is GL_ONE) 
** dest = how the dest RGBA factor is computed (default is GL_ZERO)
*/
glBlendFunc(src, dest);
/*
** mode = how we combine the src and dest colors (default is GL_FUNC_ADD)
*/
glBlendEquation(mode); 
```
So what this means by default is:
- 1 + 0 = 1
- Use the source value

As an example, I rendered the triskle png image using:
- src = GL_SRC_ALPHA
- dest = GL_ONE_MINUS_SRC_ALPHA

So if the pixel I was rendering from the texture is transparent:
- src alpha = 0;
- dest = 1 - 0 = 1
- Which means "use the destination color" - the color that's already in the buffer.

The equation would be something like that:
```
R = (r_src * 0) + (r_dest * (1 - 0)) = r_dest
G = (g_src * 0) + (g_dest * (1 - 0)) = g_dest
B = (b_src * 0) + (b_dest * (1 - 0)) = b_dest
A = (a_src * 0) + (a_dest * (1 - 0)) = a_dest
```

Now a more interesting example.

Our pixel is partially transparent, with the properties of (1.0, 1.0, 1.0, 0.5) (RGBA), it's a white but translucent. Our destination buffer is cleared to magenta (1.0, 0.0, 1.0, 1.0). So using the same blending setting as above the equation would result:

```
R = (1.0 * 0.5) + (1.0 * (1 - 0.5)) = 1.0
G = (1.0 * 0.5) + (0.0 * (1 - 0.5)) = 0.5
B = (1.0 * 0.5) + (1.0 * (1 - 0.5)) = 1.0
A = (0.5 * 0.5) + (1.0 * (1 - 0.5)) = 0.75
```
The result would be a good example of a glass. The white square becomes a bright pink as a result of the blending.