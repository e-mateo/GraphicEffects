#include <ResourceManager.h>


namespace Resources
{
	void ResourceManager::Delete(const std::string name)
	{
		if (manager.find(name) != manager.end())
		{
			manager.find(name)->second->UnloadResource();
			delete(manager.find(name)->second);
			manager.erase(name);
		}
	}
}