#include "Texture.h"
#include <GL/glew.h>
#include <SDL3_image/SDL_image.h>
#include <memory>

Texture::Texture(unsigned int id) : id(id) {}

Texture::~Texture() { glDeleteTextures(1, &id); }

std::shared_ptr<Texture> Texture::load(const std::string &path) {
    SDL_Surface *image = IMG_Load(path.c_str());
    if (!image) {
        SDL_Log("Failed to load image %s: %s", path.c_str(), SDL_GetError());
        return nullptr;
    }

    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    return std::shared_ptr<Texture>(new Texture(id));
}
