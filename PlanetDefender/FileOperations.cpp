#include "FileOperations.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string FileOperations::readFile(const char *filePath)
{
	std::string fileContent;
	std::ifstream file; 
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file.open(filePath);

		std::stringstream fileStream;
		fileStream << file.rdbuf();

		file.close();

		fileContent = fileStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "File failed to read" << std::endl;
	}
	return fileContent;
}
