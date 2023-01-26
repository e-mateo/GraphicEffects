#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>

#include <App.h>


int Core::App::Init(const AppInitializer init)
{
	width = init.width;
	height = init.height;

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, init.OpenGl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, init.OpenGl_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(init.width, init.height, init.name, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, init.framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	GLint flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(init.glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Camera
	inputs = LowRenderer::CameraInputs();
	camera = LowRenderer::Camera((float)init.width, (float)init.height);
	ui = UI((float)init.width, (float)init.height);
	glfwGetCursorPos(window, &inputs.mouseX, &inputs.mouseY);

	// ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	threadpool.StartThread();
	skybox.LoadSkybox();
	LoadShaders();

	return 1;
}

void Core::App::LoadShaders()
{
	Resources::Shader* cartoon = resourcesManager.Create<Resources::Shader>("cartoonShader");
	std::filesystem::path cartoonV("Resources/Shaders/cartoonShader.vert");
	std::filesystem::path cartoonF("Resources/Shaders/cartoonShader.frag");
	ASSERT(cartoon->SetVertexShader(cartoonV));
	ASSERT(cartoon->SetFragmentShader(cartoonF));
	ASSERT(cartoon->Link());

	Resources::Shader* normal = resourcesManager.Create<Resources::Shader>("normalShader");
	std::filesystem::path normalV("Resources/Shaders/normalShader.vert");
	std::filesystem::path normalF("Resources/Shaders/normalShader.frag");
	ASSERT(normal->SetVertexShader(normalV));
	ASSERT(normal->SetFragmentShader(normalF));
	ASSERT(normal->Link());

	Resources::Shader* reflection = resourcesManager.Create<Resources::Shader>("reflectionShader");
	std::filesystem::path reflectionV("Resources/Shaders/reflectionShader.vert");
	std::filesystem::path reflectionF("Resources/Shaders/reflectionShader.frag");
	ASSERT(reflection->SetVertexShader(reflectionV));
	ASSERT(reflection->SetFragmentShader(reflectionF));
	ASSERT(reflection->Link());

	Resources::Shader* refraction = resourcesManager.Create<Resources::Shader>("refractionShader");
	std::filesystem::path refractionV("Resources/Shaders/refractionShader.vert");
	std::filesystem::path refractionF("Resources/Shaders/refractionShader.frag");
	ASSERT(refraction->SetVertexShader(refractionV));
	ASSERT(refraction->SetFragmentShader(refractionF));
	ASSERT(refraction->Link());

	Resources::Shader* skybox = resourcesManager.Create<Resources::Shader>("skyboxShader");
	std::filesystem::path skyboxV("Resources/Shaders/skyboxShader.vert");
	std::filesystem::path skyboxF("Resources/Shaders/skyboxShader.frag");
	ASSERT(skybox->SetVertexShader(skyboxV));
	ASSERT(skybox->SetFragmentShader(skyboxF));
	ASSERT(skybox->Link());
}

void Core::App::Update()
{
	// Inputs camera
	glfwPollEvents();
	processInput(window);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(!ui.isInStart())
		skybox.Draw(camera,resourcesManager, ui.isInCartoonScene());

	if (ui.isInGame())
		scene.Update(resourcesManager);

	SetupShader();
	Render();

	ui.DrawUI(window, scene, resourcesManager, threadpool, inputs);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
}

void Core::App::SetupShader()
{
	if (ui.isInCartoonScene())
	{
		shaderProgram = resourcesManager.Get<Resources::Shader>("cartoonShader")->shaderProgram;
		glUseProgram(shaderProgram);

		scene.LoadLightsCartoonScene(shaderProgram);
		maths::Vec3 camPos = camera.GetPosition();
		glUniform1f(glGetUniformLocation(shaderProgram, "enableText"), ui.enableText);
		glUniform1f(glGetUniformLocation(shaderProgram, "doToon"), ui.doToon);
		glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), camPos.x, camPos.y, camPos.z);
		glUniform1i(glGetUniformLocation(shaderProgram, "nbDirLights"), LowRenderer::Light::nbDirLight);
		glUniform3f(glGetUniformLocation(shaderProgram, "hotColor"), ui.hot[0], ui.hot[1], ui.hot[2]);
		glUniform3f(glGetUniformLocation(shaderProgram, "coldColor"), ui.cold[0], ui.cold[1], ui.cold[2]);
		glUniform1f(glGetUniformLocation(shaderProgram, "alpha"), ui.alphaFactor);
		glUniform1f(glGetUniformLocation(shaderProgram, "beta"), ui.betaFactor);
	}
	else
	{
		shaderProgram = resourcesManager.Get<Resources::Shader>("normalShader")->shaderProgram;
		glUseProgram(shaderProgram);

		scene.LoadLightsSkyboxScene(shaderProgram);
		maths::Vec3 camPos = camera.GetPosition();
		glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), camPos.x, camPos.y, camPos.z);
		glUniform1i(glGetUniformLocation(shaderProgram, "nbDirLights"), LowRenderer::Light::nbDirLight);
		glUniform1i(glGetUniformLocation(shaderProgram, "nbPointsLights"), 0);
		glUniform1i(glGetUniformLocation(shaderProgram, "nbSpotsLights"), 0);
	}
}

