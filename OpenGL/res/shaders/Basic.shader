#shader vertex
#version 330 core
// accessing the 2 floats from a vertex (location equals the index passed on glVertexAttribPointer function
layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.5, 0.0, 0.5, 1.0);
};