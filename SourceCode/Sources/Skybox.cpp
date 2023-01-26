#include <Skybox.h>


Skybox::Skybox()
{
    cubemapCartoonScene = NULL;
    cubemapSkyboxScene = NULL;
}

void Skybox::LoadCubeMap(std::vector<std::string> faces, unsigned int& cubeMap)
{
    glGenTextures(1, &cubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            if(nrChannels == 3)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            if (nrChannels == 4)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::LoadSkybox()
{
    model = new Resources::Model;
	model->CreateSkybox();
	model->BindBuffer();

    reflectionModel = new Resources::Model;
    reflectionModel->CreateCube();
    reflectionModel->BindBuffer();

    std::vector<std::string> skyboxFaces
    {
        "Resources/Skybox/right.jpg",
        "Resources/Skybox/left.jpg",
        "Resources/Skybox/top.jpg",
        "Resources/Skybox/bottom.jpg",
        "Resources/Skybox/front.jpg",
        "Resources/Skybox/back.jpg"
    };

    std::vector<std::string> nprFaces
    {
        "Resources/Skybox/toonBoxRight.jpg",
        "Resources/Skybox/toonBoxLeft.jpg",
        "Resources/Skybox/toonBoxTop.jpg",
        "Resources/Skybox/toonBoxBottom.jpg",
        "Resources/Skybox/toonBoxFront.jpg",
        "Resources/Skybox/toonBoxBack.jpg"
    };

    LoadCubeMap(skyboxFaces, cubemapSkyboxScene);
    LoadCubeMap(nprFaces, cubemapCartoonScene);
}

void Skybox::Draw(LowRenderer::Camera& camera, Resources::ResourceManager& resourcesManager, bool isInCartoonScene)
{
    int skyboxProgram = resourcesManager.Get<Resources::Shader>("skyboxShader")->shaderProgram;
    int reflectionProgram = resourcesManager.Get<Resources::Shader>("reflectionShader")->shaderProgram;
    int refractionProgram = resourcesManager.Get<Resources::Shader>("refractionShader")->shaderProgram;

    glDepthMask(GL_FALSE);
    glUseProgram(skyboxProgram);
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "viewMatrix"), 1, GL_FALSE, &camera.GetViewMatrix().matrixArray[0]);
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projMatrix"), 1, GL_FALSE, &camera.GetProjMatrix().matrixArray[0]);
    glBindVertexArray(model->GetVAO());
    if(isInCartoonScene)
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapCartoonScene);
    else
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapSkyboxScene);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);


    // Draw reflection and refraction cube
    if (!isInCartoonScene)
    {
        maths::Vec3 v0x0x0 = maths::Vec3(0.f, 0.f, 0.f);
        maths::Vec3 v50x50x50 = maths::Vec3(50.f, 50.f, 50.f);

        maths::Mat4 modelMatrix = maths::Mat4::CreateTransformMatrix(v0x0x0, maths::Vec3(-50, 20, -50), v50x50x50);
        modelMatrix.FillArray();
        glUseProgram(reflectionProgram);
        glUniformMatrix4fv(glGetUniformLocation(reflectionProgram, "model"), 1, GL_FALSE, &modelMatrix.matrixArray[0]);
        glUniformMatrix4fv(glGetUniformLocation(reflectionProgram, "view"), 1, GL_FALSE, &camera.GetViewMatrix().matrixArray[0]);
        glUniformMatrix4fv(glGetUniformLocation(reflectionProgram, "projection"), 1, GL_FALSE, &camera.GetProjMatrix().matrixArray[0]);
        glUniform3f(glGetUniformLocation(reflectionProgram, "cameraPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glBindVertexArray(reflectionModel->GetVAO());
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapSkyboxScene);
        glDrawArrays(GL_TRIANGLES, 0, 3 * (GLsizei)reflectionModel->GetVertexBuffer().size());

        maths::Mat4 modelMatrix2 = maths::Mat4::CreateTransformMatrix(v0x0x0, maths::Vec3(100, 20, 100), v50x50x50);
        modelMatrix2.FillArray();
        glUseProgram(refractionProgram);
        glUniformMatrix4fv(glGetUniformLocation(refractionProgram, "model"), 1, GL_FALSE, &modelMatrix2.matrixArray[0]);
        glUniformMatrix4fv(glGetUniformLocation(refractionProgram, "view"), 1, GL_FALSE, &camera.GetViewMatrix().matrixArray[0]);
        glUniformMatrix4fv(glGetUniformLocation(refractionProgram, "projection"), 1, GL_FALSE, &camera.GetProjMatrix().matrixArray[0]);
        glUniform3f(glGetUniformLocation(refractionProgram, "cameraPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glBindVertexArray(reflectionModel->GetVAO());
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapSkyboxScene);
        glDrawArrays(GL_TRIANGLES, 0, 3 * (GLsizei)reflectionModel->GetVertexBuffer().size());
    }
}

Skybox::~Skybox()
{
    delete model;
    delete reflectionModel;
}