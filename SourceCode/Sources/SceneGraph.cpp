#include <SceneGraph.h>


namespace Core::DataStructure
{
	SceneGraph::SceneGraph()
	{
		root = new GameObject();
		root->worldTransform = maths::Mat4::identity();
	}

	void SceneGraph::CreateNewRoot()
	{
		root = new GameObject();
		root->worldTransform = maths::Mat4::identity();
	}

	void GameObject::SetChild(GameObject* child)
	{
		child->parent = this;
		children.push_back(child);
	}

	void GameObject::UpdateLocalTransform()
	{
		localTranform = maths::Mat4::CreateTransformMatrix(rotation, position, scale);

		if (collider.type == maths::TypeCollider::SPHERE)
			collider.localMatrix = maths::Mat4::CreateTransformMatrix(maths::Vec3(0, 0, 0), collider.sphereCollider.pos, maths::Vec3(collider.sphereCollider.radius, collider.sphereCollider.radius, collider.sphereCollider.radius));
		if (collider.type == maths::TypeCollider::CUBE)
			collider.localMatrix = maths::Mat4::CreateTransformMatrix(maths::Vec3(0, 0, 0), collider.cubeCollider.pos, maths::Vec3(collider.cubeCollider.width, collider.cubeCollider.height, collider.cubeCollider.depth));
		if (collider.type == maths::TypeCollider::CAPSULE)
			collider.localMatrix = maths::Mat4::CreateTransformMatrix(maths::Vec3(0, 0, 0), collider.capsuleCollider.pos, maths::Vec3(collider.capsuleCollider.radius, collider.capsuleCollider.height, collider.capsuleCollider.radius));
	}

	void GameObject::Update(maths::Vec3 _dir, maths::Vec3 _rot, maths::Vec3 _scale)
	{
		position = position + _dir;
		rotation = rotation + _rot;
		scale = scale + _scale;

		if (collider.type == maths::TypeCollider::CUBE)
		{
			collider.cubeCollider.pos = collider.cubeCollider.pos + _dir;
			collider.cubeCollider.width += _scale.x;
			collider.cubeCollider.height += _scale.y;
			collider.cubeCollider.depth += _scale.z;
		}

		if (collider.type == maths::TypeCollider::SPHERE)
			collider.sphereCollider.pos = collider.sphereCollider.pos + _dir;

		if (collider.type == maths::TypeCollider::CAPSULE)
			collider.capsuleCollider.pos = collider.capsuleCollider.pos + _dir;
	}

	void CalculateWorldMatrix(GameObject* node)
	{
		node->worldTransform = node->parent->worldTransform * node->localTranform;
		node->collider.worldMatrix = node->parent->worldTransform * node->collider.localMatrix;

		for (int i = 0; i < node->children.size(); i++)
			CalculateWorldMatrix(node->children[i]);
	}

	void SceneGraph::UpdateMatrix()
	{
		for (int i = 0; i < root->children.size(); i++)
			CalculateWorldMatrix(root->children[i]);
	}

	GameObject* SceneGraph::Get(const std::string name)
	{
		auto iterator = nodeManager.find(name);

		if (iterator != nodeManager.end())
		{
			return iterator->second;
		}
		else
		{
			std::cout << "Didn't Find :" << name << "\n";
			return nullptr;
		}
	}

	void SceneGraph::Delete(GameObject* node)
	{
		for (int i = 0; i < node->children.size(); i++)
			Delete(node->children[i]);

		delete node;
		nodeManager.clear();
	}
}