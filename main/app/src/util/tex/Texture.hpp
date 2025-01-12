#pragma once 

#include <iostream>
#include <GLEW/glew.h>

#include "stb_image.h"

class Texture {

private:
	unsigned int id;

	std::string name;

	int width;
	int height;
	int channels;

	unsigned char* data;

public:
	Texture(const std::string& _name);
	Texture(const Texture& texture);
	inline std::string getName() { return name; }

	void bind() const;
	void load(const std::string& fileName);
};