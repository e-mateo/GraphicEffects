#include <chrono>
#include <functional>

#include <Scene.h>
#include <Shape.h>


using namespace maths;
using namespace Resources;
using namespace Core::DataStructure;

////////// CREATE SCENE //////////

Scene::Scene()
{
	chronoEnded = false;
	cubeCollider = false;

	modelInit = 0;
	textureInit = 0;
	curIdTexture = 0;
}

void Scene::CreateModel(std::string name, std::string filePath, ResourceManager& resourcesManager, ThreadPool& threadpool)
{
	Model* newModel = resourcesManager.Create<Model>(name);
	models.push_back(newModel);

	threadpool.AddQueue(Task(newModel->loadFunc, filePath));
}

void Scene::CreateTexture(std::string name, std::string filePath, ResourceManager& resourcesManager, ThreadPool& threadpool)
{
	Texture* newText = resourcesManager.Create<Texture>(name);
	textures.push_back(newText);

	threadpool.AddQueue(Task(newText->loadFunc, filePath));
}

Mesh* Scene::CreateMesh(Model* model, Texture* texture, ResourceManager& resourcesManager)
{
	Mesh* newMesh = new Mesh(model, texture);
	meshes.push_back(newMesh);
	return newMesh;
}

void Scene::CreateSphereCollider(int radius, Vec3 pos, Model* modelCollider, GameObject* object)
{
	Shape::Sphere collider((float)radius, pos, modelCollider);
	object->collider.sphereCollider = collider;
	object->collider.type = TypeCollider::SPHERE;
}

void Scene::CreateCubeCollider(Vec3 pos, Vec3 size, Model* modelCollider, GameObject* object)
{
	Shape::Cube collider(pos, size, modelCollider);
	object->collider.cubeCollider = collider;
	object->collider.type = TypeCollider::CUBE;
}

GameObject* Scene::CreateGameObject(std::string name, Vec3 _position, Vec3 _rotation, Vec3 _scale, Mesh* mesh, GameObject* parent)
{
	GameObject* newGO = new GameObject();
	newGO->name = name;
	newGO->mesh = mesh;
	newGO->scale = _scale;
	newGO->position = _position;
	newGO->rotation = _rotation;
	newGO->UpdateLocalTransform();

	parent->SetChild(newGO);
	sceneGraph.nodeManager[name] = newGO;

	return newGO;
}

/////////// LOADS ///////////

void Scene::Binding(ResourceManager& resourcesManager)
{
	for (int i = 0; i < models.size(); ++i)
	{
		if (models[i]->isLoaded && !models[i]->isInit)
		{
			models[i]->BindBuffer();
			modelInit++;
		}
	}

	for (int i = 0; i < textures.size(); ++i)
	{
		if (textures[i]->isLoaded && !textures[i]->isInit)
		{
			textures[i]->GenAndBindTexture();
			textureInit++;
		}
	}

	if (!chronoEnded)
		if (modelInit == models.size() && textureInit == textures.size())
			chronoEnded = true;
}
	
void Scene::LoadScene(int index, ResourceManager& resourcesManager, ThreadPool& threadpool)
{
	if (index == 0)
	{
		LoadTexturesSkyBoxScene(resourcesManager, threadpool);
		LoadModelsSkyBoxScene(resourcesManager, threadpool);
		LoadMeshesSkyBoxScene(resourcesManager);
	}
	else if (index == 1)
	{
		LoadTexturesCartoonScene(resourcesManager, threadpool);
		LoadModelsCartoonScene(resourcesManager, threadpool);
		LoadMeshesCartoonScene(resourcesManager);
	}
}

/////////// SKYBOX SCENE ///////////

void Scene::LoadModelsSkyBoxScene(ResourceManager &resourcesManager, ThreadPool& threadpool)
{
	Model* cube = resourcesManager.Create<Model>("Cube");
	cube->CreateCube();
	cube->BindBuffer();

	Model* classicSphere = resourcesManager.Create<Model>("Sphere");
	classicSphere->CreateSphere(20, 20, 1);
	classicSphere->BindBuffer();
}

void Scene::LoadTexturesSkyBoxScene(ResourceManager &resourcesManager, ThreadPool& threadpool)
{
	stbi_set_flip_vertically_on_load(true);

	CreateTexture("GroundText", "Resources/Textures/ground.jpg", resourcesManager, threadpool);
}

void Scene::LoadMeshesSkyBoxScene(ResourceManager &resourcesManager)
{
	LoadLDMeshesSkyBoxScene(resourcesManager);

	Mesh* playerMesh = CreateMesh(resourcesManager.Get<Model>("Sphere"), resourcesManager.Get<Texture>("GroundText"), resourcesManager);

	Vec3 scale = Vec3(0, 0, 0);
	Vec3 position = Vec3(0, 0, 0);
	Vec3 rotation = Vec3(0, 0, 0);
	GameObject* playerGO = CreateGameObject("Player", position, rotation, scale, playerMesh, sceneGraph.root);
	CreateSphereCollider(25, position + Vec3(0, 10, 0), resourcesManager.Get<Model>("Sphere"), playerGO);

	colliderManager.SetPlayer(sceneGraph.Get("Player"));
	player.gameObject = sceneGraph.Get("Player");
	player.position = sceneGraph.Get("Player")->position;
}

