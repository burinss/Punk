#include "PTexture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

PTexture::PTexture()
{
	
}

PTexture::PTexture(const glm::vec4 color)
{
	Load(color);
}

void PTexture::Load(const std::string& path, const std::string& directory, PTextureWrapping wrapping, bool hdr, bool flip)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	glGenTextures(1, &id);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format=0;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
}

void PTexture::Load(const glm::vec4 color)
{
	GLubyte textureData[] = { color.x, color.y, color.z, color.w };
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
}

void PTexture::Resize(unsigned int width, unsigned int height, bool multisample)
{
	GLenum target = multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

	glBindTexture(target, id);

	if (multisample)
	{
		glTexImage2DMultisample(target, 4, GL_RGB, width, height, GL_TRUE);
	}
	else
	{
		glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}

	glBindTexture(target, 0);
}

void PTexture::Bind(unsigned int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, id);
}

void PTexture::LoadColor(const glm::vec2& size, bool multisample)
{

	type = "color";
	GLenum target = multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

	glGenTextures(1, &id);
	glBindTexture(target, id);

	if (multisample)
	{
		glTexImage2DMultisample(target, 4, GL_RGB, size.x, size.y, GL_TRUE);
	}
	else
	{
		glTexImage2D(target, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}

	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(target, 0);
}

void PTexture::LoadDepth(const glm::vec2& size)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.f, 1.f, 1.f, 1.f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindTexture(GL_TEXTURE_2D, 0);
}

std::shared_ptr<PTexture> PTexture::Create(const std::string& path, const std::string& directory, PTextureWrapping wrapping, bool hdr, bool flip)
{
	std::shared_ptr<PTexture> texture = std::make_shared<PTexture>();
	texture->Load(path, directory);
    return texture;
}

std::shared_ptr<PTexture> PTexture::CreateColor(const glm::vec2& size, bool multisample)
{
	std::shared_ptr<PTexture> texture = std::make_shared<PTexture>();
	texture->LoadColor(size, multisample);
	return texture;
}

std::shared_ptr<PTexture> PTexture::CreateDepth(const glm::vec2& size)
{
	std::shared_ptr<PTexture> texture = std::make_shared<PTexture>();
	texture->LoadDepth(size);
	return texture;
}
