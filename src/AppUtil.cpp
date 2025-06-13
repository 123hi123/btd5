#include "AppUtil.hpp"
#include "App.hpp"
#include "../PTSD/lib/sdl2_mixer/src/codecs/stb_vorbis/stb_vorbis.h"

#include "Util/Logger.hpp"

/**
 * @brief The function to validate the tasks.
 * @warning Do not modify this function.
 * @note See README.md for the task details.
 */
void App::ValidTask(int next_room) {
    SetFPS(60);
    Reset();
    if (next_room > 10) {
        m_PRM->NextPhase(next_room%10);
    }
    else {
        m_PRM->NextPhase(next_room);
    }
    switch (next_room) {
        case 0:
            m_Phase = Phase::LOBBY;
            AddBoard();
            break;
        case 1:
            m_Phase = Phase::FIRST_LEVEL;
            SetLevel(1);
            AddBoard();
            break;
        case 2:
            m_Phase = Phase::SECOND_LEVEL;
            SetLevel(2);
            AddBoard();
            break;
        case 3:
            m_Phase = Phase::THIRD_LEVEL;
            SetLevel(3);
            AddBoard();
            break;
        case 4:
            m_Phase = Phase::FOURTH_LEVEL;
            SetLevel(4);
            AddBoard();
            break;
        case 5:
            m_Phase = Phase::FIFTH_LEVEL;
            SetLevel(5);
            AddBoard();
            break;
        case 6:
            m_Phase = Phase::SIXTH_LEVEL;
            SetLevel(6);
            AddBoard();
            break;
        case 7:
            m_Phase = Phase::SEVENTH_LEVEL;
            SetLevel(7);
            AddBoard();
            break;
        case 8:
            m_Phase = Phase::EIGHTH_LEVEL;
            SetLevel(8);
            AddBoard();
            break;
        case 9:
            m_Phase = Phase::NINTH_LEVEL;
            SetLevel(9);
            AddBoard();
            break;
        case 10:
            m_Phase = Phase::TENTH_LEVEL;
            SetLevel(10);
            AddBoard();
            break;
        default:
            m_Phase = Phase::InfiniteMode;
            LOG_DEBUG(1);
            SetLevel(next_room);
            LOG_DEBUG(2);
            AddBoard();
            LOG_DEBUG(3);
            break;
            
    }
}

