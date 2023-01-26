#pragma once
#include <UI.h>
#include <Model.h>
#include <Camera.h>
#include <Shader.h>
#include <SceneGraph.h>
#include <ResourceManager.h>




class Skybox
{
public:
	Skybox();
	~Skybox();

	unsigned int cubemapSkyboxScene;
	unsigned int cubemapCartoonScene;

	Resources::Model* model;
	Resources::Model* reflectionModel;

	void LoadSkybox();
	void LoadCubeMap(std::vector<std::string> faces, unsigned int& cubeMap);
	void Draw(LowRenderer::Camera &camera, Resources::ResourceManager& resourcesManager, bool isInCartoonScene);
};