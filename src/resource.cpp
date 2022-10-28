#include "resource.h"
#include <stb_image.h>

namespace yc {

yc::gl::Shader Resource::OpaqueShader;
yc::gl::Shader Resource::TransparentShader;
yc::gl::Shader Resource::FloraShader;
yc::gl::Shader Resource::GrayScaleShader;
yc::gl::Shader Resource::SkyBoxShader;
yc::gl::Shader Resource::BlockOutlineShader;
yc::gl::Shader Resource::CrossHairShader;
yc::gl::Shader Resource::CompositeShader;
yc::gl::Texture Resource::GameTexure;
yc::gl::Texture Resource::CrossHairTexture;
yc::gl::Texture Resource::IconTexture;
yc::gl::Texture Resource::ResumeButtonTexture;
yc::gl::Texture Resource::HoveredResumeButtonTexture;
yc::gl::Texture Resource::BackButtonTexture;
yc::gl::Texture Resource::HoveredBackButtonTexture;

std::map<yc::world::BlockType, yc::gl::Texture> Resource::BlockIcons;

void Resource::Load() {

    Resource::OpaqueShader.loadFromFile("opaque.vert", "opaque.frag");
    Resource::TransparentShader.loadFromFile("transparent.vert", "transparent.frag");
    Resource::FloraShader.loadFromFile("flora.vert", "flora.frag");
    Resource::GrayScaleShader.loadFromFile("grayscale.vert", "grayscale.frag");
    Resource::SkyBoxShader.loadFromFile("skybox.vert", "skybox.frag");
    Resource::BlockOutlineShader.loadFromFile("block_outline.vert", "block_outline.frag");
    Resource::CrossHairShader.loadFromFile("crosshair.vert", "crosshair.frag");
    Resource::CompositeShader.loadFromFile("composite.vert", "composite.frag");


    stbi_set_flip_vertically_on_load(true);
    Resource::GameTexure.loadFromFile("./resources/texture.png");
    Resource::CrossHairTexture.loadFromFile("./resources/crosshair.png");
    stbi_set_flip_vertically_on_load(false);

    Resource::ResumeButtonTexture.loadFromFile("./resources/gui/resume_button.png");
    Resource::HoveredResumeButtonTexture.loadFromFile("./resources/gui/hovered_resume_button.png");
    Resource::BackButtonTexture.loadFromFile("./resources/gui/back_button.png");
    Resource::HoveredBackButtonTexture.loadFromFile("./resources/gui/hovered_back_button.png");

    Resource::BlockIcons[world::BlockType::GRASS_BLOCK].loadFromFile("./resources/icons/grass_block.png");
    Resource::BlockIcons[world::BlockType::DIRT].loadFromFile("./resources/icons/dirt.png");
    Resource::BlockIcons[world::BlockType::GLASS].loadFromFile("./resources/icons/glass.png");
    Resource::BlockIcons[world::BlockType::STONE].loadFromFile("./resources/icons/stone.png");
    Resource::BlockIcons[world::BlockType::SNOW].loadFromFile("./resources/icons/snow.png");
    Resource::BlockIcons[world::BlockType::SAND].loadFromFile("./resources/icons/sand.png");
    Resource::BlockIcons[world::BlockType::WOOD].loadFromFile("./resources/icons/wood.png");
    Resource::BlockIcons[world::BlockType::LEAF].loadFromFile("./resources/icons/leaf.png");
    Resource::BlockIcons[world::BlockType::RED_FLOWER].loadFromFile("./resources/icons/red_flower.png");
    Resource::BlockIcons[world::BlockType::BLUE_FLOWER].loadFromFile("./resources/icons/blue_flower.png");
    Resource::BlockIcons[world::BlockType::YELLOW_FLOWER].loadFromFile("./resources/icons/yellow_flower.png");
    Resource::BlockIcons[world::BlockType::GRASS].loadFromFile("./resources/icons/grass.png");
    Resource::BlockIcons[world::BlockType::WATER].loadFromFile("./resources/icons/water.png");
}

}