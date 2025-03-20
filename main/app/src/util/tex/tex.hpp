#pragma once 

#include <iostream>
#include <GLEW/glew.h>
#include <stb_image.h>

namespace tex {
	struct Texture {
		unsigned int id;
		std::string name;
		std::string path;
	};

	unsigned int TextureFromFile(const char* path);
};