void Core::App::Render()
{
	if (ui.isInCartoonScene())
	{
		// Draw Outlines
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(7);
		glUniform1i(glGetUniformLocation(shaderProgram, "drawLine"), true);
		for (int i = 0; i < scene.sceneGraph.root->children.size(); i++)
			DrawMesh(shaderProgram, scene.sceneGraph.root->children[i]);
	}

	// Draw Mesh
	glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniform1i(glGetUniformLocation(shaderProgram, "drawLine"), false);
	for (int i = 0; i < scene.sceneGraph.root->children.size(); i++)
		DrawMesh(shaderProgram, scene.sceneGraph.root->children[i]);
}

void Core::App::DrawMesh(const int shaderProgram, Core::DataStructure::GameObject* node)
{
	//Update Matrix//
	node->UpdateLocalTransform();
	node->mesh->Update(node->worldTransform, camera);

	//DRAW OBJECTS//
	glBindTexture(GL_TEXTURE_2D, node->mesh->GetIdTexture());
	if (node->mesh->GetCurModel()->isInit)
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &node->worldTransform.matrixArray[0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mvp"), 1, GL_FALSE, &node->mesh->MVPMatrix.matrixArray[0]);
		glBindVertexArray(node->mesh->GetCurModel()->GetVAO());
		glDrawArrays(GL_TRIANGLES, 0, 3 * (GLsizei)node->mesh->GetCurModel()->GetVertexBuffer().size());
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	for (int i = 0; i < node->children.size(); i++)
		DrawMesh(shaderProgram, node->children[i]);
}

void Core::App::Unload()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	//Unload scene
	if (ui.isInCartoonScene())
		scene.UnloadScene(1, resourcesManager, threadpool);
	else
		scene.UnloadScene(0, resourcesManager, threadpool);

	//delete ptr node
	scene.sceneGraph.Delete(scene.sceneGraph.root);

	//Delete Shader
	resourcesManager.Delete("cartoonShader");
	resourcesManager.Delete("normalShader");
	resourcesManager.Delete("skyboxShader");
	resourcesManager.Delete("reflectionShader");
	resourcesManager.Delete("refractionShader");
}

void Core::App::processInput(GLFWwindow* window)
{
	double deltaTime = clock() - oldTime;
	oldTime = clock();

	if (!ui.isInStart() && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && scene.chronoEnded)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		ui.SetInMenu(true);
		ui.SetInGame(false);
	}

	if (ui.isInGame())
	{
		//Keyboard Inputs
		inputs.jump = false;
		inputs.moveLeft = false;
		inputs.moveRight = false;
		inputs.moveForward = false;
		inputs.moveBackward = false;

		if (glfwGetKey(window, inputs.moveForwardInput[0]) == GLFW_PRESS) //Forward
			inputs.moveForward = true;

		if (glfwGetKey(window, inputs.moveBackwardInput[0]) == GLFW_PRESS) //Backward
			inputs.moveBackward = true;

		if (glfwGetKey(window, inputs.moveLeftInput[0]) == GLFW_PRESS) //Left
			inputs.moveLeft = true;

		if (glfwGetKey(window, inputs.moveRightInput[0]) == GLFW_PRESS) //Right
			inputs.moveRight = true;

		if (glfwGetKey(window, 32) == GLFW_PRESS) //Down
			inputs.jump = true;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			camera.moveCamera = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && !camera.isLocked && !camera.isFocused)
		{
			camera.moveCamera = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			if (camera.canUnlock)
			{
				camera.isFocused = false;
				camera.canUnlock = false;
				camera.isLocked = !camera.isLocked;

				if (camera.isLocked)
				{
					camera.moveCamera = true;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
				else
				{
					camera.moveCamera = false;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}
			}
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		{
			camera.canUnlock = true;
		}

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		{
			camera.isLocked = false;
			camera.isFocused = true;
			camera.moveCamera = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		{
			camera.isFocused = false;
		}

		//Mouse Inputs
		double newMouseX, newMouseY;
		glfwGetCursorPos(window, &newMouseX, &newMouseY);
		inputs.mouseDeltaX = (float)(newMouseX - inputs.mouseX);
		inputs.mouseDeltaY = (float)(newMouseY - inputs.mouseY);
		inputs.mouseX = newMouseX;
		inputs.mouseY = newMouseY;

		//Update Camera
		Core::DataStructure::GameObject* player = scene.sceneGraph.Get("Player");
		camera.Update(deltaTime, camera.moveCamera, inputs, player->collider.type, player->position, player->scale);
		scene.player.Update(deltaTime, camera.isFocused, camera.isLocked, inputs, camera);
	}
}