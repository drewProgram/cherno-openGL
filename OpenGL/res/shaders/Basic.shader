#shader vertex
#version 330 core

// accessing the 2 floats from a vertex (location equals the index passed on glVertexAttribPointer function
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

// output data to the fragment shader
out vec2 v_TexCoord;

// take the matrix from the CPU to the shader
// Model View Projection matrix
uniform mat4 u_MVP;

void main()
{
	// taking the vertex position and multiply with the projection matrix
	// moving to the appropriate space based on the orthographic matrix we provided
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
};