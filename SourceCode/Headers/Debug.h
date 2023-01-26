#pragma once
#include <string> 
#include <fstream>
#include <stdarg.h> 
#include <assert.h>
#include <iostream>  
#include <filesystem>


namespace Core::Debug
{
	#define ASSERT(cond) (assert(cond))
	#define DEBUG_LOG(string, ...) (Core::Debug::DebugLog(__FILE__, __LINE__,string, __VA_ARGS__))

	void DebugLog(const char* file, const int line, const char* format, ...);

	class Log
	{
	public:
		void Print(const char* format, ...);
		void OpenFile(std::filesystem::path const& filename);

		~Log();

	private:
		std::ofstream file;
	};
}