void App::Reset() {
    if (m_Phase != Phase::LOBBY){
        Level_Coordinates = {};
        Level_Balloons = {};
        if (!m_Counters.empty()) {
            m_Root.RemoveChild(m_Counters[0]);
            m_Root.RemoveChild(m_Counters[1]);
            m_Root.RemoveChild(m_Counters[2]);
            m_Root.RemoveChild(m_Counters[0]->GetCounterText());
            m_Root.RemoveChild(m_Counters[1]->GetCounterText());
            m_Root.RemoveChild(m_Counters[2]->GetCounterText());
            m_Counters = {};
        }
        if (!m_Monkeys.empty()) {
            for (auto& monkeyPtr : m_Monkeys) {
                m_Root.RemoveChild(monkeyPtr);
                m_Root.RemoveChild(monkeyPtr -> GetRange());
                std::vector<std::shared_ptr<Util::GameObject>> InfortionBoardObject = monkeyPtr -> GetAllInfortionBoardObject();
                for (auto& objectPtr : InfortionBoardObject) {
                    m_Root.RemoveChild(objectPtr);
                }
            }
            m_Monkeys = {};
        }

        if (!m_Attacks.empty()) {
            for (auto& attackPtr : m_Attacks) {
                m_Root.RemoveChild(attackPtr);
            }
            m_Attacks = {};
        }
        if (!m_Balloons.empty()) {
            for (auto& balloonPtr : m_Balloons) {
                std::vector<std::shared_ptr<Util::GameObject>> debuffView = balloonPtr -> GetDebuffViews();
                for (auto& debuffPtr : debuffView) {
                    m_Root.RemoveChild(debuffPtr);
                }
                m_Root.RemoveChild(balloonPtr);
            }
            m_Balloons = {};
        }

        for (auto& objectPtr : m_DragButtons) {
            m_Root.RemoveChild(objectPtr);
        }

        std::vector<std::shared_ptr<Button>> BoardObject = Win_Board -> GetAllChildren();
        for (auto& objectPtr : BoardObject) {
            m_Root.RemoveChild(objectPtr);
        }
        m_Root.RemoveChild(Win_Board);
        Win_Board = nullptr;

        BoardObject = Lose_Board -> GetAllChildren();
        for (auto& objectPtr : BoardObject) {
            m_Root.RemoveChild(objectPtr);
        }
        m_Root.RemoveChild(Lose_Board);
        Lose_Board = nullptr;

        BoardObject = Suspend_Board -> GetAllChildren();
        for (auto& objectPtr : BoardObject) {
            m_Root.RemoveChild(objectPtr);
        }
        m_Root.RemoveChild(Suspend_Board);
        Suspend_Board = nullptr;

        m_Root.RemoveChild(Suspend_Button);
        Suspend_Button = nullptr;

        m_Root.RemoveChild(Accelerate_Button);
        Accelerate_Button = nullptr;
    }
    else {
        for (auto& objectPtr : Lobby_Buttons) {
            m_Root.RemoveChild(objectPtr);
        }
        Lobby_Buttons = {};

        if (Choose_Level_Board) {
            std::vector<std::shared_ptr<Button>> BoardObject = Choose_Level_Board -> GetAllChildren();
            for (auto& objectPtr : BoardObject) {
                m_Root.RemoveChild(objectPtr);
            }
            m_Root.RemoveChild(Choose_Level_Board);
            Choose_Level_Board = nullptr;
        }
    }
}

