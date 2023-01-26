#pragma once
#include <vector>

#include <Mesh.h>
#include <maths.h>
#include <Collision.h>


namespace Core::DataStructure
{
	struct GameObject
	{
	public:
		maths::Vec3 position;
		maths::Vec3 rotation;
		maths::Vec3 scale;

		maths::Mat4 localTranform;
		maths::Mat4 worldTransform;

		GameObject* parent = nullptr;
		std::vector<GameObject*> children;

		Resources::Mesh* mesh;
		std::string name;

		maths::Collider collider;

		void SetChild(GameObject* child);
		void UpdateLocalTransform();
		void Update(maths::Vec3 _dir, maths::Vec3 _rot, maths::Vec3 _scale);
	};

	class SceneGraph 
	{
	public:
		GameObject *root;
		std::map<std::string, GameObject*> nodeManager;

		SceneGraph();
		void UpdateMatrix();
		void CreateNewRoot();
		void Delete(GameObject* node);
		GameObject* Get(const std::string name);
	};
}