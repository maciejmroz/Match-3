#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <sstream>
#include <vector>

#include "Renderer.h"

const int WIN_WIDTH = 755;
const int WIN_HEIGHT = 600;

static const char *textureNames[] = {
	"../assets/RS_bg.jpg",
	"../assets/RS_gem_blue.png",
	"../assets/RS_gem_green.png",
	"../assets/RS_gem_purple.png",
	"../assets/RS_gem_red.png",
	"../assets/RS_gem_yellow.png",
};

struct SDLRenderer::impl
{
	SDL_Window					*win;
	SDL_Renderer				*ren;
	std::vector<SDL_Texture*>	textures;
	TTF_Font					*defaultFont;

	impl();
	~impl();

	void validateTexture(TextureID tid);

	void clear();
	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void setClipRect(int x, int y, int w, int h);
	void resetClipRect();
	void drawBackground(TextureID tid);
	void drawTexture(TextureID tid, int x, int y);
	void drawTextureCentered(TextureID tid, int x, int y, int w, int h, double scale);
	void drawFilledRectangle(int x, int y, int w, int h);
	void drawText(const char *text, int x, int y);
	void present();

};

SDLRenderer::impl::impl() :
win(nullptr),
ren(nullptr),
defaultFont(nullptr)
{
	std::ostringstream errorStream;
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		errorStream << "SDL_Init Error: "  << SDL_GetError();
		throw new RendererException(errorStream.str());
	}

	win = SDL_CreateWindow("Match 3 game", 100, 100, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	if(nullptr == win)
	{
		errorStream << "SDL_CreateWindow Error: " << SDL_GetError();
		throw new RendererException(errorStream.str());
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(nullptr == ren)
	{
		errorStream << "SDL_CreateRenderer Error: " << SDL_GetError();
		throw new RendererException(errorStream.str());
	}

	static_assert(TID_LAST == sizeof(textureNames)/sizeof(char*), "textureNames array size must match TextureID enumeration!");

	for(int i = 0; i < TID_LAST; ++i)
	{
		SDL_Texture *tex = IMG_LoadTexture(ren, textureNames[i]);
		if(nullptr == tex)
		{
			errorStream << "IMG_LoadTexture Error: " << SDL_GetError();
			throw new RendererException(errorStream.str());
		}
		textures.push_back(tex);
	}

	//init ttf font
	if(TTF_Init() != 0)
	{
		errorStream << "TTF init error.";
		throw new RendererException(errorStream.str());
	}

	defaultFont = TTF_OpenFont("../assets/Bangers.ttf", 48);
	if (defaultFont == nullptr)
	{
		errorStream << "Font loading error.";
		throw new RendererException(errorStream.str());
	}
}

SDLRenderer::impl::~impl()
{
	TTF_CloseFont(defaultFont);
	for(auto tex: textures)
	{
		SDL_DestroyTexture(tex);
	}
	textures.clear();
	if(ren)
	{
		SDL_DestroyRenderer(ren);
	}
	if(win)
	{
		SDL_DestroyWindow(win);
	}
	SDL_Quit();
}

void SDLRenderer::impl::validateTexture(TextureID tid)
{
	if(tid >= TID_LAST || tid < 0)
	{
		std::ostringstream errorStream;
		errorStream << "Invalid texture id: "  << tid;
		throw new RendererException(errorStream.str());
	}
}

void SDLRenderer::impl::clear()
{
	SDL_RenderClear(ren);
}

void SDLRenderer::impl::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(ren, r, g, b, a);
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
}

void SDLRenderer::impl::setClipRect(int x, int y, int w, int h)
{
	SDL_Rect cr;
	cr.x = x;
	cr.y = y;
	cr.w = w;
	cr.h = h;
	SDL_RenderSetClipRect(ren, &cr);
}

void SDLRenderer::impl::resetClipRect()
{
	SDL_RenderSetClipRect(ren, NULL);
}

void SDLRenderer::impl::drawBackground(TextureID tid)
{
	validateTexture(tid);
	SDL_RenderCopy(ren, textures[tid], NULL, NULL);
}

void SDLRenderer::impl::drawTexture(TextureID tid, int x, int y)
{
	validateTexture(tid);
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_QueryTexture(textures[tid], NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, textures[tid], NULL, &dst);
}

void SDLRenderer::impl::drawTextureCentered(TextureID tid, int x, int y, int w, int h, double scale)
{
	validateTexture(tid);
	int textureWidth, textureHeight;
	SDL_QueryTexture(textures[tid], NULL, NULL, &textureWidth, &textureHeight);

	textureWidth = (int)(textureWidth * scale);
	textureHeight = (int)(textureHeight * scale);

	SDL_Rect dst;
	dst.x = x + (w - textureWidth) / 2;
	dst.y = y + (h - textureHeight) / 2;
	dst.w = textureWidth;
	dst.h = textureHeight;
	SDL_RenderCopy(ren, textures[tid], NULL, &dst);
}

void SDLRenderer::impl::drawFilledRectangle(int x, int y, int w, int h)
{
	SDL_Rect rectangle;

	rectangle.x = x;
	rectangle.y = y;
	rectangle.w = w;
	rectangle.h = h;
	SDL_RenderFillRect(ren, &rectangle);
}

void SDLRenderer::impl::drawText(const char *text, int x, int y)
{
	SDL_Color col;
	col.r = col.g = col.b = 255;
	col.a = 225;
	SDL_Surface *surf = TTF_RenderText_Blended(defaultFont, text, col);
	if(nullptr == surf)
	{
		return;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
	if(nullptr == texture)
	{
		return;
	}
	SDL_FreeSurface(surf);

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, texture, NULL, &dst);

	SDL_DestroyTexture(texture);
}

void SDLRenderer::impl::present()
{
	SDL_RenderPresent(ren);
}

SDLRenderer::SDLRenderer()
{
	pimpl = std::unique_ptr<impl>(new impl());
}

SDLRenderer::~SDLRenderer()
{
}

void SDLRenderer::clear()
{
	pimpl->clear();
}

void SDLRenderer::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	pimpl->setColor(r, g, b, a);
}

void SDLRenderer::setClipRect(int x, int y, int w, int h)
{
	pimpl->setClipRect(x, y, w, h);
}

void SDLRenderer::resetClipRect()
{
	pimpl->resetClipRect();
}

void SDLRenderer::drawBackground(TextureID tid)
{
	pimpl->drawBackground(tid);
}

void SDLRenderer::drawTexture(TextureID tid, int x, int y)
{
	pimpl->drawTexture(tid, x, y);
}

void SDLRenderer::drawTextureCentered(TextureID tid, int x, int y, int w, int h, double scale)
{
	pimpl->drawTextureCentered(tid, x, y, w, h, scale);
}

void SDLRenderer::drawFilledRectangle(int x, int y, int w, int h)
{
	pimpl->drawFilledRectangle(x, y, w, h);
}

void SDLRenderer::drawText(const char *text, int x, int y)
{
	pimpl->drawText(text, x, y);
}

void SDLRenderer::present()
{
	pimpl->present();
}
