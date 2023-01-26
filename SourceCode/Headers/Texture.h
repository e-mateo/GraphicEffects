#pragma once
#include <String>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB_Image/stb_image.h>

#include <ResourceManager.h>


namespace Resources
{
	class Texture : public IResource
	{
	public:
		Texture();

		unsigned char* data;

		int width;
		int height; 
		int nrChannels;

		GLuint texture;
		GLuint sampler;

		void BindSampler();
		void GenAndBindTexture();
		void UnloadResource() override;
		void LoadResource(const std::string fileName) override;

		GLuint GetId();
	};
}