#include "PhaseResourceManger.hpp"

#include "Util/Logger.hpp"

PhaseResourceManger::PhaseResourceManger() {
    m_TaskText = std::make_shared<TaskText>();
    m_Background = std::make_shared<BackgroundImage>();
}

void PhaseResourceManger::NextPhase(int next) {
    m_Background->NextPhase(imagePaths[next]);
}

std::vector<std::vector<glm::vec2>> PhaseResourceManger::GetCoordinates(int level) const {
    return coordinates[level];
}

std::vector<std::vector<int>> PhaseResourceManger::GetBalloons(int level) const {
    return balloons[level];
}

std::vector<std::vector<std::vector<glm::vec2>>> PhaseResourceManger::GetPlaceable(int level) const {
    return nonplaceinfo[level];
}

