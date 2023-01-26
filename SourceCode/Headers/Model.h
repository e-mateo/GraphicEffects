#pragma once
#include <vector>
#include <fstream>
#include <iostream>  
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ResourceManager.h>
#include <Maths.h>
#include <Debug.h>


namespace Resources
{
	struct Vertex
	{
		maths::Vec3 position;
		maths::Vec3 normal;
		maths::Vec2 textureUV;
	};

	class Model : public IResource
	{
	public:
		Model();
		unsigned int GetVAO();
		std::vector<Vertex> GetVertexBuffer();

		void BindBuffer();
		void CreateCube();
		void CreateSkybox();
		void UnloadResource() override;
		void LoadResource(const std::string fileName) override;
		void CreateSphere(const int lat, const int lon, float radius);
		void CreateCapsule(const float height, const float radius, const int sides);

	private:
		unsigned int VBO;
		unsigned int VAO;
		std::vector<Vertex> vertexBuffer;

		void StoreDatas(const std::string type, std::vector<std::string> splitLine, std::vector<maths::Vec3>& pos, std::vector<maths::Vec3>& normal, std::vector<maths::Vec2>& uv, std::vector<maths::Vec3>& index);
		void StoreIndexList(std::vector<std::string> splitLine, std::vector<maths::Vec3>& index);
		void DatasToVertexBuffer(const std::vector<maths::Vec3> pos, const std::vector<maths::Vec3> normal, const std::vector<maths::Vec2> uv, const std::vector<maths::Vec3> index);
	};
}