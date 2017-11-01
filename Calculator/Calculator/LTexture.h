#pragma once
#include "libs.h"

class LTexture
{
public:
	static SDL_Renderer* gRenderer;

	LTexture();
	~LTexture();

	bool loadFromFile(std::string path);

	void free();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	void render(float x, float y, SDL_Rect* clip = NULL, float wMult = 1, float hMult = 1);

	int getWidth();
	int getHeight();

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};