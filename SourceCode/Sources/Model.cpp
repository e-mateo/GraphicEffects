#include <Model.h>


namespace Resources
{
	Model::Model()
	{
		this->loadFunc = std::bind(&IResource::LoadResource, this, std::placeholders::_1);
	}

	void SplitLine(const std::string line, const char separator, std::vector<std::string>& splitLine)
	{
		size_t saveI = 0;

		for (size_t i = 0; line[i] != '\0'; i++) //Split the string with separator char
		{
			if (line[i] == separator)
			{
				splitLine.push_back(line.substr(saveI, i - saveI)); //Save the part of the string between the save position and the position of the separator
				saveI = i + 1;
			}
			else if (line[i + 1] == '\0') //End of the line 
			{
				splitLine.push_back(line.substr(saveI, ((i + 1) - saveI)));
			}
		}
	}

	void Model::LoadResource(const std::string fileName)
	{
		std::filesystem::path path("Resources/Obj/" + fileName);
		std::ifstream file;
		std::string line;
		std::vector<maths::Vec3> tempPos, tempNormal, tempIndex; 
		std::vector<maths::Vec2> tempUV;

		if (std::filesystem::exists(path))
		{
			file.open(path.c_str(), std::ifstream::in);
			if (file)
			{
				///Parsing///
				while (std::getline(file, line)) //Get every line of the file
				{
					std::string type = line.substr(0, 2); //Get the type of the line and delete it from the string
					if (type == "v " || type == "f ")
						line.erase(0, 2);
					else
						line.erase(0, 3);
					
					if (line[0] == ' ')
						line.erase(0, 1);


					std::vector<std::string> splitLine;
					SplitLine(line,' ', splitLine);
					StoreDatas(type, splitLine, tempPos, tempNormal, tempUV, tempIndex);
				}
				file.close();

				DatasToVertexBuffer(tempPos, tempNormal, tempUV, tempIndex);
				isLoaded = true;
			}
			else
			{
				DEBUG_LOG("Can't open file %s", fileName);
			}
		}
		else
		{
			DEBUG_LOG("File %s doesn't exist", fileName);
		}
		
	}

	void Model::StoreDatas(const std::string type, std::vector<std::string> splitLine, std::vector<maths::Vec3>& pos, std::vector<maths::Vec3>& normal, std::vector<maths::Vec2>& uv, std::vector<maths::Vec3>& index)
	{
		//Store datas in the right vector
		if (type == "v ")
			pos.push_back(maths::Vec3(std::stof(splitLine[0]), std::stof(splitLine[1]), std::stof(splitLine[2]))); //Vector3 position
		else if (type == "vn")
			normal.push_back(maths::Vec3(std::stof(splitLine[0]), std::stof(splitLine[1]), std::stof(splitLine[2]))); //Vector3 normal
		else if (type == "vt")
			uv.push_back(maths::Vec2(std::stof(splitLine[0]), std::stof(splitLine[1]))); //Vector2 uv
		else if (type == "f ")
			StoreIndexList(splitLine, index); //Vector3 index
	}

	void Model::StoreIndexList(std::vector<std::string> splitLine, std::vector<maths::Vec3> &index)
	{
		std::string number;
		std::vector<int> indexList;

		for (int i = 0; i < 3; i++) //Re split the 3 substring in 3 vectors of 3 int
		{
			for (int j = 0; splitLine[i][j] != '\0'; j++)
			{
				if (splitLine[i][j] != '/' && splitLine[i][j] != ' ' && splitLine[i][j] != '\0')
				{
					number.push_back(splitLine[i][j]); //store the number as a string
				}
				else
				{
					indexList.push_back(std::stoi(number)); //Store the number in the index list vector
					number.clear();
				}
			}
			indexList.push_back(std::stoi(number));
			number.clear();

			index.push_back(maths::Vec3((float)indexList[0], (float)indexList[1], (float)indexList[2]));  //Push back vector3 index
			indexList.clear();
		}
	}

	void Model::DatasToVertexBuffer(const std::vector<maths::Vec3> pos, const std::vector<maths::Vec3> normal, const std::vector<maths::Vec2> uv, const std::vector<maths::Vec3> index)
	{
		//Store datas in vertexBuffer (according to the right index)
		for (int i = 0; i < index.size(); i++)
		{
			Vertex newVertex;
			newVertex.position = pos[(size_t)index[i].x - (size_t)1];
			newVertex.textureUV = uv[(size_t)index[i].y - (size_t)1];
			newVertex.normal = normal[(size_t)index[i].z - (size_t)1];

			vertexBuffer.push_back(newVertex);
		}
	}

