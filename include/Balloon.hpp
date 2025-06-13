#pragma once
#ifndef BALLOON_HPP
#define BALLOON_HPP

#include <vector>
#include <string>
#include <memory>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "DebuffView.hpp"

// 前向聲明
class Attack;

class Balloon : public Util::GameObject {
public:
    enum Type {
        balloon,
        spaceship
    };

    explicit Balloon(std::vector<glm::vec2> coordinates);

    void SetPosition(const glm::vec2& Position);
    void SetImage(const std::string& ImagePath);
    void SetHealth(int num);
    void SetStageHealth(int num);
    void SetSpeed(float num);
    void SetRectangleCorners();
    void SetRotation();
    void SetRotation(float angle);
    void SetDistance(float distance);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetMoney(int money);
    void LoseHealth(int lose);
    void GetDebuff(std::vector<std::vector<int>> debuff);
    void Move();
    void SetType(Balloon::Type type);
    void AddProperty(int property);
    void SetTargetPosition(const glm::vec2& targetPosition);
    virtual void Injured();
    int ShowDebuff(int n){return m_Debuff[n];}
    void ClearDebuff();



    [[nodiscard]] int IsAttackEffective(std::vector<int> properties, int power);
    [[nodiscard]] virtual std::vector<std::shared_ptr<Balloon>> Burst() const;
    [[nodiscard]] int GetProperty(int n);

    [[nodiscard]] bool IsCollision(const std::shared_ptr<Attack>& other);
    [[nodiscard]] bool IsAlive() const { return m_Health > 0;};
    [[nodiscard]] bool IsArrive() const { return m_Coordinates.size() == 0;}

    [[nodiscard]] Balloon::Type GetType() const { return m_Type; }
    [[nodiscard]] glm::vec2 GetPosition() const { return m_Transform.translation; }
    [[nodiscard]] std::vector<glm::vec2> GetCoordinates() const { return m_Coordinates; }
    [[nodiscard]] int GetHealth() const { return m_Health; }
    [[nodiscard]] int GetStageHealth() const { return stage_Health; }
    [[nodiscard]] int GetMoney() const { return m_Money; }
    [[nodiscard]] float GetSpeed() const { return m_Speed; }
    [[nodiscard]] float GetDistance() const { return m_Distance; }
    [[nodiscard]] const std::string& GetImagePaths() const { return m_ImagePath; }
    [[nodiscard]] std::vector<glm::vec2> GetConers() const { return m_Corners; }
    [[nodiscard]] float UpdateDebuff();
    [[nodiscard]] std::vector<std::shared_ptr<Util::GameObject>> GetDebuffViews();

private:
    int m_Health = 0;
    int stage_Health = 0;
    float m_Speed = 0;
    float m_Distance = 0;
    float m_Radian = 0;
    int m_Width = 0;
    int m_Height = 0;
    int m_Money= 0;
    Balloon::Type m_Type;
    std::string m_ImagePath;
    std::vector<glm::vec2> m_Coordinates;
    std::vector<glm::vec2> m_Corners = {glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0), glm::vec2(0,0)};
    std::vector<int> m_Properties = {0};

    std::shared_ptr<DebuffView> snow =  std::make_shared<Snow>();
    std::shared_ptr<DebuffView> ice =  std::make_shared<Ice>();
    std::shared_ptr<DebuffView> rubber =  std::make_shared<Mucus>();
    std::shared_ptr<DebuffView> rock_ninja =  std::make_shared<RockNinjaDebuff>();
    std::shared_ptr<DebuffView> dizzylight =  std::make_shared<Dizzylight>(); // make debuff step1
    std::shared_ptr<DebuffView> iceburst =  std::make_shared<Iceburst>(); // make debuff step2
    std::shared_ptr<DebuffView> iced =  std::make_shared<Iced>(); // make debuff step2
    std::vector<int> m_Debuff = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0};// make debuff step2
    std::vector<float> debuff_slow = {0.5, 0, 0.2, 0, 0.5, -0.5, 0, 1, 0.5, 1, 1, 1, 1, 1, 1,-0.2}; // make debuff step3
};

class RED : public Balloon {
public:
    explicit RED(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class BLUE : public Balloon {
public:
    explicit BLUE(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class GREEN : public Balloon {
public:
    explicit GREEN(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class YELLOW : public Balloon {
public:
    explicit YELLOW(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class PINK : public Balloon {
public:
    explicit PINK(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class BLACK : public Balloon {
public:
    explicit BLACK(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class WHITE : public Balloon {
public:
    explicit WHITE(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class PURPLE : public Balloon {
public:
    explicit PURPLE(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class ZEBRA : public Balloon {
public:
    explicit ZEBRA(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class IRON : public Balloon {
public:
    explicit IRON(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class RAINBOW : public Balloon {
public:
    explicit RAINBOW(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class CERAMICS : public Balloon {
public:
    explicit CERAMICS(std::vector<glm::vec2> coordinates);
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class MOAB : public Balloon {
public:
    explicit MOAB(std::vector<glm::vec2> coordinates);
    void Injured() override;
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class BFB : public Balloon {
public:
    explicit BFB(std::vector<glm::vec2> coordinates);
    void Injured() override;
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class ZOMG : public Balloon {
public:
    explicit ZOMG(std::vector<glm::vec2> coordinates);
    void Injured() override;
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class DDT : public Balloon {
public:
    explicit DDT(std::vector<glm::vec2> coordinates);
    void Injured() override;
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

class BAD : public Balloon {
public:
    explicit BAD(std::vector<glm::vec2> coordinates);
    void Injured() override;
    [[nodiscard]] std::vector<std::shared_ptr<Balloon>> Burst() const override;
};

#endif // BALLOON_HPP

