#include <Collision.h>


namespace maths
{
	Vec3 CollisionSphereVsSphere(const Shape::Sphere &s1, const Shape::Sphere &s2, bool& hasCollided)
	{
		float distance = distanceBetweenTwoPoints(s1.pos, s2.pos);
		if (distance < (s1.radius + s2.radius))
		{
			hasCollided = true;
			Vec3 dir = (s1.pos - s2.pos).Normalize();
			dir = dir * (s1.radius + s2.radius - distance);
			return dir;
		}

		return Vec3(0,0,0);
	}

	Vec3 CalculateCubeDistance(const Shape::Cube& c1, const Shape::Cube& c2)
	{
		float dx = 0, dy = 0, dz = 0;

		if (c1.pos.x < c2.pos.x)
			dx = c2.pos.x - (c1.pos.x + c1.width);
		else if (c1.pos.x > c2.pos.x)
			dx = c1.pos.x - (c2.pos.x + c2.width);

		if (c1.pos.y < c2.pos.y)
			dy = c2.pos.y - (c1.pos.y + c1.height);
		else if (c1.pos.y > c2.pos.y)
			dy = c1.pos.y - (c2.pos.y + c2.height);

		if (c1.pos.z < c2.pos.z)
			dz = c2.pos.z - (c1.pos.z + c1.depth);
		else if (c1.pos.z > c2.pos.z)
			dz = c1.pos.z - (c2.pos.z + c2.depth);

		return Vec3(dx, dy, dz);
	}

	bool isColliding(float x1, float y1, float z1, float w1, float h1, float d1, float x2, float y2, float z2, float w2, float h2, float d2)
	{
		return (x1 < x2 + w2 && x2 < x1 + w1) &&
			   (y1 < y2 + h2 && y2 < y1 + h1) &&
			   (z1 < z2 + d2 && z2 < z1 + d1);
	}

	Vec3 CollisionAABBVsAABB(const Shape::Cube& c1, const Shape::Cube& c2, const Vec3 velocity, bool& hasCollided)
	{
		Shape::Cube oldFrame(c1.pos - velocity, Vec3(c1.width, c1.height, c1.depth), c1.model);
		bool collide = isColliding(c1.pos.x, c1.pos.y, c1.pos.z, c1.width, c1.height, c1.depth, c2.pos.x, c2.pos.y, c2.pos.z, c2.width, c2.height, c2.depth);

		if (collide)
		{
			hasCollided = true;
			Vec3 distance = CalculateCubeDistance(oldFrame, c2);
			float timeToCollideX = velocity.x != 0 ? Abs(distance.x / velocity.x) : 0;
			float timeToCollideY = velocity.y != 0 ? Abs(distance.y / velocity.y) : 0;
			float timeToCollideZ = velocity.z != 0 ? Abs(distance.z / velocity.z) : 0;

			float shortestTime = 0;
			float c1PosY = c1.pos.y;
			float c2PosMaxY = c2.pos.y + c2.height;

			Vec3 dir(0, 0, 0);

			if (velocity.x != 0 && velocity.y == 0 && velocity.z == 0)
			{
				// Collison on X only
				shortestTime = timeToCollideX;
				dir.x = shortestTime * velocity.x;
			}
			else if (velocity.x == 0 && velocity.y != 0 && velocity.z == 0)
			{
				// Collision on Y only
				shortestTime = timeToCollideY;
				dir.y = shortestTime * velocity.y;
			}
			else if (velocity.x == 0 && velocity.y == 0 && velocity.z != 0)
			{
				// Collision on Z only
				shortestTime = timeToCollideZ;
				dir.z = shortestTime * velocity.z;
			}
			else
			{
				if (velocity.x != 0 && velocity.y != 0 && velocity.z != 0) //Collision X, Y and Z
					shortestTime = Min(Abs(timeToCollideX), Abs(timeToCollideY), Abs(timeToCollideZ));
				else if (velocity.x != 0 && velocity.y != 0 && velocity.z == 0) //Collision X, Y
					shortestTime = Min(Abs(timeToCollideX), Abs(timeToCollideY));
				else if (velocity.x != 0 && velocity.y == 0 && velocity.z != 0) //Collision X, Z
					shortestTime = Min(Abs(timeToCollideX), Abs(timeToCollideZ));
				else if (velocity.x == 0 && velocity.y != 0 && velocity.z != 0) //Collision Y, Z
					shortestTime = Min(Abs(timeToCollideY), Abs(timeToCollideZ));


				dir.x = shortestTime * velocity.x;
				dir.y = shortestTime * velocity.y;
				dir.z = shortestTime * velocity.z;
			}
			return Vec3(dir - velocity);
		}
		else
		{
			return Vec3(0, 0, 0);
		}
	}

