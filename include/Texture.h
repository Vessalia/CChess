#pragma once
#include "core.h"
#include <SDL2/SDL.h>

typedef struct Texture
{
    SDL_Texture* texture;

    unsigned int width;
    unsigned int height;
} Texture;

Texture* newTexture();
void deleteTexture(Texture* tex);

bool loadFromFile(Texture* tex, char const* path, SDL_Renderer* renderer);

void setColour(Texture* tex, uint8_t red, uint8_t green, uint8_t blue);

void renderTexture(Texture* tex, SDL_Renderer* renderer, int x, int y);
void renderTextureOptions(Texture* tex, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);
