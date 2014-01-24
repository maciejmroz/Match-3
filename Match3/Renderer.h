
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <memory>
#include <string>

enum TextureID {
	TID_BACKGROUND,
	TID_BLOCK_1,
	TID_BLOCK_2,
	TID_BLOCK_3,
	TID_BLOCK_4,
	TID_BLOCK_5,
	TID_LAST
};

struct RendererException : public std::exception
{
	std::string			error;

	RendererException(const std::string &error) : error(error) {};
};

class Renderer
{
public:
	virtual void clear() = 0;
	virtual void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0;
	virtual void setClipRect(int x, int y, int w, int h) = 0;
	virtual void resetClipRect() = 0;
	virtual void drawBackground(TextureID tid) = 0;
	virtual void drawTexture(TextureID tid, int x, int y) = 0;
	virtual void drawTextureCentered(TextureID tid, int x, int y, int w, int h, double scale = 1.0) = 0;
	virtual void drawFilledRectangle(int x, int y, int w, int h) = 0;
	virtual void drawText(const char *text, int x, int y) = 0;
	virtual void present() = 0;
};

class SDLRenderer : public Renderer
{
	struct					impl;
	std::unique_ptr<impl>	pimpl;

public:
	SDLRenderer();
	~SDLRenderer();

	void clear();
	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void setClipRect(int x, int y, int w, int h);
	void resetClipRect();
	void drawBackground(TextureID tid);
	void drawTexture(TextureID tid, int x, int y);
	void drawTextureCentered(TextureID tid, int x, int y, int w, int h, double scale = 1.0);
	void drawFilledRectangle(int x, int y, int w, int h);
	void drawText(const char *text, int x, int y);
	void present();
};

//mock renderer class for testing
class NullRenderer : public Renderer
{
public:
	NullRenderer();
	~NullRenderer();

	void clear();
	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void setClipRect(int x, int y, int w, int h);
	void resetClipRect();
	void drawBackground(TextureID tid);
	void drawTexture(TextureID tid, int x, int y);
	void drawTextureCentered(TextureID tid, int x, int y, int w, int h, double scale = 1.0);
	void drawFilledRectangle(int x, int y, int w, int h);
	void drawText(const char *text, int x, int y);
	void present();
};

#endif