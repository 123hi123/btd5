#include "Board.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"

Board::Board(std::string imagePath, glm::vec2 position, glm::vec2 size){
    SetImage(imagePath);
    SetPosition(position);
    SetScale(size);
    SetVisible(false);
    SetZIndex(10);
};

void Board::SetPosition(const glm::vec2& Position) {
    m_Transform.translation = Position;
}

void Board::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

void Board::SetScale(const glm::vec2& Scale){
    m_Transform.scale = Scale;
}

void Board::AddButton(const std::shared_ptr<Button>& child) {
    m_Buttons.push_back(child);
}

void Board::UpdateVisible(bool isVisable) {
    SetVisible(isVisable);
    for (auto& child : m_Buttons) {
        child-> UpdateVisible(isVisable);
    }
}

int Board::IsClicked(glm::vec2 mousePosition){
    for (int i = 0; i < m_Buttons.size(); i++) {
        if(m_Buttons[i]->IsClicked(mousePosition)){
          return i;
        }
    }
    return -1;
}

void Board::IsTouch(glm::vec2 mousePosition){
    for(auto& child : m_Buttons) {
        child->IsTouch(mousePosition);
    }
}

std::vector<std::shared_ptr<Button>> Board::GetAllChildren(){
    return m_Buttons;
};
