#include "Button.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"

Button::Button(std::string imagePath, glm::vec2 position, glm::vec2 scale, int width, int height){
    SetImage(imagePath);
    SetPosition(position);
    SetScale(scale);
    m_Scale = scale;
    m_Width = width;
    m_Height = height;
    SetVisible(false);
    SetZIndex(11);
};

void Button::SetPosition(const glm::vec2& Position) {
    m_Transform.translation = Position;
}

void Button::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

void Button::SetScale(const glm::vec2& Scale){
    m_Transform.scale = Scale;
}

void Button::AddChild(const std::shared_ptr<GameObject>& child) {
    m_Children.push_back(child);
}

void Button::UpdateVisible(bool isVisable) {
    SetVisible(isVisable);
    for (auto& child : m_Children) {
        child->SetVisible(isVisable);
    }
}

void Button::IsTouch(glm::vec2 mousePosition){
    glm::vec2 position = m_Transform.translation;
    float x = mousePosition.x;
    float y = mousePosition.y;
    float width_half = m_Width/2;
    float height_half = m_Height/2;
    if (x > position.x - width_half && x < position.x + width_half && y > position.y - height_half && y < position.y + height_half) {
        TouchHint();
    }
    else{
        SetScale(m_Scale);
        SetZIndex(max_zIndex-1);
        for (auto& child : m_Children) {
            child -> m_Transform.scale = glm::vec2(1, 1);
        }
    }
}

void Button::TouchHint(){
    SetScale(m_Scale + glm::vec2(0.2, 0.2));
    SetZIndex(max_zIndex);
    for (auto& child : m_Children) {
        child -> m_Transform.scale = glm::vec2(1.2, 1.2);
    }
}

bool Button::IsClicked(glm::vec2 mousePosition){
    glm::vec2 position = m_Transform.translation;
    float x = mousePosition.x;
    float y = mousePosition.y;
    float width_half = m_Width/2;
    float height_half = m_Height/2;
    if (x > position.x - width_half && x < position.x + width_half && y > position.y - height_half && y < position.y + height_half) {
        return true;
    }
    return false;
}

void Button::SetMaxZ(int maxZ) {
    max_zIndex = maxZ;
    SetZIndex(max_zIndex-1);
}