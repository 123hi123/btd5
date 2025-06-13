#include "InformationImage.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"

InformationImage::InformationImage(std::string ImagePath, glm::vec2 Position){
    SetImage(ImagePath);
    SetPosition(Position);
    SetVisible(false);
    SetZIndex(6);
};

void InformationImage::SetPosition(const glm::vec2& Position) {
    m_Transform.translation = Position;
}

void InformationImage::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}