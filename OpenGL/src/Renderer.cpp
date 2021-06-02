#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    // when there is no errors, glGetError returns 0
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    //GLCall(glClear(GL_COLOR_BUFFER_BIT));

	shader.Bind();
	va.Bind();
	ib.Bind();
	// as we already bound the ibo to the ELEMENT_ARRAY_BUFFER we can set the
    // const void* indices as null
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
