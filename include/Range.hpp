#ifndef RANGE_HPP
#define RANGE_HPP

#include <vector>
#include <string>
#include <memory>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class Range : public Util::GameObject {
public:
    explicit Range (glm::vec2 position, int radius);
    void SetPosition(const glm::vec2& Position);
    void SetScale(int radius);
    void SetImage(const std::string& ImagePath);

private:
    std::string m_ImagePath;

};

#endif //RANGE_HPP
