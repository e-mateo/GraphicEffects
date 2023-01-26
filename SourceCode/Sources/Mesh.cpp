#include <Mesh.h>


using namespace Resources;

Mesh::Mesh() 
{
	curModel = nullptr; 
	MVPMatrix = maths::Mat4::identity();
}

Mesh::Mesh(Model* model, const int id)
{
	curModel = model;
	idTexture = id;
	MVPMatrix = maths::Mat4::identity();
}

Mesh::Mesh(Model* model, Texture* texture)
{
	curModel = model;
	this->texture = texture;
	MVPMatrix = maths::Mat4::identity();
}

void Mesh::SetCurrentModel(Model *model)
{
	curModel = model;
}

void Mesh::SetTexture(const int id)
{
	idTexture = id;
}

void Mesh::Update(maths::Mat4 modelMatrix, LowRenderer::Camera camera)
{
	MVPMatrix = camera.GetVPMatrix() * modelMatrix;
}

Model* Mesh::GetCurModel()
{
	return curModel;
}

GLuint Mesh::GetIdTexture()
{
	return texture->GetId();
}

void Mesh::SetTexturePtr(Texture* texture)
{
	this->texture = texture;
}

Texture* Mesh::GetTexture()
{
	return texture;
}