void App::SetLevel(int level) {
    if (level <= 10) {
        Level_Coordinates = m_PRM -> GetCoordinates(level-1);
        Level_Balloons = m_PRM -> GetBalloons(level-1);
        Level_Placeable = m_PRM -> GetPlaceable(level-1);
    }
    else {
        Level_Coordinates = m_PRM -> GetCoordinates(level%10-1);
        Level_Placeable = m_PRM -> GetPlaceable(level%10-1);
    }

    m_Counters.push_back(std::make_shared<Heart>(20, 20));
    m_Root.AddChild(m_Counters[0]);
    m_Root.AddChild(m_Counters[0]->GetCounterText());

    m_Counters.push_back(std::make_shared<Money>(1000,99999));
    m_Root.AddChild(m_Counters[1]);
    m_Root.AddChild(m_Counters[1]->GetCounterText());

    if (level > 10) {
        auto counter = std::make_shared<Round>(1,99999);
        counter -> SetTextPosition(glm::vec2(-300, -330));
        m_Counters.push_back(counter);
        m_Root.AddChild(m_Counters[2]);
        m_Root.AddChild(m_Counters[2]->GetCounterText());
    }
    else {
        m_Counters.push_back(std::make_shared<Round>(1,10));
        m_Root.AddChild(m_Counters[2]);
        m_Root.AddChild(m_Counters[2]->GetCounterText());
    }

    // 设置按钮位置的基础坐标和间隔
    float startX = 438.0f;
    float startY = 303.0f;
    float buttonYSpacing = 100.0f;  // 按钮之间的间隔
    float buttonXSpacing = 100.0f;  // 按钮之间的间隔
    
    // 创建飞镖猴子按钮
    auto m_DartMonkeyButton = std::make_shared<DartMonkeyButton>(glm::vec2(startX, startY));
    m_DragButtons.push_back(m_DartMonkeyButton);    
    m_Root.AddChild(m_DartMonkeyButton);
    
    // 创建钉子猴子按钮
    auto m_NailMonkeyButton = std::make_shared<NailMonkeyButton>(glm::vec2(startX, startY-buttonYSpacing));
    m_DragButtons.push_back(m_NailMonkeyButton);    
    m_Root.AddChild(m_NailMonkeyButton);
    
    // 创建狙击手猴子按钮
    auto m_SniperMonkeyButton = std::make_shared<SniperMonkeyButton>(glm::vec2(startX, startY-buttonYSpacing*2));
    m_DragButtons.push_back(m_SniperMonkeyButton);    
    m_Root.AddChild(m_SniperMonkeyButton);
    
    // 创建回旋镖猴子按钮
    auto m_BoomerangMonkeyButton = std::make_shared<BoomerangMonkeyButton>(glm::vec2(startX, startY-buttonYSpacing*3));
    m_DragButtons.push_back(m_BoomerangMonkeyButton);    
    m_Root.AddChild(m_BoomerangMonkeyButton);
    
    // 创建忍者猴子按钮
    auto m_NinjaMonkeyButton = std::make_shared<NinjaMonkeyButton>(glm::vec2(startX, startY-buttonYSpacing*4));
    m_DragButtons.push_back(m_NinjaMonkeyButton);    
    m_Root.AddChild(m_NinjaMonkeyButton);

    auto m_CannonButton = std::make_shared<CannonButton>(glm::vec2(startX, startY-buttonYSpacing*5));
    m_DragButtons.push_back(m_CannonButton);    
    m_Root.AddChild(m_CannonButton);

    auto m_AirportButton = std::make_shared<AirportButton>(glm::vec2(startX+buttonXSpacing, startY));
    m_DragButtons.push_back(m_AirportButton);
    m_Root.AddChild(m_AirportButton);

    auto m_BuccaneerMonkeyButton = std::make_shared<BuccaneerMonkeyButton>(glm::vec2(startX+buttonXSpacing, startY-buttonYSpacing));
    m_DragButtons.push_back(m_BuccaneerMonkeyButton);
    m_Root.AddChild(m_BuccaneerMonkeyButton);

    auto m_SuperMonkeyButton = std::make_shared<SuperMonkeyButton>(glm::vec2(startX+buttonXSpacing, startY-buttonYSpacing*2));
    m_DragButtons.push_back(m_SuperMonkeyButton);
    m_Root.AddChild(m_SuperMonkeyButton);

    auto m_IceMonkeyButton = std::make_shared<IceMonkeyButton>(glm::vec2(startX+buttonXSpacing, startY-buttonYSpacing*3));
    m_DragButtons.push_back(m_IceMonkeyButton);
    m_Root.AddChild(m_IceMonkeyButton);

    auto m_RubberMonkeyButton = std::make_shared<RubberMonkeyButton>(glm::vec2(startX+buttonXSpacing, startY-buttonYSpacing*4));
    m_DragButtons.push_back(m_RubberMonkeyButton);
    m_Root.AddChild(m_RubberMonkeyButton);

    auto m_MagicMonkeyButton = std::make_shared<MagicMonkeyButton>(glm::vec2(startX+buttonXSpacing, startY-buttonYSpacing*5));
    m_DragButtons.push_back(m_MagicMonkeyButton);
    m_Root.AddChild(m_MagicMonkeyButton);

    auto button = std::make_shared<Button>(GA_RESOURCE_DIR"/Board/suspend.png", glm::vec2(startX, startY-buttonYSpacing*6), glm::vec2(1, 1), 60, 60);
    button -> SetVisible(true);
    Suspend_Button = button;
    m_Root.AddChild(button);

    button = std::make_shared<Button>(GA_RESOURCE_DIR"/Board/accelerate.png", glm::vec2(startX+buttonXSpacing, startY-buttonYSpacing*6), glm::vec2(1, 1), 60, 60);
    button -> SetVisible(true);
    Accelerate_Button = button;
    m_Root.AddChild(button);
}

