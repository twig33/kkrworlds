#ifndef HELPER_H
#define HELPER_H

#include <fstream>
#include <sstream>
#include <iostream>

std::string loadFile(const char* path){
	std::ifstream file;
	std::stringstream filestream;
	// ensure ifstream objects can throw exceptions:
	file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try 
	{
			// open files
		file.open(path);
		// read file's buffer contents into streams
		filestream << file.rdbuf();
		// close file handlers
		file.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return filestream.str();
}

#endif