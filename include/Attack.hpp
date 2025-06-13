#pragma once
#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <vector>
#include <string>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "Attributes.hpp"
#include "Balloon.hpp"

class Attack : public Util::GameObject {
public:
    explicit Attack();
    explicit Attack(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    explicit Attack(glm::vec2 position);
    virtual void SetPosition(const glm::vec2& Position);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetImage(const std::string& ImagePath);
    void SetPenetration(int penetration);
    void SetSpeed(float speed);
    void SetPower(int power);
    void SetUnitDirection(const glm::vec2& goalPosition);
    void SetRotation(const glm::vec2& goalPosition);
    void SetCorners(const std::vector<glm::vec2>& corners);
    void RenewUnitDirection(glm::vec2 unitDirection);
    virtual void Move();
    void LosePenetration();
    void SetRectangleCorners();
    void SetScale(glm::vec2 scale);
    void SetTouchScale(glm::vec2 scale);


    [[nodiscard]] glm::vec2 GetPosition() const { return m_Transform.translation; }
    [[nodiscard]] virtual bool IsAlive() { return m_Attributes -> GetPenetration() > 0;}
    [[nodiscard]] float GetRadian() const { return m_Radian; }
    [[nodiscard]] float GetSpeed() const { return m_Attributes -> GetSpeed(); }
    [[nodiscard]] glm::vec2 GetUnitDirection() const { return m_UnitDirection; }
    [[nodiscard]] int GetPenetration() const { return m_Attributes -> GetPenetration(); }
    [[nodiscard]] int GetPower() const { return m_Attributes -> GetPower(); }
    [[nodiscard]] std::vector<glm::vec2> GetConers() const { return m_Corners; }
    [[nodiscard]] virtual bool IsOut();
    [[nodiscard]] std::shared_ptr<Attributes> GetAttributes() { return m_Attributes; }
    [[nodiscard]] std::vector<int> GetProperties() { return m_Attributes -> GetProperties(); }

private:
    int m_Width;
    int m_Height;
    float m_Radian;
    std::shared_ptr<Attributes> m_Attributes = std::make_shared<Attributes>();
    glm::vec2 m_UnitDirection;
    std::string m_ImagePath;
    std::vector<glm::vec2> m_Corners = {glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0)};
};

#endif //ATTACK_HPP

