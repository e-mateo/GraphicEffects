#pragma once
#include <Mesh.h>
#include <Maths.h>
#include <Debug.h>
#include <Model.h>
#include <Light.h>
#include <Player.h>
#include <Shader.h>
#include <Texture.h>
#include <SceneGraph.h>
#include <ThreadPool.h>
#include <ResourceManager.h>
#include <ColliderManager.h>


namespace Resources
{
	class Scene
	{
		int modelInit;
		int textureInit;
		int curIdTexture;

		std::vector<Mesh *> meshes;
		std::vector<Model *> models;
		std::vector<Texture *> textures;

		void Binding(ResourceManager& resourcesManager);

		// Create
		void CreateSphereCollider(int radius, maths::Vec3 pos, Model* modelCollider, GameObject* object);
		void CreateCubeCollider(maths::Vec3 pos, maths::Vec3 size, Model* modelCollider, GameObject* object);
		void CreateModel(std::string name, std::string filePath, ResourceManager& resourcesManager, ThreadPool& threadpool);
		void CreateTexture(std::string name, std::string filePath, ResourceManager& resourcesManager, ThreadPool& threadpool);
		Mesh* CreateMesh(Model* model, Texture* texture, ResourceManager& resourcesManager);
		Core::DataStructure::GameObject* CreateGameObject(std::string name, maths::Vec3 _position, maths::Vec3 _rotation, maths::Vec3 _scale, Mesh* mesh, Core::DataStructure::GameObject* parent);

		// Loads & Unload
		void LoadMeshesSkyBoxScene(ResourceManager& resourcesManager);
		void LoadLDMeshesSkyBoxScene(ResourceManager& resourcesManager);
		void LoadModelsSkyBoxScene(ResourceManager& resourcesManager, ThreadPool& threadpool);
		void LoadTexturesSkyBoxScene(ResourceManager& resourcesManager, ThreadPool& threadpool);
		void UnloadSkyBoxScene(ResourceManager& resourcesManager);

		void LoadMeshesCartoonScene(ResourceManager& resourcesManager);
		void LoadLDMeshesCartoonScene(ResourceManager& resourcesManager);
		void LoadModelsCartoonScene(ResourceManager& resourcesManager, ThreadPool& threadpool);
		void LoadTexturesCartoonScene(ResourceManager& resourcesManager, ThreadPool& threadpool);
		void UnloadCartoonScene(ResourceManager& resourcesManager);

	public:
		Scene();

		bool chronoEnded;
		bool cubeCollider;

		Gameplay::Player player;
		Physics::ColliderManager colliderManager;
		Core::DataStructure::SceneGraph sceneGraph;

		void LoadScene(int sceneIndex, ResourceManager& resourcesManager, ThreadPool& threadpool);
		void Update(ResourceManager& resourcesManager);
		void UnloadScene(int index, ResourceManager& resourcesManager, ThreadPool& threadpool);

		// Loads
		void LoadLightsSkyboxScene(const int shaderProgram);
		void LoadLightsCartoonScene(const int shaderProgram);

		// Getter
		inline std::vector<Mesh*> GetMeshes() { return meshes; }
		inline std::vector<Model*> GetModels() { return models; }
		inline std::vector<Texture*> GetTextureList() { return textures; }
	};
}