void Scene::LoadLDMeshesSkyBoxScene(ResourceManager& resourcesManager)
{
	Mesh* cubeMesh = CreateMesh(resourcesManager.Get<Model>("Cube"), resourcesManager.Get<Texture>("GroundText"), resourcesManager);
	GameObject* Cube = CreateGameObject("Cube", Vec3(-100, -10, -100), Vec3(0, 0, 0), Vec3(300, 10, 300), cubeMesh, sceneGraph.root);
	CreateCubeCollider(Cube->position, Cube->scale, resourcesManager.Get<Model>("Cube"), Cube);
}

////////// CARTOON SCENE //////////

void Scene::LoadModelsCartoonScene(ResourceManager& resourcesManager, ThreadPool& threadpool)
{
	Model* cube = resourcesManager.Create<Model>("Cube");
	cube->CreateCube();
	cube->BindBuffer();

	Model* classicSphere = resourcesManager.Create<Model>("Sphere");
	classicSphere->CreateSphere(25, 25, 1);
	classicSphere->BindBuffer();
}

void Scene::LoadTexturesCartoonScene(ResourceManager& resourcesManager, ThreadPool& threadpool)
{
	stbi_set_flip_vertically_on_load(true);

	CreateTexture("Red", "Resources/Textures/red.png", resourcesManager, threadpool);
	CreateTexture("Green", "Resources/Textures/green.jpg", resourcesManager, threadpool);
	CreateTexture("Black", "Resources/Textures/black.png", resourcesManager, threadpool);
	CreateTexture("Peach", "Resources/Textures/peach.jpg", resourcesManager, threadpool);
	CreateTexture("White", "Resources/Textures/white.png", resourcesManager, threadpool);
	CreateTexture("Magenta", "Resources/Textures/magenta.jpg", resourcesManager, threadpool);
	CreateTexture("FloorText", "Resources/Textures/gazon.png", resourcesManager, threadpool);
	CreateTexture("Turquoise", "Resources/Textures/turquoise.png", resourcesManager, threadpool);
}

void Scene::LoadMeshesCartoonScene(ResourceManager& resourcesManager)
{
	LoadLDMeshesCartoonScene(resourcesManager);

	Mesh* playerMesh = CreateMesh(resourcesManager.Get<Model>("Sphere"), resourcesManager.Get<Texture>("Peach"), resourcesManager);

	Vec3 scale = Vec3(0, 0, 0);
	Vec3 rotation = Vec3(0, 180, 0);
	Vec3 position = Vec3(50, 10, 350);
	GameObject* playerGO = CreateGameObject("Player", position, rotation, scale, playerMesh, sceneGraph.root);
	CreateSphereCollider(10, position + Vec3(0, 10, 0), resourcesManager.Get<Model>("Sphere"), playerGO);

	colliderManager.SetPlayer(sceneGraph.Get("Player"));
	player.gameObject = sceneGraph.Get("Player");
	player.position = sceneGraph.Get("Player")->position;
}

void Scene::LoadLDMeshesCartoonScene(ResourceManager& resourcesManager)
{
	// Meshes
	Vec3 v0x0x0 = Vec3(0, 0, 0);
	Vec3 v15x15x15 = Vec3(15, 15, 15);

	Mesh* cubeMesh = CreateMesh(resourcesManager.Get<Model>("Cube"), resourcesManager.Get<Texture>("FloorText"), resourcesManager);
	GameObject* Cube = CreateGameObject("Cube", Vec3(-100, 0, 100), v0x0x0, Vec3(300, 10, 300), cubeMesh, sceneGraph.root);
	CreateCubeCollider(Cube->position, Cube->scale, resourcesManager.Get<Model>("Cube"), Cube);

	Mesh* cubeMesh1 = CreateMesh(resourcesManager.Get<Model>("Cube"), resourcesManager.Get<Texture>("Green"), resourcesManager);
	GameObject* cube1 = CreateGameObject("Cube1", Vec3(30, 15, 225), v0x0x0, Vec3(30,30,30), cubeMesh1, sceneGraph.root);

	Mesh* sphereMesh = CreateMesh(resourcesManager.Get<Model>("Sphere"), resourcesManager.Get<Texture>("White"), resourcesManager);
	GameObject* Sphere = CreateGameObject("Sphere", Vec3(-50, 35, 150), v0x0x0, v15x15x15, sphereMesh, sceneGraph.root);

	Mesh* sphereMesh1 = CreateMesh(resourcesManager.Get<Model>("Sphere"), resourcesManager.Get<Texture>("Peach"), resourcesManager);
	GameObject* Sphere1 = CreateGameObject("Sphere1", Vec3(150, 35, 150), v0x0x0, v15x15x15, sphereMesh1, sceneGraph.root);

	Mesh* sphereMesh2 = CreateMesh(resourcesManager.Get<Model>("Sphere"), resourcesManager.Get<Texture>("Turquoise"), resourcesManager);
	GameObject* Sphere2 = CreateGameObject("Sphere2", Vec3(-50, 35, 250), v0x0x0, v15x15x15, sphereMesh2, sceneGraph.root);

	Mesh* sphereMesh3 = CreateMesh(resourcesManager.Get<Model>("Sphere"), resourcesManager.Get<Texture>("Magenta"), resourcesManager);
	GameObject* Sphere3 = CreateGameObject("Sphere3", Vec3(150, 35, 250), v0x0x0, v15x15x15, sphereMesh3, sceneGraph.root);

	Mesh* sphereMesh4 = CreateMesh(resourcesManager.Get<Model>("Sphere"), resourcesManager.Get<Texture>("Red"), resourcesManager);
	GameObject* Sphere4 = CreateGameObject("Sphere4", Vec3(-50, 35, 350), v0x0x0, v15x15x15, sphereMesh4, sceneGraph.root);

	Mesh* sphereMesh5 = CreateMesh(resourcesManager.Get<Model>("Sphere"), resourcesManager.Get<Texture>("Black"), resourcesManager);
	GameObject* Sphere5 = CreateGameObject("Sphere5", Vec3(150, 35, 350), v0x0x0, v15x15x15, sphereMesh5, sceneGraph.root);
}

