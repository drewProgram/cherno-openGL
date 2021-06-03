#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
	m_Width(0), m_Height(0), m_BPP(0)
{
	// Load image
	// flips upside down the image (pngs starts to loading at the upside left,
	// OpenGL needs to render on the bottom left (0.0, 0.0)
	stbi_set_flip_vertically_on_load(1);
	// we pass the file path and the width, height and BPP vars will be filled by this function with the
	// texture info
	// desired channel = rgba (4)
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	// Generate texture
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// Texture parameters
	// you need to specify this 4 parameters, otherwise you'll get a black texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	// S and T are like X and Y for textures
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Give OpenGL the data
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	// Unbinding texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	// Freeing data
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	// binding in some texture slot (by default we're binding at slot 0)
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
