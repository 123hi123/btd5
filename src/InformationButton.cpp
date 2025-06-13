#include "InformationButton.hpp"

InformationButton::InformationButton(glm::vec2 position, std::string cost){
    SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Button.png");
    SetPosition(position);
    SetZIndex(7);
    SetVisible(false);
    costText = std::make_shared<InformationText>(position, cost, 20);
};

void InformationButton::SetPosition(const glm::vec2& Position) {
    m_Transform.translation = Position;
}

void InformationButton::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

void InformationButton::Update(std::string cost){
    costText -> Update(cost);
}

std::vector<std::shared_ptr<Util::GameObject>> InformationButton::GetAllChildren(){
    return {costText};
}

void InformationButton::UpdateVisible(bool visible) {
    SetVisible(visible);
    costText -> SetVisible(visible);
}

void InformationButton::TouchHint() {
    m_Transform.scale = glm::vec2(1.3, 1.3);
    costText -> SetScale(glm::vec2(1.3, 1.3));
}

void InformationButton::IsTouch(glm::vec2 mousePosition) {
    glm::vec2 TopLeft = glm::vec2(m_Transform.translation.x - m_width/2, m_Transform.translation.y + m_height/2);
    glm::vec2 BottomRight = glm::vec2(m_Transform.translation.x + m_width/2, m_Transform.translation.y - m_height/2);
    if (mousePosition.x > TopLeft.x && mousePosition.x < BottomRight.x && mousePosition.y < TopLeft.y && mousePosition.y > BottomRight.y) {
        TouchHint();
    }
    else {
        m_Transform.scale = glm::vec2(1, 1);
        costText -> SetScale(glm::vec2(1, 1));
    }
}