void App::AddBoard() {
    if (m_Phase != Phase::LOBBY) {
        auto win_board = std::make_shared<Board>(GA_RESOURCE_DIR"/Board/board.png", glm::vec2(-100, 0), glm::vec2(2, 2));
        Win_Board = win_board;
        m_Root.AddChild(win_board);

        auto button = std::make_shared<Button>(GA_RESOURCE_DIR"/Board/next_level.png", glm::vec2(-100, 75), glm::vec2(1, 1), 400, 100);
        Win_Board -> AddButton(button);
        m_Root.AddChild(button);

        button = std::make_shared<Button>(GA_RESOURCE_DIR"/Board/back.png", glm::vec2(-100, -75),glm::vec2(1, 1),  400, 100);
        Win_Board -> AddButton(button);
        m_Root.AddChild(button);

        auto lose_board = std::make_shared<Board>(GA_RESOURCE_DIR"/Board/board.png", glm::vec2(-100, 0), glm::vec2(2, 2));
        Lose_Board = lose_board;
        m_Root.AddChild(lose_board);

        button = std::make_shared<Button>(GA_RESOURCE_DIR"/Board/restart.png", glm::vec2(-100, 75), glm::vec2(1, 1), 400, 100);
        Lose_Board -> AddButton(button);
        m_Root.AddChild(button);

        button = std::make_shared<Button>(GA_RESOURCE_DIR"/Board/back.png", glm::vec2(-100, -75), glm::vec2(1, 1), 400, 100);
        Lose_Board -> AddButton(button);
        m_Root.AddChild(button);

        auto suspend_board = std::make_shared<Board>(GA_RESOURCE_DIR"/Board/board.png", glm::vec2(-100, 0), glm::vec2(2, 2));
        Suspend_Board = suspend_board;
        m_Root.AddChild(suspend_board);

        button = std::make_shared<Button>(GA_RESOURCE_DIR"/Board/continue.png", glm::vec2(-100, 75), glm::vec2(1, 1), 400, 100);
        Suspend_Board -> AddButton(button);
        m_Root.AddChild(button);

        button = std::make_shared<Button>(GA_RESOURCE_DIR"/Board/back.png", glm::vec2(-100, -75), glm::vec2(1, 1), 400, 100);
        Suspend_Board -> AddButton(button);
        m_Root.AddChild(button);
    }
    else {
        auto button = std::make_shared<Button>(GA_RESOURCE_DIR"/Board/normal_mode.png", glm::vec2(-280, -25), glm::vec2(0.8, 0.8), 480, 160);
        button -> SetVisible(true);
        button -> SetMaxZ(6);
        Lobby_Buttons.push_back(button);
        m_Root.AddChild(button);

        button = std::make_shared<Button>(GA_RESOURCE_DIR"/Board/unlimit_mode.png", glm::vec2(-280, -230), glm::vec2(0.8, 0.8), 480, 160);
        button -> SetVisible(true);
        button -> SetMaxZ(6);
        Lobby_Buttons.push_back(button);
        m_Root.AddChild(button);

        auto chooseLeevelBoard = std::make_shared<Board>(GA_RESOURCE_DIR"/Board/board.png", glm::vec2(0, 0), glm::vec2(3.5, 3.5));
        Choose_Level_Board = chooseLeevelBoard;
        m_Root.AddChild(chooseLeevelBoard);


        int x = -408;
        int y = 175;
        for (int i=0; i < IsLevelUnlock.size(); i++) {
            if (IsLevelUnlock[i]) {
                button = std::make_shared<Button>(GA_RESOURCE_DIR"/Map/" + m_PRM -> GetImagePaths(i+1) + ".png", glm::vec2(x, y), glm::vec2(0.1813, 0.1813), 184, 140);
            }
            else {
                button = std::make_shared<Button>(GA_RESOURCE_DIR"/Map/" + m_PRM -> GetImagePaths(i+1) + "_invalid.png", glm::vec2(x, y), glm::vec2(0.1813, 0.1813), 184, 140);
            }
            Choose_Level_Board -> AddButton(button);
            m_Root.AddChild(button);
            x += 204;
            if (i == 4) {
                x = -408;
                y = -175;
            }
        }
    }
}
