#pragma once
#include <maths.h>
#include <Model.h>


namespace Shape
{
	class Cube
	{
	public:
		float width;
		float height;
		float depth;
		maths::Vec3 pos;
		maths::Vec3 closestPoint;
		Resources::Model* model;

		Cube();
		Cube(maths::Vec3 position, maths::Vec3 dimension, Resources::Model* modelCube = nullptr);
	};

	class Sphere
	{
	public:
		float radius;
		maths::Vec3 pos;
		Resources::Model* model;

		Sphere();
		Sphere(float radiusSphere, maths::Vec3 posSphere, Resources::Model* modelSphere = nullptr);
	};

	class Capsule
	{
	public:
		float radius;
		float height;
		maths::Vec3 pos;
		Resources::Model* model;

		Capsule();
		Capsule(float radiusCapsule, float heightCapsule, maths::Vec3 posCapsule, Resources::Model* modelCapsule = nullptr);
	};
}