#pragma once 

#include <iostream>
#include <GLEW/glew.h>
#include <stb_image.h>

class Texture {

private:
	unsigned int textureID;
	std::string textureName;

	int width;
	int height;
	int channels;

public:
	Texture(const std::string& _textureName);
	std::string getName() { return textureName; }

	void bind() const;
	void load(const std::string& fileName);
};