	bool CollisionPointVsCube(const Vec3 &point, const Shape::Cube &cube)
	{
		// X Axis
		if (point.x < cube.pos.x || cube.pos.x + cube.width < point.x)
			return false;

		// Y Axis
		if (point.y < cube.pos.y || cube.pos.y + cube.height < point.y)
			return false;

		// Z Axis
		if (point.z < cube.pos.z || cube.pos.z + cube.depth < point.z)
			return false;

		return true;
	}

	Vec3 GetGreatestVelocityNorm(Vec3 v)
	{
		if (Abs(v.y) < Abs(v.x) && Abs(v.z) < Abs(v.x))
			return Vec3(Sign(v.x), 0, 0);
		else if (Abs(v.x) < Abs(v.y) && Abs(v.z) < Abs(v.y))
			return Vec3(0, Sign(v.y), 0);
		else
			return Vec3(0, 0, Sign(v.z));
	}

	Vec3 FindExitPoint(const Vec3 dir, const Shape::Sphere& sphere, const Shape::Cube& cube)
	{
		if (dir.x == 1)
			return Vec3(cube.pos.x + cube.width, sphere.pos.y, sphere.pos.z);
		else if (dir.x == -1)
			return Vec3(cube.pos.x, sphere.pos.y, sphere.pos.z);
		else if (dir.y == 1)
			return Vec3(sphere.pos.x, cube.pos.y + cube.height, sphere.pos.z);
		else if (dir.y == -1)
			return Vec3(sphere.pos.x, cube.pos.y, sphere.pos.z);
		else if (dir.z == 1)
			return Vec3(sphere.pos.x, sphere.pos.y, cube.pos.z + cube.depth);
		else if (dir.z == -1)
			return Vec3(sphere.pos.x, sphere.pos.y, cube.pos.z);

		return Vec3(0, 0, 0);
	}


	Vec3 CollisionSphereVsCube(const Shape::Sphere &sphere, Shape::Cube &cube, const Vec3 velocity, bool& hasCollided)
	{
		// closest cube point to the sphere
		cube.closestPoint.x = max(cube.pos.x, min(sphere.pos.x, cube.pos.x + cube.width));
		cube.closestPoint.y = max(cube.pos.y, min(sphere.pos.y, cube.pos.y + cube.height));
		cube.closestPoint.z = max(cube.pos.z, min(sphere.pos.z, cube.pos.z + cube.depth));

		if (cube.closestPoint == sphere.pos && CollisionPointVsCube(sphere.pos, cube))
		{
			Vec3 dir = -1 * GetGreatestVelocityNorm(velocity);
			Vec3 exitPoint = FindExitPoint(dir, sphere, cube);
			float distance = distanceBetweenTwoPoints(exitPoint, sphere.pos) + sphere.radius;
			return distance * dir;
		}

		float distance = distanceBetweenTwoPoints(cube.closestPoint, sphere.pos);

		if (distance < sphere.radius)
		{
			hasCollided = true;
			Vec3 dir = (cube.closestPoint - sphere.pos).Normalize();
			if (dir.y >= -0.5f && dir.y <= 0.5f)
				dir.y = 0.f;

			if (CollisionPointVsCube(sphere.pos, cube))
				return (distance + sphere.radius) * dir * -1;
			else
				return (sphere.radius - distance) * dir * -1;
		}
		else if (CollisionPointVsCube(sphere.pos, cube))
		{
			distance += sphere.radius;
			Vec3 dir = (cube.closestPoint - sphere.pos).Normalize();
			return (distance + sphere.radius) * dir;
		}

		return Vec3(0,0,0);
	}

	Vec3 CollisionSphereVsCapsule(const Shape::Sphere& sphere, const Shape::Capsule& capsule, bool& hasCollided)
	{
		if (sphere.pos.y < capsule.pos.y + (capsule.height / 2) && sphere.pos.y > capsule.pos.y - (capsule.height / 2))
		{
			Vec3 pointCapSameY(capsule.pos.x, sphere.pos.y, capsule.pos.z);
			float distance = distanceBetweenTwoPoints(sphere.pos, capsule.pos);

			if (distance < sphere.radius + capsule.radius)
			{
				hasCollided = true;
				return (sphere.radius + capsule.radius - distance) * (sphere.pos - pointCapSameY).Normalize();
			}
		}

		Vec3 centerTopSphere(capsule.pos.x, capsule.pos.y + capsule.height / 2, capsule.pos.z);
		float distanceTopSphere = distanceBetweenTwoPoints(sphere.pos, centerTopSphere);
		if (distanceTopSphere < (sphere.radius + capsule.radius))
		{
			hasCollided = true;
			Vec3 dir = (sphere.pos - centerTopSphere).Normalize();
			dir = dir * (sphere.radius + capsule.radius - distanceTopSphere);
			return dir;
		}

		Vec3 centerBottomSphere(capsule.pos.x, capsule.pos.y - capsule.height / 2, capsule.pos.z);
		float distanceBottomSphere = distanceBetweenTwoPoints(sphere.pos, centerBottomSphere);
		if (distanceBottomSphere < (sphere.radius + capsule.radius))
		{
			hasCollided = true;
			Vec3 dir = (sphere.pos - centerBottomSphere).Normalize();
			dir = dir * (sphere.radius + capsule.radius - distanceBottomSphere);
			return dir;
		}

		return Vec3(0, 0, 0);
	}

