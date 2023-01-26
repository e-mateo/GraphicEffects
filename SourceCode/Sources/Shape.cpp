#include <Shape.h>


Shape::Sphere::Sphere()
{
	radius = 0;
	model = nullptr;
	pos = maths::Vec3(0, 0, 0);
}

Shape::Sphere::Sphere(float radiusSphere, maths::Vec3 posSphere, Resources::Model* modelSphere)
{
	pos = posSphere;
	model = modelSphere;
	radius = radiusSphere;
}

Shape::Cube::Cube()
{
	width = 0;
	height = 0;
	depth = 0;
	model = nullptr;
	pos = maths::Vec3(0,0,0);
}

Shape::Cube::Cube(maths::Vec3 position, maths::Vec3 dimension, Resources::Model* modelCube)
{
	pos = position;
	model = modelCube;
	width = dimension.x;
	height = dimension.y;
	depth = dimension.z;
}

Shape::Capsule::Capsule()
{
	radius = 0;
	height = 0;
	model = nullptr;
	pos = maths::Vec3(0, 0, 0);
}

Shape::Capsule::Capsule(float radiusCapsule, float heightCapsule, maths::Vec3 posCapsule, Resources::Model* modelCapsule)
{
	pos = posCapsule;
	radius = radiusCapsule;
	height = heightCapsule;
	model = modelCapsule;
}