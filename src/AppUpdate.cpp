#include "App.hpp"
#include <iostream>
#include <algorithm>
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <random>
#include <cxxabi.h>
#include <ctime>

int count = 20;
bool blocked = false;
int block_time = 0;
int nuclear_bomb_time = 0;
int balloons_round = 0;
std::vector<std::pair<std::shared_ptr<Balloon>, std::shared_ptr<Rope_tail>>> grabbedBalloons;
std::vector<std::shared_ptr<Balloon>> icetogethers;
std::vector<std::shared_ptr<Balloon>> icebursts;
std::vector<std::shared_ptr<Balloon>> new_balloons;
std::vector<std::shared_ptr<Balloon>> remove_balloons;
std::vector<std::shared_ptr<Attack>> remove_attacks;
std::vector<std::shared_ptr<Attack>> m_drops;
std::shared_ptr<Monkey> m_testMonkey;

int random_number(int n) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, n - 1);
    return dis(gen);
}

std::shared_ptr<Balloon> factory(int num, std::vector<std::vector<glm::vec2>> coordinates) {
    int way = random_number(coordinates.size());
    switch (num) {
        case 0:
            return std::make_shared<RED>(coordinates[way]);
        case 1:
            return std::make_shared<BLUE>(coordinates[way]);
        case 2:
            return std::make_shared<GREEN>(coordinates[way]);
        case 3:
            return std::make_shared<YELLOW>(coordinates[way]);
        case 4:
            return std::make_shared<PINK>(coordinates[way]);
        case 5:
            return std::make_shared<BLACK>(coordinates[way]);
        case 6:
            return std::make_shared<WHITE>(coordinates[way]);
        case 7:
            return std::make_shared<PURPLE>(coordinates[way]);
        case 8:
            return std::make_shared<ZEBRA>(coordinates[way]);
        case 9:
            return std::make_shared<IRON>(coordinates[way]);
        case 10:
            return std::make_shared<RAINBOW>(coordinates[way]);
        case 11:
            return std::make_shared<CERAMICS>(coordinates[way]);
        case 12:
            return std::make_shared<MOAB>(coordinates[way]);
        case 13:
            return std::make_shared<BFB>(coordinates[way]);
        case 14:
            return std::make_shared<ZOMG>(coordinates[way]);
        case 15:
            return std::make_shared<DDT>(coordinates[way]);
        case 16:
            return std::make_shared<BAD>(coordinates[way]);
        default:
            return std::make_shared<RED>(coordinates[way]);
    }
}

int current_room(App::Phase phase) {
    switch (phase) {
        case App::Phase::LOBBY:
            return 0;
        case App::Phase::FIRST_LEVEL:
            return 1;
        case App::Phase::SECOND_LEVEL:
            return 2;
        case App::Phase::THIRD_LEVEL:
            return 3;
        case App::Phase::FOURTH_LEVEL:
            return 4;
        case App::Phase::FIFTH_LEVEL:
            return 5;
        case App::Phase::SIXTH_LEVEL:
            return 6;
        case App::Phase::SEVENTH_LEVEL:
            return 7;
        case App::Phase::EIGHTH_LEVEL:
            return 8;
        case App::Phase::NINTH_LEVEL:
            return 9;
        case App::Phase::TENTH_LEVEL:
            return 10;
        default:
            return 11;
    }
}

