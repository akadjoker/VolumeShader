#pragma once

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include "Config.hpp"
#include "Color.hpp"
#include "Utils.hpp"
#include "glm/glm.hpp"


class  Pixmap
{
public:
    Pixmap();
    ~Pixmap();
    Pixmap(int w, int h, int components);
    Pixmap(int w, int h, int components, unsigned char *data);

    Pixmap(const Pixmap &image,const IntRect &crop);
    Pixmap(const Pixmap &other) = delete;
    Pixmap &operator=(const Pixmap &other) = delete;
    Pixmap(Pixmap&&) = delete;

    void SetPixel(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a);
    void SetPixel(u32 x, u32 y, u32 rgba);

    u32 GetPixel(u32 x, u32 y) const;
    Color GetPixelColor(u32 x, u32 y) const;

    void Fill(u8 r, u8 g, u8 b, u8 a);
    void Fill(u32 rgba);
    
    bool Save(const char* file_name);
    
    void Clear();

    void DrawRectangle(int x, int y, int w, int h, u8 r, u8 g, u8 b, u8 a, bool filled = true);

    void DrawCircle(int xc, int yc, int radius, u8 r, u8 g, u8 b, u8 a);


    void DrawLine(int x1, int y1, int x2, int y2, u8 r, u8 g, u8 b, u8 a);


    bool Load(const char* file_name);
    bool LoadFromMemory(const unsigned char *buffer,   unsigned int bytesRead);


    void FlipVertical();
    void FlipHorizontal();




    unsigned char *pixels;
    int components;
    int width;
    int height;
};


