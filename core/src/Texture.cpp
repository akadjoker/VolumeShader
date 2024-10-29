#include "pch.h"
#include "Texture.hpp"

Texture::Texture() : textureID(0)
{
}

Texture::~Texture()
{
    if (textureID != 0)
    {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}
void Texture::SetFilter(GLenum minFilter, GLenum magFilter)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetWrap(GLenum wrapS, GLenum wrapT)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::GenerateMipmaps()
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::CreateOpenglTexture(int w, int h, int components, unsigned char *data)
{
    if (textureID != 0)
    {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format = GL_RGBA;
    if (components == 1)
    {
        format = GL_RED;
    }
    else if (components == 3)
    {
        format = GL_RGB;
    }
    else if (components == 4)
    {
        format = GL_RGBA;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    return true;
}

Texture2D::~Texture2D()
{
}

void Texture2D::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture2D::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D Texture2D::GetDefaultTexture()
{
    static Texture2D defaultTexture(32,32);
    return defaultTexture;
}

Texture2D::Texture2D() : width(0), height(0), components(0) {}

Texture2D::Texture2D(int w, int h) : width(w), height(h), components(4)
{
    unsigned char *data = new unsigned char[w * h * components];
    std::fill_n(data, w * h * components, 255); // Inicializa com branco
    CreateOpenglTexture(w, h, components, data);
    delete[] data;
}

Texture2D::Texture2D(const Pixmap &pixmap)
{
    Load(pixmap);
}

Texture2D::Texture2D(const std::string &path)
{
    Load(path);
}



bool Texture2D::Load(const Pixmap &pixmap)
{
    width = pixmap.width;
    height = pixmap.height;
    components = pixmap.components;
    return CreateOpenglTexture(width, height, components, pixmap.pixels);
}

bool Texture2D::Load(const std::string &path)
{
    Pixmap pixmap;
    if (!pixmap.Load(path.c_str()))
        return false;
    return Load(pixmap);
}

bool Texture2D::LoadFromMemory(const unsigned char *buffer, u16 components, int width, int height)
{
    this->width = width;
    this->height = height;
    this->components = components;
    return CreateOpenglTexture(width, height, components, const_cast<unsigned char *>(buffer));
}

