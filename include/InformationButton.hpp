#ifndef INFORMATIONBUTTON_HPP
#define INFORMATIONBUTTON_HPP

#include <vector>
#include <string>
#include <memory>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "InformationText.hpp"

class InformationButton : public Util::GameObject {
public:

    explicit InformationButton(glm::vec2 Position, std::string cost);

    void SetPosition(const glm::vec2& Position);
    void SetImage(const std::string& ImagePath);
    void Update(std::string cost);
    void UpdateVisible(bool visible);
    void TouchHint();
    void IsTouch(glm::vec2 mousePosition);

    [[nodiscard]] std::vector<std::shared_ptr<GameObject>> GetAllChildren();

private:
    int m_width = 149;
    int m_height = 41;
    std::string m_ImagePath;
    std::shared_ptr<InformationText> costText;
};

#endif //INFORMATIONBUTTON_HPP
