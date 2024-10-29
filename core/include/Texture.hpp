#pragma once

#include <string>
#include "Config.hpp"
#include "glad/glad.h"
#include "Utils.hpp"
#include "Pixmap.hpp"
class Texture
{
public:
    Texture();
    virtual ~Texture();

    virtual void Bind(unsigned int slot = 0) const = 0;
    virtual void Unbind() const = 0;

    GLuint GetID() const { return textureID; }
    bool CreateOpenglTexture(int w, int h, int components, unsigned char *data);
    void SetFilter(GLenum minFilter, GLenum magFilter);
    void SetWrap(GLenum wrapS, GLenum wrapT);
    void GenerateMipmaps();
protected:
    GLuint textureID;
    int width, height, components;
};

class Texture2D : public Texture
{
public:
    Texture2D();
    Texture2D(int w, int h);
    Texture2D(const Pixmap &pixmap);
    Texture2D(const std::string &path);
    ~Texture2D();

    bool Load(const Pixmap &pixmap);
    bool Load(const std::string &path);
    bool LoadFromMemory(const unsigned char *buffer, u16 components, int width, int height);

    void Bind(unsigned int slot = 0) const override;
    void Unbind() const override;

    static Texture2D  GetDefaultTexture();

private:
    int width, height, components;
    std::string filePath;
};
