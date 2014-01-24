#include "Renderer.h"

NullRenderer::NullRenderer()
{
}

NullRenderer::~NullRenderer()
{
}

void NullRenderer::clear()
{
}

void NullRenderer::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
}

void NullRenderer::setClipRect(int x, int y, int w, int h)
{
}

void NullRenderer::resetClipRect()
{
}

void NullRenderer::drawBackground(TextureID tid)
{
}

void NullRenderer::drawTexture(TextureID tid, int x, int y)
{
}

void NullRenderer::drawTextureCentered(TextureID tid, int x, int y, int w, int h, double scale)
{
}

void NullRenderer::drawFilledRectangle(int x, int y, int w, int h)
{
}

void NullRenderer::drawText(const char *text, int x, int y)
{
}

void NullRenderer::present()
{
}
