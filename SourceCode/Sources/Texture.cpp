#include <Texture.h>


using namespace Resources;

Texture::Texture()
{
	width = 0;
	height = 0;
	texture = 0;
	sampler = 0;
	nrChannels = 0;
	data = nullptr;

	isInit = false;
	isLoaded = false;
	loadFunc = std::bind(&IResource::LoadResource, this, std::placeholders::_1);
}

void Texture::LoadResource(const std::string fileName)
{
	data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
	isLoaded = true;
}

void Texture::GenAndBindTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if (nrChannels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	else if (nrChannels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	GLint max = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max);
	glBindTexture(GL_TEXTURE_2D, 0);

	BindSampler();
	stbi_image_free(data);

	isInit = true;
}

void Texture::BindSampler()
{
	glGenSamplers(1, &sampler);
	glBindSampler(texture, sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.f);
	glBindSampler(texture, 0);
}

void Texture::UnloadResource()
{
	glDeleteTextures(1, &texture);
	glDeleteSamplers(1, &sampler);
}

GLuint Texture::GetId()
{
	return texture;
}