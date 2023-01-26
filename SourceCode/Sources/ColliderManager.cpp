#include <vector>

#include <ColliderManager.h>


void Physics::ColliderManager::GetAllGameObject(Core::DataStructure::GameObject* root)
{
	allObject.clear();

	for (int i = 0; i < root->children.size(); i++)
		GetAllChildrens(root->children[i]);
}

void Physics::ColliderManager::Clear()
{
	allObject.clear();
}

void Physics::ColliderManager::GetAllChildrens(Core::DataStructure::GameObject* parent)
{
	allObject.push_back(parent);

	for (int i = 0; i < parent->children.size(); i++)
		GetAllChildrens(parent->children[i]);
}

void Physics::ColliderManager::SetPlayer(Core::DataStructure::GameObject* p)
{
	playerGO = p;
}

bool Physics::ColliderManager::DoCollisions(Gameplay::Player& player)
{
	bool oneCollision = false;

	for (int i = 0; i < allObject.size(); i++)
	{
		if (allObject[i] == playerGO)
			continue;

		bool hasCollided = false;

		std::vector<Core::DataStructure::GameObject*>::iterator it;

		if (playerGO->collider.type == maths::TypeCollider::SPHERE)
		{
			if (allObject[i]->collider.type == maths::TypeCollider::SPHERE)
			{
				maths::Vec3 dir = maths::CollisionSphereVsSphere(playerGO->collider.sphereCollider, allObject[i]->collider.sphereCollider, hasCollided);
				playerGO->Update(dir, maths::Vec3(0, 0, 0), maths::Vec3(0, 0, 0));
			}
			else if (allObject[i]->collider.type == maths::TypeCollider::CUBE)
			{
				maths::Vec3 dir = maths::CollisionSphereVsCube(playerGO->collider.sphereCollider, allObject[i]->collider.cubeCollider, playerGO->collider.velocity, hasCollided);
				playerGO->Update(dir, maths::Vec3(0, 0, 0), maths::Vec3(0, 0, 0));
			}
			else if (allObject[i]->collider.type == maths::TypeCollider::CAPSULE)
			{
				maths::Vec3 dir = maths::CollisionSphereVsCapsule(playerGO->collider.sphereCollider, allObject[i]->collider.capsuleCollider, hasCollided);
				playerGO->Update(dir, maths::Vec3(0, 0, 0), maths::Vec3(0, 0, 0));
			}
		}
		else if (playerGO->collider.type == maths::TypeCollider::CUBE)
		{
			if (allObject[i]->collider.type == maths::TypeCollider::SPHERE)
			{
				maths::Vec3 dir = maths::CollisionSphereVsCube(allObject[i]->collider.sphereCollider, playerGO->collider.cubeCollider, playerGO->collider.velocity, hasCollided);
				playerGO->Update(-1 * dir, maths::Vec3(0, 0, 0), maths::Vec3(0, 0, 0));
			}
			else if (allObject[i]->collider.type == maths::TypeCollider::CUBE)
			{
				maths::Vec3 dir = maths::CollisionAABBVsAABB(playerGO->collider.cubeCollider, allObject[i]->collider.cubeCollider, playerGO->collider.velocity, hasCollided);
				playerGO->Update(dir, maths::Vec3(0, 0, 0), maths::Vec3(0, 0, 0));
			}
			else if (allObject[i]->collider.type == maths::TypeCollider::CAPSULE)
			{
				maths::Vec3 dir = maths::CollisionCubeVsCapsule(playerGO->collider.cubeCollider, allObject[i]->collider.capsuleCollider, playerGO->collider.velocity, hasCollided);
				playerGO->Update(dir, maths::Vec3(0, 0, 0), maths::Vec3(0, 0, 0));
			}
		}
		else if (playerGO->collider.type == maths::TypeCollider::CAPSULE)
		{
			if (allObject[i]->collider.type == maths::TypeCollider::SPHERE)
			{
				maths::Vec3 dir = maths::CollisionSphereVsCapsule(allObject[i]->collider.sphereCollider, playerGO->collider.capsuleCollider, hasCollided);
				playerGO->Update(-1 * dir, maths::Vec3(0, 0, 0), maths::Vec3(0, 0, 0));
			}
			else if (allObject[i]->collider.type == maths::TypeCollider::CUBE)
			{
				maths::Vec3 dir = maths::CollisionCubeVsCapsule(allObject[i]->collider.cubeCollider, playerGO->collider.capsuleCollider, playerGO->collider.velocity, hasCollided);
				playerGO->Update(-1 * dir, maths::Vec3(0, 0, 0), maths::Vec3(0, 0, 0));
			}
			else if (allObject[i]->collider.type == maths::TypeCollider::CAPSULE)
			{
				maths::Vec3 dir = maths::CollisionCapsuleVsCapsule(playerGO->collider.capsuleCollider, allObject[i]->collider.capsuleCollider, hasCollided);
				playerGO->Update(dir, maths::Vec3(0, 0, 0), maths::Vec3(0, 0, 0));
			}
		}

		if (hasCollided)
			oneCollision = true;
	}

	return oneCollision;
}