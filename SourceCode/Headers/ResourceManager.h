#pragma once
#include <map>
#include <string>
#include <iostream>  
#include <functional>
#include <type_traits>


namespace Resources
{
	class IResource
	{
	public:
		virtual void LoadResource(const std::string fileName) = 0;
		virtual void UnloadResource() = 0;

		bool isInit = false;
		bool isLoaded = false;
		std::function<void(std::string)> loadFunc;
	};

	class ResourceManager
	{
	public:
		ResourceManager() {};

		template<typename T>
		T* Create(const std::string name);

		template<typename T>
		T* Get(const std::string name);

		void Delete(const std::string name);

	private:
		std::map<std::string, IResource*> manager;
	};


	template<typename T>
	T* ResourceManager::Create(const std::string name)
	{
		T* resource = new T();
		if (manager.find(name) != manager.end())
		{
			delete((T*)manager.find(name)->second);
			manager.erase(name);
		}

		if (std::is_base_of<IResource, T>::value)
			manager[name] = (IResource*)resource;

		return resource;
	}

	template<typename T>
	T* ResourceManager::Get(const std::string name)
	{
		auto iterator = manager.find(name);

		if (iterator != manager.end())
		{
			return (T*)iterator->second;
		}
		else
		{
			std::cout << "Didn't Find :" << name << "\n";
			return nullptr;
		}
	}
}