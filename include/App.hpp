#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Util/Text.hpp"
#include "PhaseResourceManger.hpp"
#include "Balloon.hpp"
#include "Attack.hpp"
#include "Counter.hpp"
#include "Monkey.hpp"
#include "DragButton.hpp"
#include "Board.hpp"
#include "Button.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    enum class Phase {
        LOBBY,
        FIRST_LEVEL,
        SECOND_LEVEL,
        THIRD_LEVEL,
        FOURTH_LEVEL,
        FIFTH_LEVEL,
        SIXTH_LEVEL,
        SEVENTH_LEVEL,
        EIGHTH_LEVEL,
        NINTH_LEVEL,
        TENTH_LEVEL,
        InfiniteMode,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    int GetFPS() {return m_FPS;}

private:
    void ValidTask(int next_room);
    void Reset();
    void SetLevel(int level);
    void AddBoard();
    void SetFPS(int fps) {m_FPS = fps;};


    State m_CurrentState = State::START;
    Phase m_Phase = Phase::LOBBY;
    std::vector<std::vector<glm::vec2>> Level_Coordinates;
    std::vector<std::vector<int>> Level_Balloons;
    std::vector<std::vector<std::vector<glm::vec2>>> Level_Placeable;
    Util::Renderer m_Root;
    std::vector<std::shared_ptr<Counter>> m_Counters;
    std::vector<std::shared_ptr<Balloon>> m_Balloons;
    std::vector<std::shared_ptr<Attack>> m_Attacks;
    std::vector<std::shared_ptr<Monkey>> m_Monkeys;
    std::shared_ptr<PhaseResourceManger> m_PRM;
    std::vector<std::shared_ptr<DragButton>> m_DragButtons;
    std::shared_ptr<Monkey> m_DragMonkey;
    std::shared_ptr<Monkey> m_testMonkey;
    std::shared_ptr<Monkey> m_ClickedMonkey;
    std::shared_ptr<Board> Win_Board;
    std::shared_ptr<Board> Lose_Board;
    std::shared_ptr<Board> Suspend_Board;
    std::shared_ptr<Button> Suspend_Button;
    std::shared_ptr<Button> Accelerate_Button;
    std::vector<std::shared_ptr<Button>> Lobby_Buttons;
    std::shared_ptr<Board> Choose_Level_Board;
    std::vector<bool> IsLevelUnlock = {true, false, false, false, false, false, false, false, false, false};
    int mode = 0;
    int m_FPS = 180;
    int level = 0;
    

    bool m_EnterDown = false;
};

#endif