void App::Update() {
    LOG_TRACE("Update");

    const glm::vec2 mousePosition = Util::Input::GetCursorPosition();

    // ==================== 大廳界面處理 ====================
    if (m_Phase == Phase::LOBBY) {
        // ==================== 大廳作弊鍵區塊 ====================
        if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
            for (int i = 0; i < IsLevelUnlock.size(); i++) {
                IsLevelUnlock[i] = true;
            }
            ValidTask(0);
        }
        
        if (!Choose_Level_Board -> GetVisible()) {
            Lobby_Buttons[0] -> IsTouch(mousePosition);
            Lobby_Buttons[1] -> IsTouch(mousePosition);
            if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
                if (Lobby_Buttons[0] -> IsClicked(mousePosition)) {
                    Choose_Level_Board -> UpdateVisible(true);
                    mode = 0;
                }
                if (Lobby_Buttons[1] -> IsClicked(mousePosition)) {
                    Choose_Level_Board -> UpdateVisible(true);
                    mode = 10;
                }
            }
        }
        else {
            Choose_Level_Board -> IsTouch(mousePosition);
            if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
                int isClick = Choose_Level_Board -> IsClicked(mousePosition);
                if (isClick >= 0 && IsLevelUnlock[isClick]) {
                    level = isClick+1;
                    ValidTask(isClick+1+mode);
                }
            }
        }
        if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
            m_CurrentState = State::END;
        }
    }
    // ==================== 遊戲主要邏輯 ====================
    else if(!Win_Board -> GetVisible() && !Lose_Board -> GetVisible() && !Suspend_Board -> GetVisible()) {
        
        // ==================== 作弊鍵區塊 ====================
        if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
            m_Counters[1]->AddValue(90000);
        }

        // ==================== UI 控制按鈕處理 ====================
        Suspend_Button -> IsTouch(mousePosition);
        Accelerate_Button -> IsTouch(mousePosition);
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)){
            if (Suspend_Button -> IsClicked(mousePosition)) {
                Suspend_Board -> UpdateVisible(true);
            }
            else if (Accelerate_Button -> IsClicked(mousePosition)) {
                if (GetFPS() == 180) {
                    SetFPS(60);
                }
                else {
                    SetFPS(180);
                }
            }
        }

        // ==================== 猴子特殊能力處理 ====================
        for (auto& monkeyPtr : m_Monkeys) {
            int status;
            std::string monkeyType = abi::__cxa_demangle(typeid(*monkeyPtr).name(), 0, 0, &status);
            if (monkeyType == "IceMonkey") {
                if (monkeyPtr->GetLevel() >= 4 && monkeyPtr->GetUpgradePath() == 1) {
                    for (auto& balloonPtr : m_Balloons) {
                        if (monkeyPtr->IsCollision(balloonPtr) && !std::any_of(icetogethers.begin(), icetogethers.end(),
                            [balloonPtr](const std::shared_ptr<Balloon>& ptr) {
                            return ptr == balloonPtr;
                        }))
                        {
                            auto attack1 = std::make_shared<Icetogether>(balloonPtr);
                            m_Attacks.push_back(attack1);
                            m_Root.AddChild(attack1);
                            icetogethers.push_back(balloonPtr);
                        }
                    }
                }
                else if (monkeyPtr->GetLevel() >= 3 && monkeyPtr->GetUpgradePath() == 2 ) {
                    for (auto& balloonPtr : m_Balloons) {
                        if (monkeyPtr->IsCollision(balloonPtr) && !std::any_of(icebursts.begin(), icebursts.end(),
                            [balloonPtr](const std::shared_ptr<Balloon>& ptr) {
                            return ptr == balloonPtr;
                        }))
                        {
                            int num_fragments = rand() % 3 + 1;
                            float angle_step = 360.0f / num_fragments;

                            for (int i = 0; i < num_fragments; i++) {
                                float current_angle = i * angle_step;
                                auto attack1 = std::make_shared<Iceburstsliced>(
                                    balloonPtr
                                );
                                attack1 -> SetAngle(current_angle);
                                attack1 -> SetScale(glm::vec2(2, 2));
                                attack1 -> SetTouchScale(glm::vec2(2, 2));
                                attack1 -> GetAttributes() -> SetPenetration(1);
                                m_Attacks.push_back(attack1);
                                m_Root.AddChild(attack1);
                            }
                            icebursts.push_back(balloonPtr);
                        }
                    }
                }
            }
        }

        // ==================== 掉落物品處理 ====================
        remove_balloons = {};
        std::vector<std::shared_ptr<Attack>> remove_drops;
        for (auto& dropboxPtr : m_drops) {
            if (dropboxPtr -> IsOut()) {
                m_Counters[1] -> AddValue(20000);
                remove_drops.push_back(dropboxPtr);
            }
        }
        for (auto& dropboxPtr : remove_drops) {
            m_drops.erase(std::remove(m_drops.begin(), m_drops.end(), dropboxPtr), m_drops.end());
            m_Root.RemoveChild(dropboxPtr);
        }

        // ==================== 技能冷卻時間處理 ====================
        if (blocked) {
            block_time -= 1;
            if (block_time == 0) {
                blocked = false;
            }
        }
        if (nuclear_bomb_time>0){
            nuclear_bomb_time -= 1;
            if (nuclear_bomb_time == 0){
                for (auto& balloonPtr : m_Balloons) {
                    balloonPtr -> LoseHealth(350);
                }
            }
        }

        // ==================== 被抓取氣球處理 ====================
        if (!grabbedBalloons.empty()) {
            std::vector<size_t> finishedIndices;

            for (size_t i = 0; i < grabbedBalloons.size(); ++i) {
                auto rope = std::make_shared<Rope>(grabbedBalloons[i].second->GetSourcePosition(), grabbedBalloons[i].second->GetPosition(), std::make_shared<Attributes>());
                m_Attacks.push_back(rope);
                m_Root.AddChild(rope);
                if (grabbedBalloons[i].second -> CheckAndReverse()){
                    grabbedBalloons[i].first -> Move();
                }
                if (grabbedBalloons[i].second -> IsOut()){
                    finishedIndices.push_back(i);
                    auto boom = std::make_shared<Explosive_cannon>(grabbedBalloons[i].second->GetPosition());
                    boom -> SetScale(glm::vec2(0.3, 0.3));
                    m_Attacks.push_back(boom);
                    m_Root.AddChild(boom);
                }
            }

            std::sort(finishedIndices.begin(), finishedIndices.end(), std::greater<size_t>());
            for (auto idx : finishedIndices) {
                if (idx < grabbedBalloons.size()) {
                    for (auto& debuffPtr : grabbedBalloons[idx].first -> GetDebuffViews()) {
                        m_Root.RemoveChild(debuffPtr);
                    }
                    m_Root.RemoveChild(grabbedBalloons[idx].first);
                    grabbedBalloons.erase(grabbedBalloons.begin() + idx);
                }
            }
        }

        // ==================== 猴子拖拽邏輯 ====================
        for (auto& dragButtonPtr : m_DragButtons) {
            dragButtonPtr->Update();
            dragButtonPtr->UpdateButtonState(m_Counters[1]->GetCurrent());
        }

        if (m_DragMonkey) {
            if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
                m_DragMonkey->SetPosition(mousePosition);
                m_DragMonkey->UpdateRange();
                m_DragMonkey->SetRangeColor(true);
                if (m_DragMonkey->Placeable(Level_Placeable)) {
                    for (auto& monkeyPtr : m_Monkeys) {
                        if (monkeyPtr->Touched(*m_DragMonkey)) {
                            m_DragMonkey->SetRangeColor(false);
                            break;
                        }
                    }
                }else{
                    m_DragMonkey->SetRangeColor(false);
                }
            } else {
                bool allMonkeysAllowPlacement = true;

                if (m_DragMonkey->Placeable(Level_Placeable)) {
                    for (auto& monkeyPtr : m_Monkeys) {
                        if (monkeyPtr->Touched(*m_DragMonkey)) {
                            allMonkeysAllowPlacement = false;
                            break;
                        }
                    }
                }else{
                    allMonkeysAllowPlacement = false;
                }

                if (allMonkeysAllowPlacement) {
                    m_Monkeys.push_back(m_DragMonkey);
                    m_DragMonkey->SetPosition(mousePosition);
                    m_DragMonkey->UpdateRange();
                    m_DragMonkey->SetRangeColor(true);
                    m_Counters[1] -> MinusValue(m_DragMonkey -> GetCost());
                }else{
                    m_Root.RemoveChild(m_DragMonkey);
                    m_Root.RemoveChild(m_DragMonkey->GetRange());
                    std::vector<std::shared_ptr<Util::GameObject>> InfortionBoardObject = m_DragMonkey-> GetAllInfortionBoardObject();
                    for (auto& objectPtr : InfortionBoardObject) {
                        m_Root.RemoveChild(objectPtr);
                    }
                }

                m_DragMonkey = nullptr;
            }
        } else {
            for (auto& dragButtonPtr : m_DragButtons) {
                if (dragButtonPtr->IsPointInside(mousePosition) && Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
                    m_DragMonkey = dragButtonPtr->ProduceMonkey(mousePosition);

                    if (m_DragMonkey) {
                        m_Root.AddChild(m_DragMonkey);
                        m_Root.AddChild(m_DragMonkey-> GetRange());
                        m_Root.AddChildren(m_DragMonkey-> GetAllInfortionBoardObject());
                        break;
                    }
                }
            }
        }

        // ==================== 氣球生成邏輯 ====================
        if (m_Phase == Phase::InfiniteMode && count == 0) {
            int round = m_Counters[2] -> GetCurrent()-1;
            if (m_Balloons.empty() && balloons_round == (round+1)*2) {
                m_Counters[2] -> AddValue(1);
                balloons_round = 0;
            }
            int balloon_level = std::min(16, round/5);
            if (balloons_round < (round+1)*2) {
                int num = random_number(balloon_level+1);
                std::shared_ptr<Balloon> m_Balloon;
                m_Balloon = factory(num, Level_Coordinates);
                m_Balloons.push_back(m_Balloon);
                m_Root.AddChildren(m_Balloon -> GetDebuffViews());
                m_Root.AddChild(m_Balloon);
                balloons_round ++;
            }
        }
        else{
            if (m_Phase != Phase::LOBBY && count == 0) {
                int round = m_Counters[2] -> GetCurrent()-1;
                if (m_Balloons.empty() && Level_Balloons[round].empty()) {
                    m_Counters[2] -> AddValue(1);
                }
                std::shared_ptr<Balloon> m_Balloon;
                if (!Level_Balloons[round].empty() && !blocked) {
                    int num = Level_Balloons[round][0];
                    Level_Balloons[round].erase(Level_Balloons[round].begin());
                    m_Balloon = factory(num, Level_Coordinates);
                    m_Balloons.push_back(m_Balloon);
                    m_Root.AddChildren(m_Balloon -> GetDebuffViews());
                    m_Root.AddChild(m_Balloon);
                }
            }
        }

        if (count == 0) {
            count = 20;
        }
        count --;

        // ==================== 戰鬥邏輯：攻擊與碰撞檢測 ====================
        for (auto& balloonPtr : m_Balloons) {
            bool underAttack = false;
            for (auto& attackPtr : m_Attacks) {
                if (attackPtr -> IsAlive() && balloonPtr -> IsCollision(attackPtr)){
                    underAttack = true;
                    balloonPtr -> LoseHealth(balloonPtr -> IsAttackEffective(attackPtr -> GetProperties(), attackPtr -> GetPower()));
                    balloonPtr -> GetDebuff(attackPtr -> GetAttributes() -> GetDebuff());
                    attackPtr -> LosePenetration();
                    if (!attackPtr -> IsAlive()) {
                        remove_attacks.push_back(attackPtr);
                    }
                }
                if (!balloonPtr -> IsAlive()) {
                    break;
                }
            }
            for (auto& attackPtr : remove_attacks) {
                m_Attacks.erase(std::remove(m_Attacks.begin(), m_Attacks.end(), attackPtr), m_Attacks.end());
                m_Root.RemoveChild(attackPtr);
            }
            
            // 氣球死亡處理
            if (!balloonPtr -> IsAlive()) {
                std::vector<std::shared_ptr<Balloon>> bs = balloonPtr -> Burst();
                new_balloons.insert(new_balloons.end(), bs.begin(), bs.end());
                if (balloonPtr -> ShowDebuff(10) > 0 && ( balloonPtr -> ShowDebuff(0) >0 || balloonPtr -> ShowDebuff(1) > 0)) {
                    for (auto& b : bs) {
                        b -> GetDebuff({{0,100}});
                    }
                }
                if (balloonPtr -> ShowDebuff(11) > 0) {
                    for (auto& b : bs) {
                        b -> GetDebuff({{2,100}});
                        b -> GetDebuff({{11, 100}});
                        b -> GetDebuff({{12, balloonPtr -> ShowDebuff(12)}});
                        b -> GetDebuff({{13, balloonPtr -> ShowDebuff(13)}});
                    }
                }
                remove_balloons.push_back(balloonPtr);
                m_Counters[1] -> AddValue(balloonPtr -> GetMoney());
            }
            else if (underAttack) {
                balloonPtr -> Injured();
            }
        }

        // ==================== 氣球移動與到達終點處理 ====================
        for (auto& balloonPtr : remove_balloons) {
            std::vector<std::shared_ptr<Util::GameObject>> debuffView = balloonPtr -> GetDebuffViews();
            m_Balloons.erase(std::remove(m_Balloons.begin(), m_Balloons.end(), balloonPtr), m_Balloons.end());
            for (auto& debuffPtr : debuffView) {
                m_Root.RemoveChild(debuffPtr);
            }
            m_Root.RemoveChild(balloonPtr);
        }

        for (auto& balloonPtr : m_Balloons) {
            balloonPtr -> Move();
            if (balloonPtr -> IsArrive()) {
                remove_balloons.push_back(balloonPtr);
                m_Counters[0] ->MinusValue(1);
            }
        }

        // ==================== 猴子被動技能效果 ====================
        for (auto& monkeyPtr : m_Monkeys) {
            int status;
            std::string monkeyType = abi::__cxa_demangle(typeid(*monkeyPtr).name(), 0, 0, &status);
            if (monkeyType == "NailMonkey" && monkeyPtr -> GetSkillCountdown() > 0) {
                std::vector<std::shared_ptr<Attack>> attacks = monkeyPtr -> ProduceAttack(glm::vec2(100000, 100000));
                for (auto& attackPtr : attacks) {
                    m_Attacks.push_back(attackPtr);
                    m_Root.AddChild(attackPtr);
                }
            }
            else if (monkeyType == "SuperMonkey" && monkeyPtr -> GetSkillCountdown() > 0 ) {
                if (!m_Balloons.empty()) {
                    std::shared_ptr<Balloon> balloonPtr = m_Balloons[0];
                    if (balloonPtr) {
                        if (balloonPtr->GetType() == Balloon::Type::spaceship) {
                            balloonPtr->LoseHealth(200);
                        } else {
                            remove_balloons.push_back(balloonPtr);
                        }
                        auto explosive_cannon = std::make_shared<Explosive_cannon>(balloonPtr->GetPosition());
                        explosive_cannon -> SetScale(glm::vec2(0.3, 0.3));
                        m_Attacks.push_back(explosive_cannon);
                        m_Root.AddChild(explosive_cannon);
                    }
                }
            }
        }

        // 清理被移除的氣球
        for (auto& balloonPtr : remove_balloons) {
            std::vector<std::shared_ptr<Util::GameObject>> debuffView = balloonPtr -> GetDebuffViews();
            m_Balloons.erase(std::remove(m_Balloons.begin(), m_Balloons.end(), balloonPtr), m_Balloons.end());
            for (auto& debuffPtr : debuffView) {
                m_Root.RemoveChild(debuffPtr);
            }
            m_Root.RemoveChild(balloonPtr);
        }

        // 添加新生成的氣球
        for (auto& balloonPtr : new_balloons) {
            m_Balloons.push_back(balloonPtr);
            m_Root.AddChildren(balloonPtr -> GetDebuffViews());
            m_Root.AddChild(balloonPtr);
        }

        remove_balloons = {};
        new_balloons = {};

        // 氣球排序（按距離）
        std::sort(m_Balloons.begin(), m_Balloons.end(),
            [](const std::shared_ptr<Balloon>& a, const std::shared_ptr<Balloon>& b) {
                return a->GetDistance() > b->GetDistance();
            });

        remove_attacks = {};

        // ==================== 猴子信息面板處理 ====================
        if (m_ClickedMonkey) {
            m_ClickedMonkey -> IsButtonTouch(mousePosition);
        }

        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            int clickInformationBoard = 0;
            if (m_ClickedMonkey) {
                clickInformationBoard = m_ClickedMonkey -> IsInformationBoardClicked(mousePosition, m_Counters[1] -> GetCurrent());
                
                // 關閉信息面板
                if (clickInformationBoard == 2) {
                    m_ClickedMonkey -> UpdateAllObjectVisible(false);
                    m_ClickedMonkey = nullptr;
                }
                // 賣掉猴子
                else if (clickInformationBoard == 3) {
                    int status;
                    std::string monkeyType = abi::__cxa_demangle(typeid(*m_ClickedMonkey).name(), 0, 0, &status);
                    if (monkeyType == "SuperMonkey") {
                        auto absorbedMonkeys = m_ClickedMonkey->gettogethermonkey();
                        
                        for (auto& absorbedMonkey : absorbedMonkeys) {
                            m_Monkeys.erase(std::remove(m_Monkeys.begin(), m_Monkeys.end(), absorbedMonkey), m_Monkeys.end());
                            m_Root.RemoveChild(absorbedMonkey);
                            m_Root.RemoveChild(absorbedMonkey->GetRange());
                            std::vector<std::shared_ptr<Util::GameObject>> InfortionBoardObject = absorbedMonkey-> GetAllInfortionBoardObject();
                            for (auto& objectPtr : InfortionBoardObject) {
                                m_Root.RemoveChild(objectPtr);
                            }
                            std::vector<std::shared_ptr<Attack>> attacks = absorbedMonkey-> GetAttackChildren();
                            for (auto& attacktPtr : attacks) {
                                m_Attacks.erase(std::remove(m_Attacks.begin(), m_Attacks.end(), attacktPtr), m_Attacks.end());
                                m_Root.RemoveChild(attacktPtr);
                            }
                        }
                    }
                    
                    m_Monkeys.erase(std::remove(m_Monkeys.begin(), m_Monkeys.end(), m_ClickedMonkey), m_Monkeys.end());
                    m_Root.RemoveChild(m_ClickedMonkey);
                    m_Root.RemoveChild(m_ClickedMonkey->GetRange());
                    std::vector<std::shared_ptr<Util::GameObject>> InfortionBoardObject = m_ClickedMonkey-> GetAllInfortionBoardObject();
                    for (auto& objectPtr : InfortionBoardObject) {
                        m_Root.RemoveChild(objectPtr);
                    }
                    std::vector<std::shared_ptr<Attack>> attacks = m_ClickedMonkey-> GetAttackChildren();
                    for (auto& attacktPtr : attacks) {
                        m_Attacks.erase(std::remove(m_Attacks.begin(), m_Attacks.end(), attacktPtr), m_Attacks.end());
                        m_Root.RemoveChild(attacktPtr);
                    }
                    m_Counters[1] -> AddValue(m_ClickedMonkey -> GetValue());
                    m_ClickedMonkey = nullptr;
                }
                // 使用技能
                else if (clickInformationBoard == 4) {
                    int status;
                    std::string monkeyType = abi::__cxa_demangle(typeid(*m_ClickedMonkey).name(), 0, 0, &status);
                    if (monkeyType == "DartMonkey") {
                        for (auto& monkeyPtr : m_Monkeys) {
                            monkeyType = abi::__cxa_demangle(typeid(*monkeyPtr).name(), 0, 0, &status);
                            if (monkeyType == "DartMonkey") {
                                monkeyPtr -> UseSkill();
                            }
                        }
                    }
                    else if (monkeyType == "NailMonkey") {
                        m_ClickedMonkey -> UseSkill();
                    }
                    else if (monkeyType == "RubberMonkey") {
                        for (auto& BalloonPtr : m_Balloons) {
                            BalloonPtr -> GetDebuff(m_ClickedMonkey -> GetAttributes() -> GetDebuff());
                        }
                    }
                    else if (monkeyType == "IceMonkey") {
                        for (auto& BalloonPtr : m_Balloons) {
                            if (BalloonPtr -> GetType() != Balloon::Type::spaceship) {
                                BalloonPtr -> GetDebuff({{1, 240}});
                            }
                        }
                    }
                    else if (monkeyType == "Cannon") {
                        for (auto& BalloonPtr : m_Balloons) {
                            if (BalloonPtr -> GetType() == Balloon::Type::spaceship) {
                                BalloonPtr -> LoseHealth(1000);
                                auto explosive_cannon = std::make_shared<Explosive_cannon>(BalloonPtr -> GetPosition());
                                m_Attacks.push_back(explosive_cannon);
                                m_Root.AddChild(explosive_cannon);
                                break;
                            }
                        }
                    }
                    else if (monkeyType == "NinjaMonkey") {
                        blocked = true;
                        block_time = 600;
                        glm::vec2 position = Level_Coordinates[0][0];
                        for (auto& balloonPtr : m_Balloons) {
                            balloonPtr -> GetDebuff({{4, 600}});
                        }
                        auto rock_ninja = std::make_shared<RockNinja>(position);
                        m_Attacks.push_back(rock_ninja);
                        m_Root.AddChild(rock_ninja);
                    }
                    else if (monkeyType == "BoomerangMonkey") {
                        m_ClickedMonkey -> UseSkill();
                    }
                    else if (monkeyType == "SuperMonkey") {
                        m_ClickedMonkey -> UseSkill();
                    }
                    else if (monkeyType == "Airport") {
                        auto nuclear_bomb = std::make_shared<Nuclear_bomb>(glm::vec2(0.0, 0.0));
                        m_Attacks.push_back(nuclear_bomb);
                        m_Root.AddChild(nuclear_bomb);
                        nuclear_bomb_time = 138;
                    }
                    else if (monkeyType == "BuccaneerMonkey") {
                        for (size_t i = 0; i < m_Balloons.size(); i++) {
                            if (m_Balloons[i]->GetType() == Balloon::Type::spaceship && m_Balloons[i] -> GetHealth() > 0) {
                                auto balloonPtr = m_Balloons[i];
                                auto rope_tail = std::make_shared<Rope_tail>(m_ClickedMonkey->GetPosition(), balloonPtr->GetPosition(), m_ClickedMonkey->GetAttributes());
                                m_Attacks.push_back(rope_tail);
                                m_Root.AddChild(rope_tail);
                                balloonPtr->SetTargetPosition(m_ClickedMonkey->GetPosition());
                                balloonPtr->SetSpeed(20.0f);
                                grabbedBalloons.push_back(std::make_pair(balloonPtr, rope_tail));
                                m_Balloons.erase(m_Balloons.begin() + i);
                                break;
                            }
                        }
                    }
                    else if (monkeyType == "SniperMonkey") {
                        auto dropbox = std::make_shared<Dropbox>(glm::vec2(rand() % 950 - 600, rand() % 300));
                        dropbox -> SetScale(glm::vec2(0.1, 0.1));
                        m_drops.push_back(dropbox);
                        m_Root.AddChild(dropbox);
                    }
                    else if (monkeyType == "MagicMonkey") {
                        auto thebird = std::make_shared<TheBird>(m_ClickedMonkey->GetPosition(), glm::vec2(0.0, 0.0), m_ClickedMonkey->GetAttributes());
                        m_Attacks.push_back(thebird);
                        m_Root.AddChild(thebird);
                    }
                }
                // 升級猴子
                else if (clickInformationBoard != 0 && clickInformationBoard != 1) {
                    m_Counters[1] -> MinusValue(clickInformationBoard);
                    
                    int status;
                    std::string monkeyType = abi::__cxa_demangle(typeid(*m_ClickedMonkey).name(), 0, 0, &status);
                    if (monkeyType == "SuperMonkey" && m_ClickedMonkey->GetLevel() == 4 && m_ClickedMonkey->GetUpgradePath() == 1) {
                        glm::vec2 superMonkeyPos = m_ClickedMonkey->GetPosition();
                        float superMonkeyRange = m_ClickedMonkey->GetRadius();
                        int absorbedCount = 0;
                        for (auto& monkeyPtr : m_Monkeys) {
                            std::string monkeyType = abi::__cxa_demangle(typeid(*monkeyPtr).name(), 0, 0, &status);
                            if (monkeyType != "SuperMonkey" && monkeyPtr->GetTag() != "absorbed" && monkeyType != "BuccaneerMonkey") {
                                m_ClickedMonkey->addtogethermonkey(monkeyPtr);
                                superMonkeyRange = m_ClickedMonkey->GetRadius();
                                auto superMonkeyAttr = m_ClickedMonkey->GetAttributes();
                                int superMonkeyPower = superMonkeyAttr->GetPower();
                                glm::vec2 otherMonkeyPos = monkeyPtr->GetPosition();
                                float distance = sqrt(pow(superMonkeyPos.x - otherMonkeyPos.x, 2) + pow(superMonkeyPos.y - otherMonkeyPos.y, 2));
                                
                                if (distance <= superMonkeyRange) {
                                    monkeyPtr->SetPosition(superMonkeyPos);
                                    monkeyPtr->UpdateRange();
                                    
                                    monkeyPtr->SetVisible(false);
                                    monkeyPtr->UpdateAllObjectVisible(false);
                                    monkeyPtr->SetTag("absorbed");
                                    
                                    float monkeyRange = monkeyPtr->GetRadius();
                                    if (monkeyRange > superMonkeyRange) {
                                        m_ClickedMonkey->SetRadius(monkeyRange);
                                    }
                                    
                                    auto monkeyAttr = monkeyPtr->GetAttributes();
                                    int monkeyPower = monkeyAttr->GetPower();
                                    if (monkeyPower > superMonkeyPower) {
                                        superMonkeyAttr->SetPower(monkeyPower);
                                    }
                                    std::vector<std::shared_ptr<Attack>> RELATED_ATTACK = monkeyPtr->GetAttackChildren();
                                    for (auto& attackPtr : RELATED_ATTACK) {
                                        attackPtr->SetPosition(superMonkeyPos);
                                    }
                                    
                                    std::vector<int> monkeyProperties = monkeyPtr->GetProperties();
                                    std::vector<int> superProperties = m_ClickedMonkey->GetProperties();
                                   
                                    for (int prop : monkeyProperties) {
                                        if (std::find(superProperties.begin(), superProperties.end(), prop) == superProperties.end()) {
                                            superProperties.push_back(prop);
                                        }
                                    }
                                    
                                    m_ClickedMonkey->SetProperties(superProperties);
                                }
                            }
                        }
                        m_ClickedMonkey->UpdateRange();
                    }
                }
            }
            
            // 選擇新的猴子
            if (clickInformationBoard == 0) {
                m_ClickedMonkey = nullptr;
                for (auto& monkeyPtr : m_Monkeys) {
                    if (monkeyPtr->GetTag() == "absorbed") {
                        continue;
                    }
                    
                    if (monkeyPtr -> IsClicked(mousePosition)) {
                        m_ClickedMonkey = monkeyPtr;
                    }
                }
            }
        }

        // ==================== 猴子攻擊邏輯 ====================
        for (auto& monkeyPtr : m_Monkeys) {
            if (monkeyPtr -> Countdown()) {
                std::vector<int> properties = monkeyPtr -> GetProperties();
                std::sort(properties.begin(), properties.end());
                bool camouflage = std::binary_search(properties.begin(), properties.end(), 2);
                for (auto& balloonPtr : m_Balloons) {
                    if (balloonPtr -> GetProperty(1) == 2 && !camouflage) {
                        continue;
                    }

                    if (monkeyPtr -> IsCollision(balloonPtr)) {
                        std::vector<std::shared_ptr<Attack>> attacks = monkeyPtr -> ProduceAttack(balloonPtr->GetPosition());
                        for (auto& attackPtra : attacks) {
                            m_Attacks.push_back(attackPtra);
                            m_Root.AddChild(attackPtra);
                        }
                        break;
                    }
                }
            }
        }

        // ==================== 攻擊移動與清理 ====================
        for (auto& attackPtr : m_Attacks) {
            attackPtr -> Move();
            if (attackPtr -> IsOut()) {
                remove_attacks.push_back(attackPtr);
            }
        }

        for (auto& attackPtr : remove_attacks) {
            m_Attacks.erase(std::remove(m_Attacks.begin(), m_Attacks.end(), attackPtr), m_Attacks.end());
            m_Root.RemoveChild(attackPtr);
        }
        remove_attacks = {};

        // ==================== 遊戲退出處理 ====================
        if (Util::Input::IsKeyPressed(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
            m_CurrentState = State::END;
        }

        // ==================== 勝利/失敗條件檢查 ====================
        if (m_Counters[2] -> GetCurrent() == m_Counters[2] -> GetMaxValue()) {
            if (m_Balloons.empty() && Level_Balloons[m_Counters[2] -> GetMaxValue()-1].empty()) {
                Win_Board -> UpdateVisible(true);
                IsLevelUnlock[current_room(m_Phase)] = true;
            }
        }

        if (!Win_Board -> GetVisible() && m_Counters[0] -> GetCurrent() == 0) {
            Lose_Board -> UpdateVisible(true);
        }
    }
    // ==================== 遊戲結束界面處理 ====================
    else {
        if (Win_Board -> GetVisible()) {
            Win_Board -> IsTouch(mousePosition);
        }
        else if (Lose_Board -> GetVisible()) {
            Lose_Board -> IsTouch(mousePosition);
        }
        else if (Suspend_Board -> GetVisible()) {
            Suspend_Board -> IsTouch(mousePosition);
        }
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            bool isClick;
            if (Win_Board -> GetVisible()) {
                isClick = Win_Board -> IsClicked(mousePosition);
                if (isClick >= 0) {
                    switch (isClick) {
                        case 0:
                            if (current_room(m_Phase) != 10) {
                                ValidTask(current_room(m_Phase)+1);
                                break;
                            }
                            ValidTask(0);
                            break;
                        case 1:
                            ValidTask(0);
                            break;
                    }
                }
            }
            else if (Lose_Board -> GetVisible()) {
                isClick = Lose_Board -> IsClicked(mousePosition);
                if (isClick >= 0) {
                    switch (isClick) {
                        case 0:
                            if (current_room(m_Phase) <= 10) {
                                ValidTask(current_room(m_Phase)+mode);
                            }
                            else {
                                ValidTask(level + mode);
                            }
                            break;
                        case 1:
                            ValidTask(0);
                            break;
                    }
                }
            }
            else if (Suspend_Board -> GetVisible()) {
                isClick = Suspend_Board -> IsClicked(mousePosition);
                if (isClick >= 0) {
                    switch (isClick) {
                        case 0:
                            Suspend_Board -> UpdateVisible(false);
                        break;
                        case 1:
                            balloons_round = 0;
                            ValidTask(0);
                        break;
                    }
                }
            }
        }
    }

    m_EnterDown = Util::Input::IsKeyPressed(Util::Keycode::RETURN);
    m_Root.Update();
}
