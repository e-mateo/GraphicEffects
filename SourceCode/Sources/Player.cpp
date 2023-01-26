#include <iostream>

#include <Player.h>


Gameplay::Player::Player()
{
	state = ONGROUND;
	moveSpeed = 12.f;
	jumpForce = 1.43f;
	jumpPower = 5.75f;
	bumpReduce = 0.5f;
	gravity = 0.04321f;
	position = maths::Vec3(0, 0, 0);
	rotationEuler = maths::Vec3(0, 0, 0);
	velocity.y = -1.f;
}

void Gameplay::Player::Update(const double deltaTime, bool isFocused, bool isLocked, const LowRenderer::CameraInputs& inputs, const LowRenderer::Camera camera)
{
	velocity.x = 0;
	velocity.z = 0;

	if (position.x >= 800 && position.x <= 1000 && position.y >= 100 && position.y <= 300 && position.z <= -300 && position.z >= -2300)
		moveSpeed = 36.f;
	else
		moveSpeed = 15.f;

	if (position.y <= -50)
		SpawnPlayer(maths::Vec3(50, 10, 350));

	Jump(inputs, camera);
	Move(deltaTime, inputs, camera);
	
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;
	if (isFocused)
		gameObject->rotation = maths::Vec3(0, camera.angleTheta + 180.f, 0);
	else
		gameObject->rotation = rotationEuler;
	gameObject->collider.velocity = velocity;
	gameObject->Update(velocity, maths::Vec3(0, 0, 0), maths::Vec3(0, 0, 0));
}

void Gameplay::Player::Jump(const LowRenderer::CameraInputs& inputs, const LowRenderer::Camera camera)
{
	if (inputs.jump && state == ONGROUND)
	{
		velocity.y = jumpForce;
		state = ONJUMP;
	}
}

void Gameplay::Player::SpawnPlayer(const maths::Vec3 spawnPos)
{
	position = spawnPos;
	rotationEuler = maths::Vec3(0, 0, 0);
	gameObject->position = position;

	gameObject->collider.sphereCollider.pos = spawnPos + maths::Vec3(0, 10, 0);
	gameObject->collider.cubeCollider.pos = position - maths::Vec3(10, 0, 10);
}

void Gameplay::Player::Move(const double deltaTime, const LowRenderer::CameraInputs& inputs, const LowRenderer::Camera camera)
{
	float deltatime = (float)deltaTime;
	if (state == ONGROUND)
	{
		velocity.y = 0;
	}
	else if (state != ONGROUND && state != ONJUMP)
	{
		velocity.y -= gravity;
	}
	else if (state == ONJUMP)
	{
		if (velocity.y >= 0)
			velocity.y -= gravity * jumpPower * deltatime / inputs.reduce;
		else
			velocity.y -= (gravity * 1.8f) * jumpPower * deltatime / inputs.reduce;
	}

	float dirXx = camera.dirX.x * moveSpeed * deltatime / inputs.reduce;
	float dirXz = camera.dirX.z * moveSpeed * deltatime / inputs.reduce;
	float dirZx = camera.dirZ.x * moveSpeed * deltatime / inputs.reduce;
	float dirZz = camera.dirZ.z * moveSpeed * deltatime / inputs.reduce;

	if (inputs.moveForward)
	{
		if (inputs.moveLeft)
		{
			velocity.x = dirZx - dirXx;
			velocity.z = dirZz - dirXz;
			rotationEuler.y = camera.angleTheta + 135;
		}
		else if (inputs.moveRight)
		{
			velocity.x = dirZx + dirXx;
			velocity.z = dirZz + dirXz;
			rotationEuler.y = camera.angleTheta - 135;
		}
		else
		{
			velocity.x = dirZx;
			velocity.z = dirZz;
			rotationEuler.y = camera.angleTheta + 180;
		}
	}

	if (inputs.moveBackward)
	{
		if (inputs.moveLeft)
		{
			velocity.x = -dirZx - dirXx;
			velocity.z = -dirZz - dirXz;
			rotationEuler.y = camera.angleTheta + 45;
		}
		else if (inputs.moveRight)
		{
			velocity.x = -dirZx + dirXx;
			velocity.z = -dirZz + dirXz;
			rotationEuler.y = camera.angleTheta - 45;
		}
		else
		{
			velocity.x = -dirZx;
			velocity.z = -dirZz;
			rotationEuler.y = camera.angleTheta;
		}
	}

	if (!inputs.moveBackward && !inputs.moveForward)
	{
		if (inputs.moveLeft)
		{
			velocity.x = -dirXx;
			velocity.z = -dirXz;
			rotationEuler.y = camera.angleTheta + 90;
		}

		if (inputs.moveRight)
		{
			velocity.x = dirXx;
			velocity.z = dirXz;
			rotationEuler.y = camera.angleTheta + 270;
		}
	}
}