#pragma once
#include "PunkDefines.hpp"

class PTexture
{
public:
    unsigned int id;
    std::string type;
    std::string path;

    PTexture();
    PTexture(const glm::vec4 color);

    void Load(const std::string& path, const std::string& directory, PTextureWrapping wrapping = PTextureWrapping::EdgeClamp, bool hdr = false, bool flip = true);
    void Load(const glm::vec4 color);

    void Resize(unsigned int width, unsigned int height, bool multisample = false);
    void Bind(unsigned int index);

    void LoadColor(const glm::vec2& size, bool multisample = false);
    void LoadDepth(const glm::vec2& size);

    static  std::shared_ptr<PTexture> Create(const std::string& path, const std::string& directory, PTextureWrapping wrapping = PTextureWrapping::EdgeClamp, bool hdr = false, bool flip = true);
    static  std::shared_ptr<PTexture> CreateColor(const glm::vec2& size, bool multisample = false);
    static  std::shared_ptr<PTexture> CreateDepth(const glm::vec2& size);
};