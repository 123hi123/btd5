#include "Range.hpp"
#include "Util/Logger.hpp"

Range::Range(glm::vec2 position, int radius)
{
    SetImage(GA_RESOURCE_DIR"/Monkey/range.png");
    SetPosition(position);
    SetScale(radius);
    SetZIndex(-9);
}

void Range::SetPosition(const glm::vec2& Position){
    m_Transform.translation = Position;
}

void Range::SetScale(int radius){
    float scale = (radius*2)/100.0;
    m_Transform.scale = glm::vec2(scale, scale);
}

void Range::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}