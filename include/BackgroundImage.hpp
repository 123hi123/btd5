#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"

class BackgroundImage : public Util::GameObject {

public:
    BackgroundImage() : GameObject(
            std::make_unique<Util::Image>(GA_RESOURCE_DIR"/Background/Lobby.png"), -10) {
    }

    void NextPhase(const std::string imagePath) {
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        temp->SetImage(ImagePath(imagePath));
    }

private:
    inline std::string ImagePath(const std::string imagePath) {
        return GA_RESOURCE_DIR"/Background/" + imagePath + ".png";
    }
};

#endif //BACKGROUND_IMAGE_HPP
