#include <Debug.h>


void Core::Debug::DebugLog(const char* file, const int line, const char* format, ...)
{
	std::string s_file = file;
	std::string s_line = "(" + std::to_string(line) + "): ";
	std::string output = s_file + s_line;

	va_list args;
	va_start(args, format);
	int i = 0;
	while (format[i] != '\0')
	{
		if (format[i] == '%')
		{
			if (format[i + 1] == 'd') //Int 
			{
				int var = va_arg(args, int);

				//Int to string//
				std::string add = std::to_string(var);
				output.insert(output.length(), add);
			}
			else if (format[i + 1] == 'f') //Float
			{
				float var = (float)va_arg(args, double);

				//Float to string//
				std::string add = std::to_string(var);
				output.insert(output.length(), add);
			}
			else if (format[i + 1] == 'c') //Char
			{
				char add = va_arg(args, char);
				output.push_back(add);
			}
			else if (format[i + 1] == 's') //String
			{
				std::string add = va_arg(args, std::string);
				output.insert(output.length(), add);
			}
			i += 2;
		}
		else
		{
			output.push_back(format[i]);
			i++;
		}
	}
	va_end(args);

	std::cout << output << std::endl;
}

void Core::Debug::Log::OpenFile(std::filesystem::path const& filename)
{
	if (std::filesystem::exists(filename))
	{
		file.open(filename.c_str(), std::ofstream::out);

		if (!file)
			DEBUG_LOG("Can't open file for writing");
	}
	else
	{
		DEBUG_LOG("File doesn't exist");
	}
}

void Core::Debug::Log::Print(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	std::string output;

	int i = 0;
	while (format[i] != '\0')
	{
		if (format[i] == '%')
		{
			if (format[i + 1] == 'd') //Int 
			{
				int var = va_arg(args, int);
				std::string add = std::to_string(var);
				output.insert(output.length(), add);
			}
			else if (format[i + 1] == 'f') //Float
			{
				float var = (float)va_arg(args, double);
				std::string add = std::to_string(var);
				output.insert(output.length(), add);
			}
			else if (format[i + 1] == 'c') //Char
			{
				char add = va_arg(args, char);
				output.push_back(add);
			}
			else if (format[i + 1] == 's') //String
			{
				std::string add = va_arg(args, std::string);
				output.insert(output.length(), add);
			}
			i += 2;
		}
		else
		{
			output.push_back(format[i]);
			i++;
		}
	}

	va_end(args);
	std::cout << output << std::endl;

	if (file)
		file << output;
}

Core::Debug::Log::~Log()
{
	file.close();
}