#ifndef COUNTERTEXT_HPP
#define COUNTERTEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Util/Logger.hpp"

class CounterText : public Util::GameObject {
public:
    CounterText() : GameObject(
    std::make_unique<Util::Text>(GA_RESOURCE_DIR"/Font/Inkfree.ttf", 30,
    append_string_views("0", "0"),Util::Color::FromName(Util::Colors::WHITE)),100) {
        m_Transform.translation = {0.0F, 0.0F};
    }

    void Update(const int current, const int maxValue) {
        auto temp = std::dynamic_pointer_cast<Util::Text>(m_Drawable);
        temp->SetText(append_string_views(std::to_string(current), std::to_string(maxValue)));
    }

    void SetPosition(glm::vec2 position) {
        m_Transform.translation = position;
    }

private:
    inline static std::string append_string_views(std::string current, std::string maxValue) {
        return current + "/" + maxValue;
    }
};

#endif //COUNTERTEXT_HPP
