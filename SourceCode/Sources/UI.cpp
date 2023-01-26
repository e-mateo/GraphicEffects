#include <UI.h>

UI::UI(float _width, float _height)
{
	width = _width;
	height = _height;

	shaderName = "Gooch Shader";

	doToon = true;
	inGame = false;
	inMenu = false;
	inStart = true;
	enableText = true;
	alwaysOpen = true;
	inSettings = false;
	inCartoonScene = false;

	betaFactor = 0.6f;
	alphaFactor = 0.4f;

	hot[0] = 0.47f, hot[1] = 0.12f, hot[2] = 0.08f;
	cold[0] = 0.f, cold[1] = 0.0f, cold[2] = 0.78f;
}

void UI::DrawUI(GLFWwindow* window, Resources::Scene& scene, Resources::ResourceManager& resourcesManager, ThreadPool& threadpool, LowRenderer::CameraInputs& inputs)
{
	if (inStart)
		UIStartMenu(window, scene, resourcesManager, threadpool);

	if (inGame)
		UIInGame();

	if (inMenu)
	{
		UIMenu(window, scene, threadpool, resourcesManager);
		if (inSettings)
			UISettings(inputs);
	}
}

void UI::UIInGame()
{
	if (inCartoonScene)
	{
		ImGui::SetNextWindowSize(ImVec2((float)(320), (float)(650)));
		ImGui::SetNextWindowPos(ImVec2(20, 20));

		ImGui::Begin("Settings", &alwaysOpen);

		ImGui::Checkbox("Enable textures", &enableText);
		if (ImGui::Button(shaderName, ImVec2(200, 30)))
		{
			if (doToon)
			{
				doToon = false;
				shaderName = "Toon Shader";
			}
			else
			{
				doToon = true;
				shaderName = "Gooch Shader";
			}
		}

		if (!doToon)
		{
			ImGui::ColorPicker3("Hot", hot);
			ImGui::ColorPicker3("Cold", cold);
			ImGui::SliderFloat("BetaFactor", &betaFactor, 0.f, 1.f);
			ImGui::SliderFloat("AlphaFactor", &alphaFactor, 0.f, 1.f);
		}
		ImGui::End();
	}
}

void UI::UIStartMenu(GLFWwindow* window, Resources::Scene& scene, Resources::ResourceManager& resourcesManager, ThreadPool& threadpool)
{
	float winWidth = 320.f;
	float winHeight = 351.f;

	ImGui::SetNextWindowPos(ImVec2((float)(width / 2 - winWidth / 2), (float)(height / 2 - winHeight / 2)));
	ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));

	ImGui::Begin("Welcome to Mystery Machine Surfers", &alwaysOpen);

	ImGui::SetCursorPos(ImVec2(8.f, 27.f));
	if (ImGui::Button("Skybox Scene", ImVec2(304.f, 100.f)))
	{
		inCartoonScene = false;
		CloseStartMenu(window);
		scene.LoadScene(0,resourcesManager, threadpool);
	}

	ImGui::SetCursorPos(ImVec2(8.f, 135.f));
	if (ImGui::Button("NPR Scene", ImVec2(304.f, 100.f)))
	{
		inCartoonScene = true;
		CloseStartMenu(window);
		scene.LoadScene(1,resourcesManager, threadpool);
	}

	ImGui::SetCursorPos(ImVec2(8.f, 243.f));
	if (ImGui::Button("QUIT", ImVec2(304.f, 100.f)))
		glfwSetWindowShouldClose(window, true);

	ImGui::End();
}

void UI::CloseStartMenu(GLFWwindow* window)
{
	inGame = true;
	inMenu = false;
	inStart = false;
	inSettings = false;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void UI::UISettings(LowRenderer::CameraInputs& inputs)
{
	float winWidth = 320.f;
	float winHeight = 650.f;
	float winWidthM = 316.f;

	ImGui::SetNextWindowPos(ImVec2((float)(width / 2 - winWidthM / 2 + winWidthM + 8), (float)(height / 2 - winHeight / 2)));
	ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight));

	ImGui::Begin("KEY BINDINGS", &alwaysOpen);

	ImGui::InputTextWithHint("Forward", "EnterText", inputs.moveForwardInput, IM_ARRAYSIZE(inputs.moveForwardInput));
	ImGui::InputTextWithHint("Backward", "EnterText", inputs.moveBackwardInput, IM_ARRAYSIZE(inputs.moveBackwardInput));
	ImGui::InputTextWithHint("Left", "EnterText", inputs.moveLeftInput, IM_ARRAYSIZE(inputs.moveLeftInput));
	ImGui::InputTextWithHint("Right", "EnterText", inputs.moveRightInput, IM_ARRAYSIZE(inputs.moveRightInput));

	//To upper case
	if (inputs.moveForwardInput[0] >= 97 && inputs.moveForwardInput[0] <= 122)
		inputs.moveForwardInput[0] -= 32;
	if (inputs.moveBackwardInput[0] >= 97 && inputs.moveBackwardInput[0] <= 122)
		inputs.moveBackwardInput[0] -= 32;
	if (inputs.moveLeftInput[0] >= 97 && inputs.moveLeftInput[0] <= 122)
		inputs.moveLeftInput[0] -= 32;
	if (inputs.moveRightInput[0] >= 97 && inputs.moveRightInput[0] <= 122)
		inputs.moveRightInput[0] -= 32;

	ImGui::End();
}

void UI::UIMenu(GLFWwindow* window, Resources::Scene& scene, ThreadPool& threadpool, Resources::ResourceManager& resourcesManager)
{
	float winHeight = 459.f;
	float winWidthM = 316.f;

	ImGui::SetNextWindowPos(ImVec2((float)(width / 2 - winWidthM / 2), (float)(height / 2 - winHeight / 2)));
	ImGui::SetNextWindowSize(ImVec2(winWidthM, winHeight));

	ImGui::Begin("PAUSE", &alwaysOpen);

	ImGui::SetCursorPos(ImVec2(8.f, 27.f));
	if (ImGui::Button("RESUME", ImVec2(300.f, 100.f)))
	{
		inGame = true;
		inMenu = false;
		inSettings = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	ImGui::SetCursorPos(ImVec2(8.f, 135.f));
	if (ImGui::Button("MAIN MENU", ImVec2(300.f, 100.f)))
	{
		threadpool.DeleteJobs();
		scene.sceneGraph.Delete(scene.sceneGraph.root);
		scene.sceneGraph.CreateNewRoot();

		if (inCartoonScene)
			scene.UnloadScene(1,resourcesManager, threadpool);
		else
			scene.UnloadScene(0, resourcesManager, threadpool);

		inGame = false;
		inMenu = false;
		inStart = true;
		inSettings = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	ImGui::SetCursorPos(ImVec2(8.f, 243.f));
	if (ImGui::Button("SETTINGS", ImVec2(300.f, 100.f)))
	{
		if (!inSettings)
			inSettings = true;
		else
			inSettings = false;
	}

	ImGui::SetCursorPos(ImVec2(8.f, 351.f));
	if (ImGui::Button("QUIT", ImVec2(300.f, 100.f)))
		glfwSetWindowShouldClose(window, true);

	ImGui::End();
}