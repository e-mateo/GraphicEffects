#pragma once
#include <String.h>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ResourceManager.h>
#include <Debug.h>
#include <Mesh.h>


namespace Resources
{
	class Shader : public IResource
	{
	public:
		Shader();

		void LoadResource(const std::string fileName) override;
		void UnloadResource() override;

		bool Link();
		bool SetVertexShader(std::filesystem::path const& filename);
		bool SetFragmentShader(std::filesystem::path const& filename);

	private:
		int vertexShader;
		int fragmentShader;
		
		std::string shaderString;

	public:
		int shaderProgram;
	};
}