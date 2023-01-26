#include <Light.h>


namespace LowRenderer
{
	int Light::nbDirLight;

	Light::Light(const maths::Vec3 ambiente, const maths::Vec3 diffuse, const maths::Vec3 specular)
	{
		ambienteColor = ambiente;
		diffuseColor = diffuse;
		specularColor = specular;
	}

	void Light::CreateDirectionalLight(const maths::Vec3 direction, const int shaderProgram)
	{
		std::string i = std::to_string(nbDirLight);
		glUniform3f(glGetUniformLocation(shaderProgram, ("dirLights[" + i + "].direction").c_str()), direction.x, direction.y, direction.z);

		glUniform3f(glGetUniformLocation(shaderProgram, ("dirLights[" + i + "].ambient").c_str()), ambienteColor.x, ambienteColor.y, ambienteColor.z);
		glUniform3f(glGetUniformLocation(shaderProgram, ("dirLights[" + i + "].diffuse").c_str()), diffuseColor.x, diffuseColor.y, diffuseColor.z);
		glUniform3f(glGetUniformLocation(shaderProgram, ("dirLights[" + i + "].specular").c_str()), specularColor.x, specularColor.y, specularColor.z);

		nbDirLight++;
	}
}