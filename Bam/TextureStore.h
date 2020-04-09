#pragma once

class TextureStore
{
private:
	GLuint textureHandle;
public:

	TextureStore(std::string name);
	TextureStore() = delete;
	~TextureStore();

	deleteDefaults(TextureStore);
};