	void Model::BindBuffer()
	{
		glGenBuffers(1, &VBO);
		//glGenBuffers(1, &EBO);

		glGenVertexArrays(1, &VAO);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		//glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertexBuffer.size(), &this->vertexBuffer.front(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(maths::Vec3)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(maths::Vec3)));
		glEnableVertexAttribArray(2);

		isInit = true;
	}

	unsigned int Model::GetVAO()
	{
		return VAO;
	}

	std::vector<Vertex> Model::GetVertexBuffer()
	{
		return vertexBuffer;
	}

	void Model::UnloadResource()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		vertexBuffer.~vector();
	}

	void Model::CreateSkybox()
	{
		            							   // POSITION			          //NORMAL			     //UV
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f,  1.0f, -1.0f), maths::Vec3(0, 0, -1), maths::Vec2((float)1, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f, -1.0f, -1.0f), maths::Vec3(0, 0, -1), maths::Vec2((float)1, (float)2/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f, -1.0f, -1.0f), maths::Vec3(0, 0, -1), maths::Vec2((float)3/4, (float)2/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f, -1.0f, -1.0f), maths::Vec3(0, 0, -1), maths::Vec2((float)3/4, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f,  1.0f, -1.0f), maths::Vec3(0, 0, -1), maths::Vec2((float)1, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f,  1.0f, -1.0f), maths::Vec3(0, 0, -1), maths::Vec2((float)3/4, (float)2/3) });
												   
		//BACK FACE								   
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f, -1.0f,  1.0f), maths::Vec3(0, 0, 1), maths::Vec2((float)1/2, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f, -1.0f, -1.0f), maths::Vec3(0, 0, 1), maths::Vec2((float)1/2, (float)2/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f,  1.0f, -1.0f), maths::Vec3(0, 0, 1), maths::Vec2((float)1/4, (float)2/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f,  1.0f, -1.0f), maths::Vec3(0, 0, 1), maths::Vec2((float)1/4, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f,  1.0f,  1.0f), maths::Vec3(0, 0, 1), maths::Vec2((float)1/2, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f, -1.0f,  1.0f), maths::Vec3(0, 0, 1), maths::Vec2((float)1/4, (float)2/3) });
												  
		//LEFT FACE								  
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f, -1.0f, -1.0f), maths::Vec3(1, 0, 0), maths::Vec2((float)1/4, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f, -1.0f,  1.0f), maths::Vec3(1, 0, 0), maths::Vec2((float)1/4, (float)2/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f,  1.0f,  1.0f), maths::Vec3(1, 0, 0), maths::Vec2((float)0, (float)2/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f,  1.0f,  1.0f), maths::Vec3(1, 0, 0), maths::Vec2((float)0, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f,  1.0f, -1.0f), maths::Vec3(1, 0, 0), maths::Vec2((float)1/4, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f, -1.0f, -1.0f), maths::Vec3(1, 0, 0), maths::Vec2((float)0, (float)2/3) });
												   
		//RIGHT FACE							   
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f, -1.0f,  1.0f), maths::Vec3(-1, 0, 0), maths::Vec2((float)3/4, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f,  1.0f,  1.0f), maths::Vec3(-1, 0, 0), maths::Vec2((float)3/4, (float)2/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f,  1.0f,  1.0f), maths::Vec3(-1, 0, 0), maths::Vec2((float)1/2, (float)2/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f,  1.0f,  1.0f), maths::Vec3(-1, 0, 0), maths::Vec2((float)1/2, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f, -1.0f,  1.0f), maths::Vec3(-1, 0, 0), maths::Vec2((float)3/4, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f, -1.0f,  1.0f), maths::Vec3(-1, 0, 0), maths::Vec2((float)1/2, (float)2/3) });
												   
		//BOTTOM FACE							   
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f,  1.0f, -1.0f), maths::Vec3(0, 1, 0), maths::Vec2((float)1/4, (float)1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f,  1.0f, -1.0f), maths::Vec3(0, 1, 0), maths::Vec2((float)1/4, (float)2/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f,  1.0f,  1.0f), maths::Vec3(0, 1, 0), maths::Vec2((float)1/2, (float)2/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f,  1.0f,  1.0f), maths::Vec3(0, 1, 0), maths::Vec2((float)1/2, (float)1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f,  1.0f,  1.0f), maths::Vec3(0, 1, 0), maths::Vec2((float)1/4, (float)1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f,  1.0f, -1.0f), maths::Vec3(0, 1, 0), maths::Vec2((float)1/2, (float)2/3) });
												   
		//TOP FACE								 
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f, -1.0f, -1.0f), maths::Vec3(0, -1, 0), maths::Vec2((float)1/4, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f, -1.0f,  1.0f), maths::Vec3(0, -1, 0), maths::Vec2((float)1/4, (float)0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f, -1.0f, -1.0f), maths::Vec3(0, -1, 0), maths::Vec2((float)1/2, (float)0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f, -1.0f, -1.0f), maths::Vec3(0, -1, 0), maths::Vec2((float)1/2, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(-1.0f, -1.0f,  1.0f), maths::Vec3(0, -1, 0), maths::Vec2((float)1/4, (float)1/3) });
		vertexBuffer.push_back(Vertex{ maths::Vec3( 1.0f, -1.0f,  1.0f), maths::Vec3(0, -1, 0), maths::Vec2((float)1/2, (float)0) });
		isLoaded = true;
	}

	void Model::CreateCube()
	{
		//FRONT FACE								// POSITION			//NORMAL			//UV
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 0, 0), maths::Vec3(0, 0, -1), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 0, 0), maths::Vec3(0, 0, -1), maths::Vec2(0, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 1, 0), maths::Vec3(0, 0, -1), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 0, 0), maths::Vec3(0, 0, -1), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 1, 0), maths::Vec3(0, 0, -1), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 1, 0), maths::Vec3(0, 0, -1), maths::Vec2(1, 1) });

		//BACK FACE							
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 0, 1), maths::Vec3(0, 0, 1), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 0, 1), maths::Vec3(0, 0, 1), maths::Vec2(0, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 1, 1), maths::Vec3(0, 0, 1), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 0, 1), maths::Vec3(0, 0, 1), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 1, 1), maths::Vec3(0, 0, 1), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 1, 1), maths::Vec3(0, 0, 1), maths::Vec2(1, 1) });

		//LEFT FACE									
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 0, 0), maths::Vec3(-1, 0, 0), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 0, 1), maths::Vec3(-1, 0, 0), maths::Vec2(0, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 1, 1), maths::Vec3(-1, 0, 0), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 0, 0), maths::Vec3(-1, 0, 0), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 1, 1), maths::Vec3(-1, 0, 0), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 1, 0), maths::Vec3(-1, 0, 0), maths::Vec2(1, 1) });

		//RIGHT FACE									
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 0, 1), maths::Vec3(1, 0, 0), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 0, 0), maths::Vec3(1, 0, 0), maths::Vec2(0, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 1, 0), maths::Vec3(1, 0, 0), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 0, 1), maths::Vec3(1, 0, 0), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 1, 0), maths::Vec3(1, 0, 0), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 1, 1), maths::Vec3(1, 0, 0), maths::Vec2(1, 1) });

		//BOTTOM FACE									
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 0, 1), maths::Vec3(0, -1, 0), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 0, 1), maths::Vec3(0, -1, 0), maths::Vec2(0, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 0, 0), maths::Vec3(0, -1, 0), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 0, 1), maths::Vec3(0, -1, 0), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 0, 0), maths::Vec3(0, -1, 0), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 0, 0), maths::Vec3(0, -1, 0), maths::Vec2(1, 1) });

		//TOP FACE									
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 1, 0), maths::Vec3(0, 1, 0), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 1, 0), maths::Vec3(0, 1, 0), maths::Vec2(0, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 1, 1), maths::Vec3(0, 1, 0), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 1, 0), maths::Vec3(0, 1, 0), maths::Vec2(1, 0) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(0, 1, 1), maths::Vec3(0, 1, 0), maths::Vec2(0, 1) });
		vertexBuffer.push_back(Vertex{ maths::Vec3(1, 1, 1), maths::Vec3(0, 1, 0), maths::Vec2(1, 1) });
		isLoaded = true;
	}

	static maths::Vec3 getSphericalCoords(const float radius, const float theta, const float phi)
	{
		return { radius * sinf(theta) * cosf(phi),
				 radius * cosf(theta),
				 radius * sinf(theta) * sinf(phi) };
	}

	void Model::CreateSphere(const int lat, const int lon, float radius)
	{
		for (int j = 0; j < lat; ++j)
		{
			float theta0 = ((j + 0) / (float)lat) * maths::PI;
			float theta1 = ((j + 1) / (float)lat) * maths::PI;

			for (int i = 0; i < lon; ++i)
			{
				float phi0 = ((i + 0) / (float)lon) * 2.f * maths::PI;
				float phi1 = ((i + 1) / (float)lon) * 2.f * maths::PI;

				maths::Vec3 c0 = getSphericalCoords(radius, theta0, phi0);
				maths::Vec3 c1 = getSphericalCoords(radius, theta0, phi1);
				maths::Vec3 c2 = getSphericalCoords(radius, theta1, phi1);
				maths::Vec3 c3 = getSphericalCoords(radius, theta1, phi0);

				//                                           pos            /                normal              /                 uv
				vertexBuffer.push_back(Vertex{ maths::Vec3(c0.x, c0.y, c0.z), maths::Vec3((c0.x), (c0.y), (c0.z)), maths::Vec2(0.0f, 0.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c1.x, c1.y, c1.z), maths::Vec3((c1.x), (c1.y), (c1.z)), maths::Vec2(0.0f, 1.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c3.x, c3.y, c3.z), maths::Vec3((c3.x), (c3.y), (c3.z)), maths::Vec2(1.0f, 0.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c1.x, c1.y, c1.z), maths::Vec3((c1.x), (c1.y), (c1.z)), maths::Vec2(0.0f, 1.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c2.x, c2.y, c2.z), maths::Vec3((c2.x), (c2.y), (c2.z)), maths::Vec2(1.0f, 1.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c3.x, c3.y, c3.z), maths::Vec3((c3.x), (c3.y), (c3.z)), maths::Vec2(1.0f, 0.0f) });
			}
		}
		isLoaded = true;
	}

	void Model::CreateCapsule(const float height, const float radius, const int sides)
	{
		//Cylinder 
		for (int i = 0; i < sides; i++)
		{
			float theta0 = ((float)(i) / (float)sides) * 2 * maths::PI;
			float theta1 = ((float)(i + 1) / (float)sides) * 2 * maths::PI;

			//Top circle
			vertexBuffer.push_back(Vertex{ maths::Vec3(0, height/2, 0), maths::Vec3(0,1,0), maths::Vec2(0.5f, 0.5f)});
			vertexBuffer.push_back(Vertex{ maths::Vec3(radius * cos(theta1), height / 2, radius * sin(theta1)), maths::Vec3(0,1,0), maths::Vec2(cos(theta1),  sin(theta0)) });
			vertexBuffer.push_back(Vertex{ maths::Vec3(radius * cos(theta0), height / 2, radius * sin(theta0)), maths::Vec3(0,1,0), maths::Vec2(cos(theta0), sin(theta0)) });

			//Bottom circle
			vertexBuffer.push_back(Vertex{ maths::Vec3(0, -height / 2, 0), maths::Vec3(0,-1,0), maths::Vec2(0.0f, 0.0f) });
			vertexBuffer.push_back(Vertex{ maths::Vec3(radius * cos(theta0), -height / 2, radius * sin(theta0)), maths::Vec3(0,-1,0), maths::Vec2(cos(theta0), sin(theta0))});
			vertexBuffer.push_back(Vertex{ maths::Vec3(radius * cos(theta1), -height / 2, radius * sin(theta1)), maths::Vec3(0,-1,0), maths::Vec2(cos(theta1),  sin(theta0))});


			//Base
			vertexBuffer.push_back(Vertex{ maths::Vec3(radius * cos(theta1), -height / 2, radius * sin(theta1)),  maths::Vec3(cos(theta1), 0, sin(theta1)), maths::Vec2(0.0f, 1.0f) });
			vertexBuffer.push_back(Vertex{ maths::Vec3(radius * cos(theta0), -height / 2, radius * sin(theta0)), maths::Vec3(cos(theta0), 0, sin(theta0)), maths::Vec2(0.0f, 0.0f) });
			vertexBuffer.push_back(Vertex{ maths::Vec3(radius * cos(theta1), height / 2, radius * sin(theta1)), maths::Vec3(cos(theta1), 0, sin(theta1)), maths::Vec2(1.0f, 1.0f)});

			vertexBuffer.push_back(Vertex{ maths::Vec3(radius * cos(theta0), -height / 2, radius * sin(theta0)), maths::Vec3(cos(theta0), 0, sin(theta0)), maths::Vec2(0.0f, 0.0f) });
			vertexBuffer.push_back(Vertex{ maths::Vec3(radius * cos(theta0), height / 2, radius * sin(theta0)), maths::Vec3(cos(theta0), 0, sin(theta0)), maths::Vec2(0.0f, 1.0f) });
			vertexBuffer.push_back(Vertex{ maths::Vec3(radius * cos(theta1), height / 2, radius * sin(theta1)),  maths::Vec3(cos(theta1), 0, sin(theta1)), maths::Vec2(1.0f, 1.0f) });
		}

		//Half Spheres 
		for (int j = 0; j < sides; ++j)
		{
			float theta0 = ((j + 0) / (float)sides) * maths::PI/2;
			float theta1 = ((j + 1) / (float)sides) * maths::PI/2;

			for (int i = 0; i < sides; ++i)
			{
				float phi0 = ((i + 0) / (float)sides) * 2.f * maths::PI;
				float phi1 = ((i + 1) / (float)sides) * 2.f * maths::PI;

				maths::Vec3 c0 = getSphericalCoords(radius, theta0, phi0);
				maths::Vec3 c1 = getSphericalCoords(radius, theta0, phi1);
				maths::Vec3 c2 = getSphericalCoords(radius, theta1, phi1);
				maths::Vec3 c3 = getSphericalCoords(radius, theta1, phi0);

				//Top Half Sphere
				vertexBuffer.push_back(Vertex{ maths::Vec3(c0.x, c0.y + (height/2), c0.z), maths::Vec3((c0.x), (c0.y), (c0.z)), maths::Vec2(0.0f, 0.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c1.x, c1.y + (height/2), c1.z), maths::Vec3((c1.x), (c1.y), (c1.z)), maths::Vec2(0.0f, 1.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c3.x, c3.y + (height/2), c3.z), maths::Vec3((c3.x), (c3.y), (c3.z)), maths::Vec2(1.0f, 0.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c1.x, c1.y + (height/2), c1.z), maths::Vec3((c1.x), (c1.y), (c1.z)), maths::Vec2(0.0f, 1.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c2.x, c2.y + (height/2), c2.z), maths::Vec3((c2.x), (c2.y), (c2.z)), maths::Vec2(1.0f, 1.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c3.x, c3.y + (height/2), c3.z), maths::Vec3((c3.x), (c3.y), (c3.z)), maths::Vec2(1.0f, 0.0f) });

				//Bottom Half Sphere										   
				vertexBuffer.push_back(Vertex{ maths::Vec3(c0.x, c0.y * -1 - (height / 2), c0.z), maths::Vec3((c0.x), -(c0.y), (c0.z)), maths::Vec2(0.0f, 0.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c3.x, c3.y * -1 - (height / 2), c3.z), maths::Vec3((c3.x), -(c3.y), (c3.z)), maths::Vec2(1.0f, 0.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c1.x, c1.y * -1 - (height / 2), c1.z), maths::Vec3((c1.x), -(c1.y), (c1.z)), maths::Vec2(0.0f, 1.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c1.x, c1.y * -1 - (height / 2), c1.z), maths::Vec3((c1.x), -(c1.y), (c1.z)), maths::Vec2(0.0f, 1.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c3.x, c3.y * -1 - (height / 2), c3.z), maths::Vec3((c3.x), -(c3.y), (c3.z)), maths::Vec2(1.0f, 0.0f) });
				vertexBuffer.push_back(Vertex{ maths::Vec3(c2.x, c2.y * -1 - (height / 2), c2.z), maths::Vec3((c2.x), -(c2.y), (c2.z)), maths::Vec2(1.0f, 1.0f) });
			}
		}
		isLoaded = true;
	}
}