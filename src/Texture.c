#include "Texture.h"
#include <SDL2/SDL_image.h>
#include "core.h"

Texture* newTexture()
{
    Texture* tex = (Texture*)malloc(sizeof(Texture));
    tex->texture = NULL;
    tex->width = 0;
    tex->height = 0;

    return tex;
}

void deleteTexture(Texture* tex)
{
    if(tex)
    {
        if(tex->texture)
        {
            free(tex->texture);
        }

        free(tex);
    }
}

bool loadFromFile(Texture* tex, char const* path, SDL_Renderer* renderer)
{
	if(!tex) return false;

    if(tex->texture) free(tex->texture); tex->texture = NULL;

	SDL_Texture* newTex = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return false;
	}
    
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

    newTex = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if(newTex == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    tex->width = loadedSurface->w;
    tex->height = loadedSurface->h;
	tex->texture = newTex;

    SDL_FreeSurface(loadedSurface);

	return tex->texture != NULL;
}

void setColor(Texture* tex, uint8_t red, uint8_t green, uint8_t blue )
{
	SDL_SetTextureColorMod(tex->texture, red, green, blue);
}

void renderTexture(Texture* tex, SDL_Renderer* renderer, int x, int y)
{
    renderTextureOptions(tex, renderer, x, y, NULL, 0.0, NULL, SDL_FLIP_NONE);
}

void renderTextureOptions(Texture* tex, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x, y, tex->width, tex->height };

	if(clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(renderer, tex->texture, clip, &renderQuad, angle, center, flip);
}
