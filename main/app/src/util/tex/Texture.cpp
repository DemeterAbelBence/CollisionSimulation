#include "Texture.hpp"

Texture::Texture(const std::string& _name) {
	name = _name;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
}

Texture::Texture(const Texture& texture) {
	this->id = texture.id;
	this->name = texture.name;
	this->width = texture.width;
	this->height = texture.height;
	this->channels = texture.channels;
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::load(const std::string& fileName) {
	bind();
	data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture:" 
			<< name << "!" << std::endl;
	}
	stbi_image_free(data);
}