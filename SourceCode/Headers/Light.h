#pragma once
#include<String>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Maths.h>


namespace LowRenderer
{
	class Light
	{
		maths::Vec3 ambienteColor;
		maths::Vec3 diffuseColor;
		maths::Vec3 specularColor;

	public:
		Light(const maths::Vec3 ambiente, const maths::Vec3 diffuse, const maths::Vec3 specular);

		static int nbDirLight;

		void CreateDirectionalLight(const maths::Vec3 direction, const int shaderProgram);
	};
}