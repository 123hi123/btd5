#ifndef INFORMATIONIMAGE_HPP
#define INFORMATIONIMAGE_HPP

#include <vector>
#include <string>
#include <memory>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class InformationImage : public Util::GameObject {
public:

    explicit InformationImage(std::string ImagePath, glm::vec2 Position);

    void SetPosition(const glm::vec2& Position);
    void SetImage(const std::string& ImagePath);

private:
    std::string m_ImagePath;
};

#endif //INFORMATIONIMAGE_HPP
