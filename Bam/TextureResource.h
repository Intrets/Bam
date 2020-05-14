#pragma once

class TextureStore;

class TextureResource
{
private:
	int32_t index;

public:
	TextureStore* get();

	TextureResource(std::string name);
	TextureResource() = default;
	~TextureResource() = default;
};
