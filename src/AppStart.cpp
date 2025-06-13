#include "App.hpp"

#include "Util/Logger.hpp"
#include "Balloon.hpp"

void App::Start() {
    LOG_TRACE("Start");
    m_PRM = std::make_shared<PhaseResourceManger>();
    m_Root.AddChildren(m_PRM->GetChildren());
    ValidTask(0);
    m_CurrentState = State::UPDATE;
}
