#ifndef LEVELINFORMATION_HPP
#define LEVELINFORMATION_HPP
#include "pch.hpp" // IWYU pragma: export

class Information {
public:
    explicit Information(std::vector<glm::vec2> coordinates, std::string imagePath,std::vector<std::vector<int>> balloons)
      : m_coordinates(coordinates), m_imagePath(imagePath), m_balloons(balloons) {};
    [[nodiscard]] std::vector<glm::vec2> GetCoordinates() const { return m_coordinates; }
    [[nodiscard]] std::string GetImagePath() const { return m_imagePath; }
    [[nodiscard]] std::vector<std::vector<int>> GetBalloons() const { return m_balloons; }
private:
    std::vector<glm::vec2> m_coordinates;
    std::string m_imagePath;
    std::vector<std::vector<int>> m_balloons;
};

class LevelInformation {
public:
    [[nodiscard]] Information GetInformation(int level) const { return m_level[level]; }
private:
    //Level_1
    std::vector<glm::vec2> coordinates =
           {glm::vec2(-523.0f, -222.0f),
            glm::vec2(-347.0f, -217.0f),
            glm::vec2(-346.0f, -32.0f),
            glm::vec2(-250.0f, -22.0f),
            glm::vec2(-242.0f, 170.0f),
            glm::vec2(-145.0f, 179.0f),
            glm::vec2(-139.0f, 340.0f),
            glm::vec2(-14.0f, 334.0f),
            glm::vec2(-10.0f, -270.0f),
            glm::vec2(126.0f, -259.0f),
            glm::vec2(128.0f, 339.0f),
            glm::vec2(257.0f, 342.0f),
            glm::vec2(262.0f, 175.0f),
            glm::vec2(361.0f, 167.0f),
            glm::vec2(374.0f, -28.0f),
            glm::vec2(458.0f, -31.0f),
            glm::vec2(465.0f, -217.0f),
            glm::vec2(554.0f, -222.0f)};
    std::string imagePath = "InTheWall";
    std::vector<std::vector<int>> balloons =
        {{1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1}};
    Information information_1 = Information(coordinates, imagePath, balloons);
    std::vector<Information> m_level = {information_1};
};
#endif //LEVELINFORMATION_HPP