class Dart : public Attack {
public:
    explicit Dart(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};

class Nail : public Attack {
public:
    explicit Nail(glm::vec2 position, glm::vec2 goal_position, int radius, std::shared_ptr<Attributes> attributes);
    void Move() override;
private:
    int m_Radius;
    glm::vec2 m_SourcePosition;
};
class Superlight : public Attack {
public:
    explicit Superlight(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};
class Boomerang : public Attack {
public:
    explicit Boomerang(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
    void RotationUnitDirection();
    void RotationImage();
private:
    int isTurn = 0;
    int turn = 5;
    int rotation = 0;
    bool WillNotDisappear = true;
    glm::vec2 m_GoalPosition;
    glm::vec2 m_SourcePosition;
};

class Shuriken : public Attack {
public:
    explicit Shuriken(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    void RotationImahe();
private:
    int rotation = 0;
};

class Bomb : public Attack {
public:
    explicit Bomb(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};

class Explosion : public Attack {
public:
    explicit Explosion(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attack> bomb, std::shared_ptr<Attributes> attributes);
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    int existTime = 2;
    std::shared_ptr<Attack> m_Bomb;
};

class Icetogether : public Attack {
public:
    explicit Icetogether(std::shared_ptr<Balloon> balloon);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    std::shared_ptr<Balloon> m_Balloon;
    int existTime = 0;
};

class Iceburstsliced : public Attack {
public:
    explicit Iceburstsliced(std::shared_ptr<Balloon> balloon);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
    void SetAngle(float angle);
private:
    std::shared_ptr<Balloon> m_Balloon;
    float m_angle;
    int existTime = 0;
};

class Explosion_slice : public Attack {
public:
    explicit Explosion_slice(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attack> bomb, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    std::shared_ptr<Attack> m_Bomb;
    int existTime = 3;
};

class Explosionlight : public Attack {
public:
    explicit Explosionlight(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attack> bomb, std::shared_ptr<Attributes> attributes);
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    int existTime = 3;
    std::shared_ptr<Attack> m_Bomb;
};
class Airplane : public Attack {
public:
    explicit Airplane(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
    void SetPosition(const glm::vec2& Position) override;
    void RotationImage(glm::vec2 position);
private:
    int time = 0;
    int max_Penetration;
    bool WillNotDisappear = true;
    glm::vec2 m_SourcePosition;
};

class Ray : public Attack {
public:
    explicit Ray(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};

class Blizzard : public Attack {
public:
    explicit Blizzard(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes, int radius);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    int exit = 1;
};

class Rubber : public Attack {
public:
    explicit Rubber (glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};

class MagicBall : public Attack {
public:
    explicit MagicBall (glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};

class Rock : public Attack {
public:
    explicit Rock (glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
};

class Fire : public Attack {
public:
    explicit Fire (glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes, int radius);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    int exit = 20;
    float m_scale;
};

class Knife : public Attack {
public:
    explicit Knife (glm::vec2 position, glm::vec2 goal_position, int radius, std::shared_ptr<Attributes> attributes);
    void Move() override;
private:
    int m_Radius;
    glm::vec2 m_SourcePosition;
};

class BladedDisc : public Attack {
public:
    explicit BladedDisc(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
    void RotationUnitDirection();
    void RotationImage();
private:
    int isTurn = 0;
    int turn = 5;
    int rotation = 0;
    bool WillNotDisappear = true;
    glm::vec2 m_GoalPosition;
    glm::vec2 m_SourcePosition;
};

class BladedDisc_Rebound : public Attack {
public:
    explicit BladedDisc_Rebound(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    void RotationUnitDirection();
    void RotationImage();
private:
    int rotation = 0;
    glm::vec2 m_SourcePosition;
};

class BladedDisc_Around : public Attack {
public:
    explicit BladedDisc_Around(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
    void RotationImage();
private:
    int rotation = 0;
    int renewPenetrationCd;
    int max_Penetration = 0;
    bool WillNotDisappear = true;
    glm::vec2 m_SourcePosition;
};


//###########################################################


class Explosive_cannon : public Attack {
public:
    explicit Explosive_cannon(glm::vec2 position);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
    void RotationImage();
private:
    int time = 3;
};

class RockNinja : public Attack {
public:
    explicit RockNinja(glm::vec2 position);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    int time = 600;
};

class Nuclear_bomb : public Attack {
public:
    explicit Nuclear_bomb(glm::vec2 position);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
    void RotationImage();
private:
    int time = 138;
};

class NinjaShuriken : public Attack {
public:
    explicit NinjaShuriken(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
    void RotationImage();
    void CalculateNewDirection();
private:
    enum class State {
        MOVING_TO_TARGET,    // 移動到目標點
        MOVING_FORWARD,      // 向前移動 50 單位
        RANDOM_MOVEMENT      // 隨機八方向移動
    };
    
    State m_CurrentState = State::MOVING_TO_TARGET;
    int rotation = 0;
    int m_RandomMovementTime = 600;  // 將隨機移動的持續時間從300增加到600
    float m_CurrentAngle = 0.0f;     // 當前運動角度
    int m_DirectionChangeCount = 0;  // 方向變更計數
    
    // 八字形運動參數
    float m_CurrentTime = 0.0f;      // 參數化時間
    float m_OrbitRadius = 50.0f;     // 軌道半徑
    float m_RotationAngle = 0.0f;    // 整體旋轉角度
    
    glm::vec2 m_SourcePosition;      // 起始位置
    glm::vec2 m_GoalPosition;        // 目標位置
    glm::vec2 m_ExtendedPosition;    // 延伸位置
    bool WillNotDisappear = true;
};

class Rope : public Attack {
public:
    explicit Rope(glm::vec2 sourcePosition, glm::vec2 targetPosition, std::shared_ptr<Attributes> attributes);
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    glm::vec2 m_SourcePosition;
    glm::vec2 m_TargetPosition;
    glm::vec2 m_GoalPosition;
    int time = 1;
};

class Rope_tail : public Attack {
public:
    explicit Rope_tail(glm::vec2 sourcePosition, glm::vec2 targetPosition, std::shared_ptr<Attributes> attributes);
    bool CheckAndReverse();
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
    [[nodiscard]] glm::vec2 GetSourcePosition();
private:
    glm::vec2 m_SourcePosition;
    glm::vec2 m_GoalPosition;
    float m_Length;
    float m_Angle;
    bool m_IsReturning = false;
    bool m_IsFinished = false;
}; 

class Dropbox : public Attack {
public:
    explicit Dropbox(glm::vec2 position);
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    int time = 120;
};

class TheBird : public Attack {
public:
    explicit TheBird(glm::vec2 centerPosition, glm::vec2 useless, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut();
    [[nodiscard]] bool IsAlive() ;  
    void UpdatePosition();
    void RotateImage();
private:
    int time = 1200;
    glm::vec2 m_CenterPosition;
    float m_Radius;
    int max_Penetration;         // 设置最大穿透值为50
    int renewPenetrationCd;       // 每5帧恢复一次穿透值
    bool WillNotDisappear ; 
    float m_CurrentAngle;
};

class smallbomb : public Attack {
public:
    explicit smallbomb(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attack> bomb, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    std::shared_ptr<Attack> m_Bomb; 
    int existTime = 3;
};

class Chasenormal : public Attack {
public:
    explicit Chasenormal(glm::vec2 position, glm::vec2 goal_position, glm::vec2 chase_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    bool IsOut() override;
private:
    int time = 0;
    glm::vec2 m_ChasePosition;
};

class BombPiapple : public Attack {
public:
    explicit BombPiapple(glm::vec2 position);
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    int time = 180;
};
class Explosionnew : public Attack {
public:
    explicit Explosionnew(std::shared_ptr<Attack> bomb);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    std::shared_ptr<Attack> m_Bomb;
    int existTime = 3;
};

class lightExplosion : public Attack {
public:
    explicit lightExplosion(std::shared_ptr<Attack> bomb);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    std::shared_ptr<Attack> m_Bomb;
    int existTime = 5;
};


// New WindAttack class
class WindAttack : public Attack {
public:
    explicit WindAttack(glm::vec2 position, glm::vec2 goal_position, std::shared_ptr<Attributes> attributes);
    void Move() override;
    [[nodiscard]] bool IsOut() override;
    [[nodiscard]] bool IsAlive() override;
private:
    glm::vec2 m_GoalPosition;
    bool m_HasReachedGoal = false;
    int m_StayDuration = 100;
};
