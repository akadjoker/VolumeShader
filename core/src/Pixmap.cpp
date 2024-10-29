#include "pch.h"
#include "Pixmap.hpp"

Pixmap::Pixmap()
{

    width = 0;
    height = 0;
    components = 0;
    pixels = nullptr;
}

Pixmap::~Pixmap()
{
    if (pixels)
    {
        delete[] pixels;
    }
}

Pixmap::Pixmap(const Pixmap &image, const IntRect &crop)
{
    width = crop.width;
    height = crop.height;
    components = image.components;
    pixels = (unsigned char *)malloc(width * height * components);
    for (int y = (int)crop.y, offsetSize = 0; y < (int)(crop.y + crop.height); y++)
    {
        memcpy(pixels + offsetSize, ((unsigned char *)image.pixels) + (y * image.width + (int)crop.x) * components, (int)crop.width * components);
        offsetSize += ((int)crop.width * components);
    }
}

Pixmap::Pixmap(int w, int h, int components)
{

    width = w;
    height = h;
    this->components = components;
    pixels = (unsigned char *)malloc(w * h * components);
}

Pixmap::Pixmap(int w, int h, int components, unsigned char *data)
{

    pixels = (unsigned char *)malloc(w * h * components);
    width = w;
    height = h;
    this->components = components;
    memcpy(pixels, data, w * h * components);
}

void Pixmap::Clear()
{
    if (pixels)
    {
        for (int i = 0; i < width * height * components; i++)
        {
            pixels[i] = 0;
        }
    }
}

