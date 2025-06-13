#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <vector>
#include <string>
#include <memory>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class Button : public Util::GameObject {
public:

    explicit Button(std::string imagePath, glm::vec2 position, glm::vec2 scale, int width, int height);

    void SetPosition(const glm::vec2& Position);
    void SetImage(const std::string& ImagePath);
    void SetScale(const glm::vec2& Scale);
    void AddChild(const std::shared_ptr<GameObject>& child);
    void UpdateVisible(bool isVisible);
    void IsTouch(glm::vec2 mousePosition);
    void TouchHint();
    void SetMaxZ(int maxZ);

    [[nodiscard]] bool IsClicked(glm::vec2 mousePosition);


private:
    int m_Width;
    int m_Height;
    int max_zIndex = 12;
    glm::vec2 m_Scale = glm::vec2(1, 1);
    std::string m_ImagePath;
    std::vector<std::shared_ptr<GameObject>> m_Children;
};

#endif //BUTTON_HPP
