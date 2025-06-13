#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include <memory>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "Button.hpp"

class Board : public Util::GameObject {
public:

    explicit Board(std::string imagePath, glm::vec2 position, glm::vec2 size);

    void SetPosition(const glm::vec2& Position);
    void SetImage(const std::string& ImagePath);
    void SetScale(const glm::vec2& Scale);
    void AddButton(const std::shared_ptr<Button>& child);
    void UpdateVisible(bool isVisible);
    void IsTouch(glm::vec2 mousePosition);

    [[nodiscard]] int IsClicked(glm::vec2 mousePosition);
    [[nodiscard]] bool GetVisible() { return m_Visible; }
    [[nodiscard]] std::vector<std::shared_ptr<Button>> GetAllChildren();

private:
    std::string m_ImagePath;
    std::vector<std::shared_ptr<Button>> m_Buttons;
};

#endif //BOARD_HPP
