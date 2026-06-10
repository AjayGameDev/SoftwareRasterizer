#include "FileManager.h"
#include "SDL3/SDL_iostream.h"
#include "spdlog/spdlog.h"
#include "SDL3/SDL.h"
#include <iostream>

// Create the definition outside of the class in cpp file
unordered_map<FileManager::Location, string> FileManager::locations;


string FileManager::GetLocation(Location location)
{
	static bool initialized = false;

	if (!initialized)
	{
		Initialize();
		initialized = true;
	}

	return locations[location];
}


void FileManager::Initialize()
{

#if defined(Target_Platform_Android)
	string base = "";
	locations[Texture] = base + "textures/astc/";
	locations[Skybox]  = base + "textures/astc/hdri/";
	locations[Shader]  = base + "shaders/compiled/android/";
#elif defined(Target_Platform_Win32)
	//string base = "C:/Users/dubey/CLionProjects/AjaxTown/assets/";
	std::string base   = std::string(SDL_GetBasePath()) + "assets/";
	locations[Texture] = base + "textures/bc/";
	locations[Skybox]  = base + "textures/bc/hdri/";
	locations[Shader]  = base + "shaders/compiled/win32/";
#elif defined(Target_Platform_Linux)
	std::string base   = std::string(SDL_GetBasePath()) + "assets/";
	locations[Texture] = base + "textures/bc/";
	locations[Skybox]  = base + "textures/bc/hdri/";
	locations[Shader]  = base + "shaders/compiled/win32/";
	
	std::cout << "Linuxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx!!!!!!!";
#endif
	

	locations[Model]   = base + "model/";

	locations[Fonts]   = base + "fonts/";

	locations[Config]  = base + "configuration/";


}


std::vector<uint8_t> FileManager::LoadBytes_8bit(const std::string& filePath)
{

	SDL_IOStream* io = SDL_IOFromFile(filePath.c_str(),"rb");

	if (!io)
	{
		spdlog::error("file manager can't open file: " + filePath);
		throw std::runtime_error("file manager can't open file: " + filePath);
	}

	Sint64 size = SDL_GetIOSize(io);      // Get the size
	std::vector<uint8_t> buffer(size);   //  Create buffer of that size
	SDL_ReadIO(io,buffer.data(),size);  //   write data to that buffer
	SDL_CloseIO(io);	               //    close io

	return buffer;
}

std::vector<uint32_t> FileManager::LoadBytes_32bit(const std::string& filePath)
{
	SDL_IOStream* io = SDL_IOFromFile(filePath.c_str(),"rb");

	if (!io)
	{
		spdlog::error("file manager can't open file: " + filePath);
		throw std::runtime_error("file manager can't open file: " + filePath);
	}

	Sint64 size = SDL_GetIOSize(io);      // Get the size
	std::vector<uint32_t> buffer(size / sizeof(uint32_t));   //  Create buffer of that size
	SDL_ReadIO(io,buffer.data(),size);  //   write data to that buffer
	SDL_CloseIO(io);	               //    close io

	return buffer;
}


























