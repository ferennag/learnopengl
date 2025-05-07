#pragma once

#include <memory>
#include <string>

class Texture {
  private:
    unsigned int id;

    Texture(unsigned int id);

  public:
    ~Texture();

    static std::shared_ptr<Texture> load(const std::string &path);

    unsigned int getId() const { return id; }
};
