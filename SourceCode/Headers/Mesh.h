#pragma once
#include <Model.h>
#include <Camera.h>
#include <Texture.h>


namespace Resources
{
	class Mesh
	{
	public:
		maths::Mat4 MVPMatrix;

		Mesh();
		Mesh(Model* model, const int id);
		Mesh(Model* model, Texture* texture);


		void SetCurrentModel(Model *model);
		void SetTexture(const int id);
		void SetTexturePtr(Texture* texture);

		Model* GetCurModel();
		GLuint GetIdTexture();
		Texture* GetTexture();

		void Update(maths::Mat4 modelMatrix, LowRenderer::Camera camera);

	private:
		int idTexture;
		Model* curModel;
		Texture* texture;
	};
}