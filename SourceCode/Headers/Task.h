#pragma once
#include <functional>


struct Task
{
public:
	Task() {};
	Task(std::function<void(std::string)> _func, std::string _filePath) {func = _func;  filePath = _filePath;}
	std::function<void(std::string)> func;
	std::string filePath;
};