	Vec3 CollisionCubeVsCapsule(Shape::Cube& cube, Shape::Capsule& capsule, const Vec3 velocity, bool& hasCollide)
	{
		bool collide = false;

		Shape::Sphere sphereTop(capsule.radius, Vec3(capsule.pos.x, capsule.pos.y + capsule.height/2, capsule.pos.z));
		Shape::Sphere sphereBottom(capsule.radius, Vec3(capsule.pos.x, capsule.pos.y - capsule.height / 2, capsule.pos.z));

		if (cube.pos.y < capsule.pos.y - capsule.height / 2 && capsule.pos.y - capsule.height / 2 < cube.pos.y + cube.height
		||  cube.pos.y < capsule.pos.y + capsule.height / 2 && capsule.pos.y + capsule.height / 2 < cube.pos.y + cube.height
		||  capsule.pos.y - capsule.height / 2 < cube.pos.y && cube.pos.y < capsule.pos.y + capsule.height / 2
		||  capsule.pos.y - capsule.height / 2 < cube.pos.y + cube.height && cube.pos.y + cube.height < capsule.pos.y + capsule.height / 2)
		{
			Shape::Sphere sphere(capsule.radius, Vec3(capsule.pos.x, cube.pos.y + cube.height / 2, capsule.pos.z));
			Vec3 dir = CollisionSphereVsCube(sphere, cube, velocity, collide);

			if (collide)
				hasCollide = true;

			return -1 * dir;
		}

		Vec3 dirSphereBottom = CollisionSphereVsCube(sphereBottom, cube, velocity, collide);
		if (collide)
		{
			hasCollide = true;
			return -1 * dirSphereBottom;
		}

		Vec3 dirSphereTop = CollisionSphereVsCube(sphereTop, cube, velocity, collide);
		if (collide)
		{
			hasCollide = true;
			return -1 * dirSphereTop;
		}

		return Vec3(0, 0, 0);
	}

	Vec3 CollisionCapsuleVsCapsule(const Shape::Capsule& capsule, const Shape::Capsule& capsule2, bool& hasCollide)
	{
		bool collide = false;

		if (capsule.pos.y + capsule.height / 2 >= capsule2.pos.y + capsule2.height / 2 && capsule2.pos.y + capsule2.height / 2 >= capsule.pos.y
		||  capsule.pos.y + capsule.height / 2 >= capsule2.pos.y && capsule2.pos.y >= capsule.pos.y)
		{
			Vec3 pointC1(capsule.pos.x, capsule.pos.y + capsule.height / 2, capsule.pos.z);
			Vec3 pointC2(capsule2.pos.x, capsule.pos.y + capsule.height / 2, capsule2.pos.z);

			if (distanceBetweenTwoPoints(pointC1, pointC2) < capsule.radius + capsule2.radius)
			{
				hasCollide = true;
				Vec3 dir = (pointC1 - pointC2).Normalize();
				float distanceDir = capsule.radius + capsule2.radius - distanceBetweenTwoPoints(pointC1, pointC2);
				return dir * distanceDir;
			}
		}

		//Above
		if (capsule.pos.y - capsule.height / 2 > capsule2.pos.y + capsule2.height / 2)
		{
			Shape::Sphere sphereBottom1(capsule.radius, Vec3(capsule.pos.x, capsule.pos.y - capsule.height / 2, capsule.pos.z));
			Shape::Sphere sphereTop2(capsule2.radius, Vec3(capsule2.pos.x, capsule2.pos.y + capsule2.height / 2, capsule2.pos.z));
			Vec3 dir = CollisionSphereVsSphere(sphereBottom1, sphereTop2, collide);

			if (collide)
				hasCollide = true;

			return dir;
		}

		//Below
		if (capsule.pos.y + capsule.height / 2 > capsule2.pos.y - capsule2.height / 2)
		{
			Shape::Sphere sphereBottom2(capsule2.radius, Vec3(capsule2.pos.x, capsule2.pos.y - capsule2.height / 2, capsule2.pos.z));
			Shape::Sphere sphereTop1(capsule.radius, Vec3(capsule.pos.x, capsule.pos.y + capsule.height / 2, capsule.pos.z));
			Vec3 dir = CollisionSphereVsSphere(sphereTop1, sphereBottom2, collide);

			if (collide)
				hasCollide = true;

			return dir;
		}

		return Vec3(0, 0, 0);
	}
}