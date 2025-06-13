#ifndef ANIMATED_CHARACTER_HPP
#define ANIMATED_CHARACTER_HPP

#include <vector>
#include <string>

#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"


class AnimatedCharacter : public Util::GameObject {

public:
    explicit AnimatedCharacter(const std::vector<std::string>& AnimationPaths);

    [[nodiscard]] bool IsLooping() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetLooping();
    }

    [[nodiscard]] bool IsPlaying() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetState() == Util::Animation::State::PLAY;
    }

    void SetLooping(bool looping) {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->SetLooping(looping);
    }

    void SetPlaying() {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->Play();
    }

    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }

    [[nodiscard]] bool IfAnimationEnds() const;

    [[nodiscard]] bool GetVisibility() const { return m_Visible; }
};

#endif //ANIMATED_CHARACTER_HPP
