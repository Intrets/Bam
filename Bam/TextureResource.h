#pragma once

class TextureStore;

class TextureResource
{
private:
	int index;

public:
	TextureStore* get();

	TextureResource(std::string name);
	TextureResource() = default;
	~TextureResource() = default;
};