void Pixmap::SetPixel(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a)
{

    if (x >= (u32)width || y >= (u32)height)
        return;

    if (components == 1)
    {
        glm::vec3 coln((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
        unsigned char gray = (unsigned char)((coln.x * 0.299f + coln.y * 0.587f + coln.z * 0.114f) * 255.0f);
        ((u8 *)pixels)[y * width + x] = gray;
    }
    else if (components == 2)
    {
        glm::vec3 coln((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
        u8 gray = (u8)((coln.x * 0.299f + coln.y * 0.587f + coln.z * 0.114f) * 255.0f);

        ((u8 *)pixels)[(y * width + x) * 2] = gray;
        ((u8 *)pixels)[(y * width + x) * 2 + 1] = a;
    }
    else if (components == 3)
    {

        ((u8 *)pixels)[(y * width + x) * 3] = r;
        ((u8 *)pixels)[(y * width + x) * 3 + 1] = g;
        ((u8 *)pixels)[(y * width + x) * 3 + 2] = b;
    }
    else if (components == 4)
    {
        ((u8 *)pixels)[(y * width + x) * 4] = r;
        ((u8 *)pixels)[(y * width + x) * 4 + 1] = g;
        ((u8 *)pixels)[(y * width + x) * 4 + 2] = b;
        ((u8 *)pixels)[(y * width + x) * 4 + 3] = a;
    }
}

void Pixmap::SetPixel(u32 x, u32 y, u32 rgba)
{

    if (x >= (u32)width || y >= (u32)height)
        return;

    u8 r = rgba;
    u8 g = rgba >> 8;
    u8 b = rgba >> 16;
    u8 a = rgba >> 24;
    SetPixel(x, y, r, g, b, a);
}

u32 Pixmap::GetPixel(u32 x, u32 y) const
{

    if (x >= (u32)width || y >= (u32)height)
        return 0;

    if (components == 1)
    {
        return pixels[y * width + x];
    }
    else if (components == 2)
    {
        return pixels[(y * width + x) * 2] | (pixels[(y * width + x) * 2 + 1] << 8);
    }
    else if (components == 3)
    {
        return pixels[(y * width + x) * 3] | (pixels[(y * width + x) * 3 + 1] << 8) | (pixels[(y * width + x) * 3 + 2] << 16);
    }
    else if (components == 4)
    {
        return pixels[(y * width + x) * 4] | (pixels[(y * width + x) * 4 + 1] << 8) | (pixels[(y * width + x) * 4 + 2] << 16) | (pixels[(y * width + x) * 4 + 3] << 24);
    }

    return 0;
}

Color Pixmap::GetPixelColor(u32 x, u32 y) const
{

    Color color = Color::BLACK;

    if ((x < (u32)width) && (y < (u32)height))
    {

        if (components == 1)
        {
            color.r = (u8)((u8 *)pixels)[y * width + x];
            color.g = (u8)((u8 *)pixels)[y * width + x];
            color.b = (u8)((u8 *)pixels)[y * width + x];
            color.a = 255;
        }
        else if (components == 2)
        {
            color.r = (u8)((u8 *)pixels)[(y * width + x) * 2];
            color.g = (u8)((u8 *)pixels)[(y * width + x) * 2];
            color.b = (u8)((u8 *)pixels)[(y * width + x) * 2];
            color.a = (u8)((u8 *)pixels)[(y * width + x) * 2 + 1];
        }
        else if (components == 3)
        {
            color.r = (u8)((u8 *)pixels)[(y * width + x) * 3];
            color.g = (u8)((u8 *)pixels)[(y * width + x) * 3 + 1];
            color.b = (u8)((u8 *)pixels)[(y * width + x) * 3 + 2];
            color.a = 255;
        }
        else if (components == 4)
        {
            color.r = (u8)((u8 *)pixels)[(y * width + x) * 4];
            color.g = (u8)((u8 *)pixels)[(y * width + x) * 4 + 1];
            color.b = (u8)((u8 *)pixels)[(y * width + x) * 4 + 2];
            color.a = (u8)((u8 *)pixels)[(y * width + x) * 4 + 3];
        }
    }
    return color;
}

void Pixmap::Fill(u8 r, u8 g, u8 b, u8 a)
{
    for (u32 y = 0; y < (u32)height; y++)
    {
        for (u32 x = 0; x < (u32)width; x++)
        {
            SetPixel(x, y, r, g, b, a);
        }
    }
}

void Pixmap::Fill(u32 rgba)
{
    for (u32 y = 0; y < (u32)height; y++)
    {
        for (u32 x = 0; x < (u32)width; x++)
        {
            SetPixel(x, y, rgba);
        }
    }
}

bool Pixmap::Load(const char *file_name)
{

    SDL_Surface *surface = IMG_Load(file_name);
    if (!surface)
    {
        return false;
    }

    components = surface->format->BytesPerPixel;

    width = surface->w;
    height = surface->h;

    pixels = new unsigned char[width * height * components];
    std::memcpy(pixels, surface->pixels, width * height * components);

    SDL_FreeSurface(surface);

    LogInfo("PIXMAP: Load image: %s (%d,%d) bpp:%d", file_name, width, height, components);
    return true;
}

bool Pixmap::LoadFromMemory(const unsigned char *buffer, unsigned int bytesRead)
{

    SDL_RWops *rw = SDL_RWFromConstMem(buffer, bytesRead);
    if (!rw)
        return false;

    SDL_Surface *surface = IMG_Load_RW(rw, 1);
    if (!surface)
        return false;

    components = surface->format->BytesPerPixel;
    width = surface->w;
    height = surface->h;
    pixels = new unsigned char[width * height * components];
    std::memcpy(pixels, surface->pixels, width * height * components);

    SDL_FreeSurface(surface);
    return true;
}

bool Pixmap::Save(const char *file_name)
{
    if (pixels == nullptr)
    {
        LogError("Failed to save image: %s", file_name);
        return false;
    }

    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
        pixels, width, height, components * 8, width * components,
        0x000000ff, 0x0000ff00, 0x00ff0000, components == 4 ? 0xff000000 : 0);

    if (!surface)
    {
        return false;
    }

    bool success = (SDL_SaveBMP(surface, file_name) == 0);

    SDL_FreeSurface(surface);
    return success;
}

void Pixmap::FlipVertical()
{
    if (pixels == nullptr)
    {
        LogError("Failed to flip image");
        return;
    }
    int rowSize = width * components;
    unsigned char *row = (unsigned char *)malloc(rowSize);
    for (int y = 0; y < height / 2; y++)
    {
        unsigned char *src = ((unsigned char *)pixels) + y * rowSize;
        unsigned char *dst = ((unsigned char *)pixels) + (height - y - 1) * rowSize;
        memcpy(row, src, rowSize);
        memcpy(src, dst, rowSize);
        memcpy(dst, row, rowSize);
    }
    free(row);
}

void Pixmap::FlipHorizontal()
{
    if (pixels == nullptr)
    {
        LogError("Failed to flip image");
        return;
    }
    int rowSize = width * components;
    unsigned char *row = (unsigned char *)malloc(rowSize);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            unsigned char *src = ((unsigned char *)pixels) + y * rowSize + x * components;
            unsigned char *dst = ((unsigned char *)pixels) + y * rowSize + (width - x - 1) * components;
            memcpy(row, src, components);
            memcpy(src, dst, components);
            memcpy(dst, row, components);
        }
    }
    free(row);
}
void Pixmap::DrawLine(int x0, int y0, int x1, int y1, u8 r, u8 g, u8 b, u8 a)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true)
    {
        SetPixel(x0, y0, r, g, b, a);

        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}
void Pixmap::DrawRectangle(int x, int y, int w, int h, u8 r, u8 g, u8 b, u8 a, bool filled )
{
    if (filled)
    {
        for (int i = x; i < x + w; i++)
        {
            for (int j = y; j < y + h; j++)
            {
                SetPixel(i, j, r, g, b, a);
            }
        }
    }
    else
    {
        DrawLine(x, y, x + w - 1, y, r, g, b, a);                 // Topo
        DrawLine(x, y + h - 1, x + w - 1, y + h - 1, r, g, b, a); // Fundo
        DrawLine(x, y, x, y + h - 1, r, g, b, a);                 // Esquerda
        DrawLine(x + w - 1, y, x + w - 1, y + h - 1, r, g, b, a); // Direita
    }
}
void Pixmap::DrawCircle(int xc, int yc, int radius, u8 r, u8 g, u8 b, u8 a)
{
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    while (y >= x)
    {
        SetPixel(xc + x, yc + y, r, g, b, a);
        SetPixel(xc - x, yc + y, r, g, b, a);
        SetPixel(xc + x, yc - y, r, g, b, a);
        SetPixel(xc - x, yc - y, r, g, b, a);
        SetPixel(xc + y, yc + x, r, g, b, a);
        SetPixel(xc - y, yc + x, r, g, b, a);
        SetPixel(xc + y, yc - x, r, g, b, a);
        SetPixel(xc - y, yc - x, r, g, b, a);
        x++;
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}