////////// LIGHTS //////////

void Scene::LoadLightsCartoonScene(const int shaderProgram)
{
	//Init numbers of lights
	LowRenderer::Light::nbDirLight = 0;

	// DIRECTIONAL LIGHT//
	LowRenderer::Light dirLight(Vec3(0.f, 0.f, 0.f), Vec3(1.0f, 1.0f, 1.0f), Vec3(1, 1, 1));
	dirLight.CreateDirectionalLight(Vec3(-0.25, -1, -0.5), shaderProgram);
}

void Scene::LoadLightsSkyboxScene(const int shaderProgram)
{
	//Init numbers of lights
	LowRenderer::Light::nbDirLight = 0;

	LowRenderer::Light globalLight(Vec3(0.3f, 0.3f, 0.3f), Vec3(0, 0, 0), Vec3(0, 0, 0));
	globalLight.CreateDirectionalLight(Vec3(0, 0, 0), shaderProgram);

	// DIRECTIONAL LIGHT//
	LowRenderer::Light dirLight(Vec3(0.f, 0.f, 0.f), Vec3(0.8f, 0.6f, 0.6f), Vec3(1, 1, 1));
	dirLight.CreateDirectionalLight(Vec3(0.5, -0.50, 1), shaderProgram);
}

////////// UPDATE SCENE ////////// 

void Scene::Update(ResourceManager& resourcesManager)
{
	Binding(resourcesManager);
	GameObject* playerGO = sceneGraph.Get("Player");

	if (cubeCollider)
	{
		playerGO->collider.type = maths::TypeCollider::CUBE;
		playerGO->collider.cubeCollider.pos = playerGO->position - Vec3(10, 0, 10);
	}
	else
	{
		playerGO->collider.type = maths::TypeCollider::SPHERE;
		playerGO->collider.sphereCollider.pos = playerGO->position + Vec3(0, 10, 0);
	}

	//Collisions//
	colliderManager.GetAllGameObject(sceneGraph.root);
	bool hasCollided = colliderManager.DoCollisions(player);
	player.position = playerGO->position;

	if (hasCollided)
		player.state = Gameplay::ONGROUND;
	else if (player.state != Gameplay::ONJUMP)
		player.state = Gameplay::ONJUMP;

	sceneGraph.UpdateMatrix();
}

////////// UNLOAD SCENE	 //////////
void Scene::UnloadScene(int index, ResourceManager& resourcesManager, ThreadPool& threadpool)
{
	if (index == 0)
		UnloadSkyBoxScene(resourcesManager);
	else
		UnloadCartoonScene(resourcesManager);

	for (int i = 0; i < meshes.size(); i++)
		delete meshes[i];

	//Nodes
	chronoEnded = false;

	modelInit = 0;
	textureInit = 0;

	meshes.clear();
	models.clear();
	textures.clear();
	colliderManager.Clear();
	sceneGraph.nodeManager.clear();

	threadpool.DeleteJobs();
}

void Scene::UnloadSkyBoxScene(ResourceManager& resourcesManager)
{
	//Models
	resourcesManager.Delete("Cube");
	resourcesManager.Delete("Sphere");

	//Textures
	resourcesManager.Delete("FloorText");
	resourcesManager.Delete("GroundText");
}

void Scene::UnloadCartoonScene(ResourceManager& resourcesManager)
{
	//Models
	resourcesManager.Delete("Cube");
	resourcesManager.Delete("Sphere");

	//Textures
	resourcesManager.Delete("Red");
	resourcesManager.Delete("Black");
	resourcesManager.Delete("Green");
	resourcesManager.Delete("Peach");
	resourcesManager.Delete("White");
	resourcesManager.Delete("Magenta");
	resourcesManager.Delete("FloorText");
	resourcesManager.Delete("Turquoise");
	resourcesManager.Delete("GroundText");
}