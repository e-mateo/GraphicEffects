#pragma once
#include <Shape.h>


namespace maths
{
	enum TypeCollider
	{
		NOCOLLIDER,
		SPHERE,
		CUBE,
		CAPSULE,
	};

	class Collider
	{
	public:
		TypeCollider type;

		Vec3 velocity;
		Mat4 localMatrix;
		Mat4 worldMatrix;

		Shape::Cube cubeCollider;
		Shape::Sphere sphereCollider;
		Shape::Capsule capsuleCollider;
	};

	bool CollisionPointVsCube(Vec3& point, Shape::Cube& cube);

	Vec3 FindExitPoint(const Vec3 dir, const Shape::Sphere& sphere, const Shape::Cube& cube);
	Vec3 CollisionSphereVsSphere(const Shape::Sphere &s1, const Shape::Sphere &s2, bool& hasCollided);
	Vec3 CollisionAABBVsAABB(const Shape::Cube& c1, const Shape::Cube& c2, const Vec3 velocity, bool& hasCollided);
	Vec3 CollisionSphereVsCapsule(const Shape::Sphere& sphere, const Shape::Capsule& capsule, bool& hasCollided);
	Vec3 CollisionCapsuleVsCapsule(const Shape::Capsule& capsule, const Shape::Capsule& capsule2, bool& hasCollide);
	Vec3 CollisionSphereVsCube(const Shape::Sphere &sphere, Shape::Cube &cube, const Vec3 velocity, bool& hasCollided);
	Vec3 CollisionCubeVsCapsule(Shape::Cube& cube, Shape::Capsule& capsule, const Vec3 velocity, bool& hasCollide);
}