#pragma once
#include <String>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <UI.h>
#include <Mesh.h>
#include <Scene.h>
#include <Model.h>
#include <Light.h>
#include <Shader.h>
#include <Skybox.h>
#include <Camera.h>
#include <SceneGraph.h>
#include <ThreadPool.h>
#include <ResourceManager.h>


namespace Core
{
	struct AppInitializer
	{
		unsigned int OpenGl_major;
		unsigned int OpenGl_minor;
		unsigned int width;
		unsigned int height;
		const char* name;
		void (*framebuffer_size_callback)(GLFWwindow* window, int width, int height);
		void (*glDebugOutput)(GLenum source, GLenum type, GLuint id, GLenum severity,
			GLsizei length, const GLchar* message, const void* userParam);
	};

	class App
	{
		bool drawLine;

		int width;
		int height;
		int shaderProgram;

		double oldTime = 0;

		UI ui;
		Skybox skybox;
		GLFWwindow* window;
		ThreadPool threadpool;
		Resources::Scene scene;
		LowRenderer::Camera camera;
		LowRenderer::CameraInputs inputs;
		Resources::ResourceManager resourcesManager;
		
		void Render();
		void LoadShaders();
		void SetupShader();
		void processInput(GLFWwindow* window);
		void DrawMesh(const int shaderProgram, Core::DataStructure::GameObject* node);

	public:

		App() {};

		int Init(const AppInitializer init);

		void Update();
		void Unload();

		inline GLFWwindow* GetWindow() { return window; }
		inline Resources::ResourceManager& GetResourceManager() { return resourcesManager; }
	};
}