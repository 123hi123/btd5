#ifndef INFORMATIONTEXT_HPP
#define INFORMATIONTEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Util/Logger.hpp"

class InformationText : public Util::GameObject {
public:
    InformationText(glm::vec2 position, std::string text, int size) : GameObject(
    std::make_unique<Util::Text>(GA_RESOURCE_DIR"/Font/Inkfree.ttf",size,
    text,Util::Color::FromName(Util::Colors::WHITE)),100) {
        SetVisible(false);
        SetPosition(position);
        SetZIndex(8);
    }

    void Update(std::string text) {
        auto temp = std::dynamic_pointer_cast<Util::Text>(m_Drawable);
        temp->SetText(text);
    }

    void SetPosition(glm::vec2 position) {
        m_Transform.translation = position;
    }

    void SetScale(glm::vec2 scale) {
        m_Transform.scale = scale;
    }
};

#endif //INFORMATIONTEXT_HPP
