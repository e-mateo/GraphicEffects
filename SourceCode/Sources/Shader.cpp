#include <Shader.h>


Resources::Shader::Shader()
{
	vertexShader = 0;
	shaderProgram = 0;
	fragmentShader = 0;
}

bool Resources::Shader::SetVertexShader(std::filesystem::path const& filename)
{
	// vertex shader
	LoadResource(filename.string());
	const char* shader = shaderString.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &shader, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::string log(infoLog);
		DEBUG_LOG("ERROR::SHADER::VERTEX::COMPILATION_FAILED %s\n", log);
		return false;
	}

	return true;
}

bool Resources::Shader::SetFragmentShader(std::filesystem::path const& filename)
{
	// fragment shader
	LoadResource(filename.string());
	const char* shader = shaderString.c_str();

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shader, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::string log(infoLog);
		DEBUG_LOG("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED %s\n", log);
		return false;
	}

	return true;
}

bool Resources::Shader::Link()
{
	// link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::string log(infoLog);
		DEBUG_LOG("ERROR::SHADER::PROGRAM::LINKING_FAILED %s\n", log);
		return false;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

void Resources::Shader::LoadResource(const std::string fileName)
{
	std::filesystem::path path = fileName;
	std::string shader;
	std::string line;
	std::ifstream file;

	if (std::filesystem::exists(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		shader.assign((std::istreambuf_iterator< char >(file)), std::istreambuf_iterator< char >());
		shader.push_back('\0');
	}
	else
	{
		DEBUG_LOG("Shader doesn't exist");
	}

	file.close();
	shaderString = shader;
}

void Resources::Shader::UnloadResource()
{
	shaderString.~basic_string();
}