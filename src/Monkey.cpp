#include "Monkey.hpp"

#include "Util/Logger.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

#define PI 3.14159265358979323846

Monkey::Monkey(glm::vec2 position)
{
    SetPosition(position);
    SetSkillTime(600); //技能持續時間
}

void Monkey::SetPosition(const glm::vec2& Position){
    m_Transform.translation = Position;
}

void Monkey::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

void Monkey::SetRadius(int radius) {
    m_Radius = radius;
};

void Monkey::SetProperties(const std::vector<int>& properties) {
    // 重設屬性
    auto attributes = GetAttributes();
    
    // 先清除現有屬性列表
    // 由於 Attributes 類沒有直接清除屬性的方法，我們需要創建一個新的 Attributes 對象
    std::shared_ptr<Attributes> newAttributes = std::make_shared<Attributes>();
    
    // 複製原有的屬性值
    newAttributes->SetPenetration(attributes->GetPenetration());
    newAttributes->SetPower(attributes->GetPower());
    newAttributes->SetSpeed(attributes->GetSpeed());
    
    // 複製原有的 debuff
    for (const auto& debuff : attributes->GetDebuff()) {
        newAttributes->AddDebuff(debuff);
    }
    
    // 設置新的屬性
    for (int property : properties) {
        newAttributes->AddProperty(property);
    }
    
    // 更新猴子的 Attributes
    m_Attributes = newAttributes;
}

void Monkey::addtogethermonkey(std::shared_ptr<Monkey> monkey) {
    togethersell_monkeys.push_back(monkey);
}

void Monkey::SetSize(glm::vec2 size){
    m_Size = size;
};

void Monkey::UpdateRange() {
    m_Range -> SetPosition(m_Transform.translation);
    m_Range -> SetScale(m_Radius);
};

bool Monkey::IsInside(glm::vec2 mousePosition){
    // 计算猴子的边界范围
    float leftBound = m_Transform.translation.x - m_Size.x / 2.0f;
    float rightBound = m_Transform.translation.x + m_Size.x / 2.0f;
    float topBound = m_Transform.translation.y + m_Size.y / 2.0f;
    float bottomBound = m_Transform.translation.y - m_Size.y / 2.0f;
    
    // 判断鼠标点击位置是否在猴子范围内
    return (mousePosition.x >= leftBound &&
            mousePosition.x <= rightBound &&
            mousePosition.y >= bottomBound &&
            mousePosition.y <= topBound);
}

void Monkey::SetSkillTime(int time) {
    skill_time = time;
}

void Monkey::SetSkillCountdown() {
    skill_countdown = skill_time;
}

// 判断猴子是否在一个矩形区域内
bool Monkey::IsMonkeyInRectangle(glm::vec2 topLeft, glm::vec2 bottomRight) {
    // 计算猴子的边界范围
    float monkeyLeft = m_Transform.translation.x - m_Size.x / 2.0f;
    float monkeyRight = m_Transform.translation.x + m_Size.x / 2.0f;
    float monkeyTop = m_Transform.translation.y + m_Size.y / 2.0f;
    float monkeyBottom = m_Transform.translation.y - m_Size.y / 2.0f;
    LOG_DEBUG("monkeyLeft: {}, monkeyRight: {}, monkeyTop: {}, monkeyBottom: {}", monkeyLeft, monkeyRight, monkeyTop, monkeyBottom);
    
    // 使用 AABB 碰撞检测来判断是否有重叠
    // 如果一个矩形在另一个矩形的左侧、右侧、上方或下方，则它们不重叠
    if (monkeyRight < topLeft.x || 
        monkeyLeft > bottomRight.x || 
        monkeyBottom > topLeft.y || 
        monkeyTop < bottomRight.y) {
        return false;
    }
    
    // 如果没有满足上述任一条件，则矩形重叠
    return true;
}

void Monkey::SetRangeColor(bool is_placeable){
    if (is_placeable){
        m_Range -> SetImage(GA_RESOURCE_DIR"/Monkey/range.png");
    } else {
        m_Range -> SetImage(GA_RESOURCE_DIR"/Monkey/range_red.png");
    }
}

bool Monkey::Touched(Monkey& other){
    // 计算当前猴子的边界范围
    float thisLeftBound = m_Transform.translation.x - m_Size.x / 2.0f;
    float thisRightBound = m_Transform.translation.x + m_Size.x / 2.0f;
    float thisTopBound = m_Transform.translation.y + m_Size.y / 2.0f;
    float thisBottomBound = m_Transform.translation.y - m_Size.y / 2.0f;
    
    // 计算另一个猴子的边界范围
    float otherLeftBound = other.m_Transform.translation.x - other.m_Size.x / 2.0f;
    float otherRightBound = other.m_Transform.translation.x + other.m_Size.x / 2.0f;
    float otherTopBound = other.m_Transform.translation.y + other.m_Size.y / 2.0f;
    float otherBottomBound = other.m_Transform.translation.y - other.m_Size.y / 2.0f;
    
    // 检查两个矩形是否重叠（AABB碰撞检测）
    // 如果一个矩形在另一个矩形的左侧、右侧、上方或下方，则它们不重叠
    if (thisRightBound < otherLeftBound || 
        thisLeftBound > otherRightBound || 
        thisBottomBound > otherTopBound || 
        thisTopBound < otherBottomBound) {
        return false;
    }
    
    // 如果没有满足上述任一条件，则矩形重叠
    return true;
}

bool Monkey::IsClicked(glm::vec2 mousePosition) {
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        if (IsInside(mousePosition)) {
            UpdateAllObjectVisible(true);
            return true;
        } else {
            UpdateAllObjectVisible(false);
            return false;
        }
    }
    return false;
}

void Monkey::SetCd(int cd) {
    m_Cd = cd;
};

void Monkey::SetCost(int cost) {
    m_Cost = cost;
    m_Value = cost;
}

std::vector<std::shared_ptr<Attack>> Monkey::ProduceAttack(glm::vec2 goalPosition){
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    return attacks;
}

bool Monkey::IsCollision(const std::shared_ptr<Balloon>& other) const{
    glm::vec2 position = other->GetPosition();
    float distance = sqrt(pow(position.x - m_Transform.translation.x, 2) + pow(position.y - m_Transform.translation.y, 2));
    return distance <= m_Radius;
}

void Monkey::SetRotation(glm::vec2 Position) {
    double dx = Position.x - m_Transform.translation.x;
    double dy = Position.y - m_Transform.translation.y;
    double angle_rad = atan2(dy, dx);
    m_Transform.rotation = angle_rad;
}

bool Monkey::Countdown() {
    SkillCountdown();
    m_InformationBoard -> SkillCountDown();
    if (m_Count == m_Cd) {
        return true;
    }
    m_Count += 1;
    return otheratk();
}
bool Monkey::otheratk(){
    return false;
}

void Monkey::ResetCount(){
    m_Count = 0;
}


int Monkey::IsInformationBoardClicked(glm::vec2 mousePosition, int money) {
    std::vector<int> val = m_InformationBoard -> IsClick(mousePosition, money);
    int res = val[0];
    if (val[0] == 5) {
        level += 1;
        upgradePath = 1;
        UpdateLevel();
        m_Value += val[1];
        res = val[1];
    }
    else if (val[0] == 6) {
        level += 1;
        upgradePath = 2;
        UpdateLevel();
        m_Value += val[1];
        res = val[1];
    }
    return res;
}

bool Monkey::Placeable(std::vector<std::vector<std::vector<glm::vec2>>> Level_Placeable){
    // 检查猴子是否在 x >= 388 的区域
    float monkeyLeft = m_Transform.translation.x - m_Size.x / 2.0f;
    float monkeyRight = m_Transform.translation.x + m_Size.x / 2.0f;
    if (monkeyRight >= 388.0) {
        LOG_DEBUG("猴子位置超出 x=388 的限制");
        return false;
    }

    std::vector<std::vector<glm::vec2>> groundplacelist = Level_Placeable[0];
    for (auto& groundplacestr : groundplacelist) {
        glm::vec2 topLeft = groundplacestr[0];    // 左上角坐标
        glm::vec2 bottomRight = groundplacestr[1]; // 右下角坐标
        // 驗證 topleft 和 bottomright 的順序
        if (topLeft.x > bottomRight.x || topLeft.y < bottomRight.y) {
            std::swap(topLeft, bottomRight);
        }
        
        if (IsMonkeyInRectangle(topLeft, bottomRight)) {
            LOG_DEBUG("碰到了: {}, {}", topLeft, bottomRight);
            return false;
        }
        LOG_DEBUG("沒碰到:{}, {}", topLeft, bottomRight);
    }
    // 如果猴子不在任何一个矩形区域内，返回true
    return true;
}

void Monkey::IsButtonTouch(glm::vec2 mousePosition) {
    m_InformationBoard -> IsButtonTouch(mousePosition);
}

std::vector<std::shared_ptr<Util::GameObject>> Monkey::GetAllInfortionBoardObject() {
    std::vector<std::shared_ptr<Util::GameObject>> infortionBoardObjects = m_InformationBoard -> GetAllChildren();
    infortionBoardObjects.push_back(m_InformationBoard);
    return infortionBoardObjects;
}

void Monkey::UpdateAllObjectVisible(bool isClicked) {
    m_Range -> SetVisible(isClicked);
    m_InformationBoard -> UpdateAllObjectVisible(isClicked);
}

void Monkey::UpdateLevel() {}

void Monkey::UseSkill(){}

void Monkey::EndSkill(){}

void Monkey::SkillCountdown() {
    skill_countdown -= 1;
    if (skill_countdown == 0) {
        EndSkill();
    }
}

// ####################################################################

DartMonkey::DartMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(40);

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<DartMonkeyInformationBoard>();

    SetCost(200);
    SetImage(GA_RESOURCE_DIR"/Monkey/DartMonkey.png");
    SetCd(50);
    SetRadius(150);
    UpdateRange();
}

void DartMonkey::UseSkill() {
    skillEffect = true;
    SetSkillCountdown();
    SetImage(GA_RESOURCE_DIR"/Monkey/SuperMonkey.png");
    cd_radius_tmp = {GetCd(), GetRadius()};
    SetCd(20);
    ResetCount();
    SetRadius(300);
    UpdateRange();
    auto informationBoard = GetInfortionBoard();
    informationBoard -> SetSkillEffect(true);
}

void DartMonkey::EndSkill() {
    skillEffect = false;
    SetImage(GA_RESOURCE_DIR"/Monkey/DartMonkey.png");
    SetCd(cd_radius_tmp[0]);
    ResetCount();
    SetRadius(cd_radius_tmp[1]);
    UpdateRange();
    auto informationBoard = GetInfortionBoard();
    informationBoard -> SetSkillEffect(false);
}

std::vector<std::shared_ptr<Attack>> DartMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    std::vector<std::shared_ptr<Attack>> attacks;
    if (skillEffect == true) {
        glm::vec2 direction = goalPosition - m_Transform.translation;
        glm::vec2 unit_direction = glm::normalize(direction);
        double perp_x = -unit_direction.y;
        double perp_y = unit_direction.x;
        int distance = 15;

        glm::vec2 movePosition = glm::vec2(distance * perp_x, distance * perp_y);

        std::shared_ptr<Attack> attack = std::make_shared<Ray>(GetPosition()+movePosition, goalPosition+movePosition, GetAttributes());
        attacks.push_back(attack);
        attack = std::make_shared<Ray>(GetPosition()-movePosition, goalPosition-movePosition, GetAttributes());
        attacks.push_back(attack);
    }
    else if (upgradePath == 1 && level >= 3) {
        std::shared_ptr<Attack> attack = std::make_shared<Rock>(GetPosition(), goalPosition, GetAttributes());
        attacks.push_back(attack);
    }
    else {
        std::shared_ptr<Attack> attack = std::make_shared<Dart>(GetPosition(), goalPosition, GetAttributes());
        attacks.push_back(attack);
        if (upgradePath == 2 && level >= 3) {
            double radians = 30 * PI / 180.0;  // 角度轉為弧度
            double cosA = cos(radians);
            double sinA = sin(radians);
            glm::vec2 center = m_Transform.translation;
            double x = center.x + (goalPosition.x - center.x) * cosA - (goalPosition.y - center.y) * sinA;
            double y = center.y + (goalPosition.x - center.x) * sinA + (goalPosition.y - center.y) * cosA;
            attack = std::make_shared<Dart>(GetPosition(), glm::vec2(x,y), GetAttributes());
            attacks.push_back(attack);

            radians = -30 * PI / 180.0;  // 角度轉為弧度
            cosA = cos(radians);
            sinA = sin(radians);
            center = m_Transform.translation;
            x = center.x + (goalPosition.x - center.x) * cosA - (goalPosition.y - center.y) * sinA;
            y = center.y + (goalPosition.x - center.x) * sinA + (goalPosition.y - center.y) * cosA;
            attack = std::make_shared<Dart>(GetPosition(), glm::vec2(x,y), GetAttributes());
            attacks.push_back(attack);
        }
    }
    return attacks;
}

void DartMonkey::UpdateLevel() {
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                SetRadius(GetRadius() *1.25);
                UpdateRange();
                break;
            case 2:
                SetRadius(GetRadius() *1.25);
                UpdateRange();
                attributes -> AddProperty(2);
                break;
            case 3:
                SetCd(150);
                ResetCount();
                attributes -> SetPenetration(18);
                break;
            case 4:
                attributes -> AddProperty(1);
                attributes -> AddProperty(3);
                break;
        }
    }
    else {
        switch (level) {
            case 1:
                attributes -> SetPenetration(2);
                attributes -> SetPower(2);
                break;
            case 2:
                attributes -> SetPenetration(3);
                attributes -> SetPower(3);
                break;
        }
    }
}

//#####################################################################

NailMonkey::NailMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(40);

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<NailMonkeyInformationBoard>();

    SetCost(360);
    SetImage(GA_RESOURCE_DIR"/Monkey/NailMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));
    SetCd(80);
    SetRadius(150);
    SetSkillTime(450);  // 技能持续时间
    UpdateRange();
}

void NailMonkey::UseSkill() {
    // 实现技能使用逻辑
    skillEffect = true;
    SetSkillCountdown();

}

void NailMonkey::EndSkill() {
    // 实现技能结束逻辑
    skillEffect = false;
}

std::vector<std::shared_ptr<Attack>> NailMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    std::vector<std::shared_ptr<Attack>> attacks;
    if (skillEffect == true) {
        // 設置固定半徑
        float radius = 30.0f;  // 改回合理的半徑值，防止飛太遠看不到
        int skillTime = GetSkillTime();
        int skillCountdown = GetSkillCountdown();
        
        // 直接使用skillCountdown計算旋轉角度
        float anglePerTick = PI/15; // 每tick旋转角度
        float angle = skillCountdown * anglePerTick;
        
        // 計算圓周上的點(作為攻擊起始點)
        float x1 = m_Transform.translation.x + radius * cos(angle);
        float y1 = m_Transform.translation.y + radius * sin(angle);
        float x2 = m_Transform.translation.x + radius * cos(angle + PI);
        float y2 = m_Transform.translation.y + radius * sin(angle + PI);
        
        // 計算切線方向(攻擊移動方向)
        float tangent_x1 = -sin(angle);  // 切線x分量
        float tangent_y1 = cos(angle);   // 切線y分量
        float tangent_x2 = -sin(angle + PI);
        float tangent_y2 = cos(angle + PI);
        
        // 計算目標點(沿切線方向)
        float target_x1 = x1 + tangent_x1 * 100;  // 沿切線前進100單位(可調整)
        float target_y1 = y1 + tangent_y1 * 100;
        float target_x2 = x2 + tangent_x2 * 100;
        float target_y2 = y2 + tangent_y2 * 100;
        
        // 創建兩個攻擊，起始點是圓周上的點
        std::shared_ptr<Attack> attack1 = std::make_shared<Knife>(
            glm::vec2(x1, y1),  // 從圓周上的點發射
            glm::vec2(target_x1, target_y1),  // 目標是沿切線方向
            9999999.0f,  // 極大值作為無限半徑
            GetAttributes()
        );
        std::shared_ptr<Attack> attack2 = std::make_shared<Knife>(
            glm::vec2(x2, y2),  // 從圓周上的對稱點發射
            glm::vec2(target_x2, target_y2),  // 目標是沿切線方向
            9999999.0f,  // 極大值作為無限半徑
            GetAttributes()
        );
        
        // 調整速度和大小
        attack1->SetSpeed(attack1->GetSpeed() * 0.5f);
        attack2->SetSpeed(attack2->GetSpeed() * 0.5f);
        
        attacks.push_back(attack1);
        attacks.push_back(attack2);
        
        return attacks;
    }
    // if (goalPosition == glm::vec2(100000, 100000)) {
    //     return {};
    // }

    if (upgradePath == 1 && level == 4) {
        std::shared_ptr<Attack> attack = std::make_shared<Fire>(GetPosition(), goalPosition, GetAttributes(), GetRadius());
        attacks.push_back(attack);
    }
    else if (upgradePath == 2 && level >= 3) {
        if (upgradePath == 2 && level == 4) {
            for (int i = 0; i < 8; i++) {
                double theta = i * PI / 4 + PI / 8;  // 角度轉為弧度
                double x = m_Transform.translation.x + 1 * cos(theta);
                double y = m_Transform.translation.y + 1 * sin(theta);
                std::shared_ptr<Attack> attack = std::make_shared<Knife>(GetPosition(), glm::vec2(x,y), GetRadius(), GetAttributes());
                attacks.push_back(attack);
            }
        }
        for (int i = 0; i < 8; i++) {
            double theta = i * PI / 4;  // 角度轉為弧度
            double x = m_Transform.translation.x + 1 * cos(theta);
            double y = m_Transform.translation.y + 1 * sin(theta);
            std::shared_ptr<Attack> attack = std::make_shared<Knife>(GetPosition(), glm::vec2(x,y), GetRadius(), GetAttributes());
            attacks.push_back(attack);
        }
    }
    else {
        if (upgradePath == 1 && level == 3) {
            for (int i = 0; i < 8; i++) {
                double theta = i * PI / 4 + PI / 8;  // 角度轉為弧度
                double x = m_Transform.translation.x + 1 * cos(theta);
                double y = m_Transform.translation.y + 1 * sin(theta);
                std::shared_ptr<Attack> attack = std::make_shared<Nail>(GetPosition(), glm::vec2(x,y), GetRadius(), GetAttributes());
                attacks.push_back(attack);
            }
        }
        for (int i = 0; i < 8; i++) {
            double theta = i * PI / 4;  // 角度轉為弧度
            double x = m_Transform.translation.x + 1 * cos(theta);
            double y = m_Transform.translation.y + 1 * sin(theta);
            std::shared_ptr<Attack> attack = std::make_shared<Nail>(GetPosition(), glm::vec2(x,y), GetRadius(), GetAttributes());
            attacks.push_back(attack);
        }
    }

    return attacks;
}

void NailMonkey::UpdateLevel() {
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                SetCd(60);
                ResetCount();
                break;
            case 2:
                SetCd(30);
                ResetCount();
                break;
        }
    }
    else {
        switch (level) {
            case 1:
                SetRadius(GetRadius() *1.25);
                UpdateRange();
                break;
            case 2:
                SetRadius(GetRadius() *1.25);
                UpdateRange();
                break;
        }
    }
}

//#####################################################################

SniperMonkey::SniperMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(2);
    attributes -> SetPower(1);
    attributes -> SetSpeed(100);

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<SniperMonkeyInformationBoard>();

    SetCost(400);
    SetImage(GA_RESOURCE_DIR"/Monkey/SniperMonkey.png");
    SetCd(120);
    SetRadius(1500);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> SniperMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack = std::make_shared<Dart>(GetPosition(), goalPosition, GetAttributes());
    attack -> SetSpeed(100);
    attack -> SetPenetration(2);
    attacks.push_back(attack);
    return attacks;
}

void SniperMonkey::UpdateLevel() {
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                attributes -> SetPenetration(4);
                attributes -> AddProperty(1);
                attributes -> AddProperty(4);
                break;
            case 2:
                attributes -> SetPenetration(7);
                break;
            case 3:
                attributes -> SetPenetration(18);
                attributes -> AddProperty(3);
                break;
            case 4:
                attributes -> AddDebuff({3, 10});
                break;
        }
    }
    else {
        switch (level) {
            case 1:
                SetCd(90);
                ResetCount();
                break;
            case 2:
                attributes -> AddProperty(2);
                break;
            case 3:
                attributes -> AddProperty(2);
                SetCd(30);
                ResetCount();
                break;
        }
    }
}

void Monkey::AddAttackChild(std::shared_ptr<Attack> attack) {
    m_Attacks.push_back(attack);
}

//########################################################################

BoomerangMonkey::BoomerangMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/BoomerangMonkey.png");

    auto attributes = GetAttributes();
    attributes -> SetPenetration(3);
    attributes -> SetPower(1);
    attributes -> SetSpeed(10);

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<BoomerangMonkeyInformationBoard>();

    SetCost(400);
    SetCd(80);
    SetRadius(120);
    UpdateRange();
    SetSize(glm::vec2(50.0f, 50.0f));
}
void BoomerangMonkey::UseSkill() {
    skillEffect = true;
    SetImage(GA_RESOURCE_DIR"/Monkey/BoomerangMonkey_skilling.png");
    original_cd = GetCd();
    SetCd(10);
    ResetCount();
    SetSkillCountdown();
}

void BoomerangMonkey::EndSkill() {
    skillEffect = false;
    SetImage(GA_RESOURCE_DIR"/Monkey/BoomerangMonkey.png");
    SetCd(original_cd);
    ResetCount();
}

std::vector<std::shared_ptr<Attack>> BoomerangMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    std::vector<std::shared_ptr<Attack>> attacks;
    if (upgradePath == 1 && level == 2) {
        std::shared_ptr<Attack> attack = std::make_shared<BladedDisc>(GetPosition(), goalPosition, GetAttributes());
        attacks.push_back(attack);
    }
    else if (upgradePath == 1 && level >= 3) {
        std::shared_ptr<Attack> attack = std::make_shared<BladedDisc_Rebound>(GetPosition(), goalPosition, GetAttributes());
        attacks.push_back(attack);
    }
    else {
        std::shared_ptr<Attack> attack = std::make_shared<Boomerang>(GetPosition(), goalPosition, GetAttributes());
        attacks.push_back(attack);
    }
    if (upgradePath == 1 && level == 4 && !has_BladedDisc_Around) {
        has_BladedDisc_Around = true;
        glm::vec2 position = GetPosition();
        std::shared_ptr<Attack> attack = std::make_shared<BladedDisc_Around>(position, position+glm::vec2(1,0), GetAttributes());
        attacks.push_back(attack);
        AddAttackChild(attack);
        attack = std::make_shared<BladedDisc_Around>(GetPosition(), position-glm::vec2(1,0), GetAttributes());
        attacks.push_back(attack);
        AddAttackChild(attack);
    }
    return attacks;
}

void BoomerangMonkey::UpdateLevel() {
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                attributes -> SetPenetration(7);
                break;
            case 2:
                attributes -> SetPower(3);
                attributes -> SetSpeed(20);
                SetCd(60);
                ResetCount();
                break;
            case 4:
                attributes -> AddProperty(2);
                break;
        }
    }
    else {
        switch (level) {
            case 1:
                attributes -> AddProperty(4);
                break;
            case 2:
                attributes -> AddProperty(1);
                break;
            case 3:
                SetCd(40);
                ResetCount();
                break;
        }
    }
}

//########################################################################

NinjaMonkey::NinjaMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/NinjaMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(60);
    attributes -> AddProperty(2);

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<NinjaMonkeyInformationBoard>();

    SetCost(600);
    SetCd(50);
    SetRadius(150);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> NinjaMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    std::vector<std::shared_ptr<Attack>> attacks;
    if (upgradePath == 1 && level >= 3) {
        std::shared_ptr<Attack> attack = std::make_shared<Shuriken>(GetPosition(), goalPosition, GetAttributes());
        attacks.push_back(attack);
        if (upgradePath == 1 && level == 4) {
            for (int i = 0; i < 3; i++) {
                std::shared_ptr<Attack> attack = std::make_shared<Shuriken>(GetPosition(), goalPosition, GetAttributes());
                attacks.push_back(attack);
            }
        }
    }else if (upgradePath == 2 && level > 0) {
        std::shared_ptr<Attack> attack = std::make_shared<NinjaShuriken>(GetPosition(), goalPosition, GetAttributes());
        attacks.push_back(attack);
    }
    else {
        std::shared_ptr<Attack> attack = std::make_shared<Shuriken>(GetPosition(), goalPosition, GetAttributes());
        attacks.push_back(attack);
    }
    if (upgradePath ==2 && level==3){

        //have posiblity to produce attack 
        if (rand() % 100 < 30) {
            auto newAttributes = std::make_shared<Attributes>(*GetAttributes());
            newAttributes->SetPenetration(1);
            newAttributes->SetPower(0);
            newAttributes->SetSpeed(40);
            newAttributes->AddDebuff({6, 100});
            std::shared_ptr<Attack> attack1 = std::make_shared<Bomb>(GetPosition(), goalPosition, newAttributes);
            attack1 -> SetImage(GA_RESOURCE_DIR"/Attack/lightbomb.png");
            //change scale
            attack1 -> SetScale(glm::vec2(1.5, 1.5));
            attacks.push_back(attack1);
            std::shared_ptr<Attack> attack2 = std::make_shared<Explosionlight>(GetPosition(), goalPosition, attack1, newAttributes);
            attack2 -> SetVisible(false);
            attacks.push_back(attack2);
        }
    }
    return attacks;
}

void NinjaMonkey::UpdateLevel() {
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                SetCd(30);
                ResetCount();
                SetRadius(GetRadius()*1.5);
                UpdateRange();
                break;
            case 2:
                attributes -> SetPenetration(4);
                break;
        }
    }
    else {
        switch (level) {
            case 1:
                attributes -> SetPenetration(20);
                break;
            case 2:
                // attributes -> SetPenetration(40);
                attributes -> AddDebuff({5, 100});
                break;
        }
    }
}

//########################################################################

Cannon::Cannon(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/Cannon.png");

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(40);

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<CannonInformationBoard>();

    SetCost(700);
    SetCd(100);
    SetRadius(160);
    UpdateRange();
    SetSize(glm::vec2(50.0f, 50.0f));
}

std::vector<std::shared_ptr<Attack>> Cannon::ProduceAttack(glm::vec2 goalPosition) {
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    std::vector<std::shared_ptr<Attack>> remove_attacks;
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    std::shared_ptr<Attack> attack1 = std::make_shared<Bomb>(GetPosition(), goalPosition, GetAttributes());
    attacks.push_back(attack1);
    std::shared_ptr<Attack> attack2 = std::make_shared<Explosion>(GetPosition(), goalPosition, attack1, GetAttributes());
    attack2 -> SetVisible(false);
    attacks.push_back(attack2);
    if (upgradePath == 1){
        switch (level) {
            case 4:{

            }
            case 3:{
                double shift = (rand() % 100) / 100.0 * (PI/6);  // 隨機偏移最多30度
                for (int i = 0; i < 3; i++) {
                    double theta = i * 2*PI / 3 + shift;  // 三等分圓，每個方向隨機偏移最多30度
                    auto attributes = std::make_shared<Attributes>(*GetAttributes());
                    attributes -> SetPower(1);
                    attributes -> SetSpeed(20);
                    double targetX = goalPosition.x + cos(theta) * 100; // 100單位距離，可調整
                    double targetY = goalPosition.y + sin(theta) * 100;
                    glm::vec2 targetPos = glm::vec2(targetX, targetY);
                    std::shared_ptr<Attack> attack3 = std::make_shared<smallbomb>(goalPosition, targetPos, attack1, attributes);
                    attack3 -> SetVisible(false);
                    attack3 -> SetScale(glm::vec2(0.7, 0.7));
                    attack3 -> SetTouchScale(glm::vec2(0.7, 0.7));
                    attack3 -> SetPosition(goalPosition);
                    attacks.push_back(attack3);
                    
                    std::shared_ptr<Attack> attack4 = std::make_shared<Explosion>(goalPosition, targetPos, attack3, attributes);
                    attack4 -> SetTouchScale(glm::vec2(0.7, 0.7));
                    attack4 -> SetImage(GA_RESOURCE_DIR"/Attack/Explosion_small.png");
                    attack4 -> SetScale(glm::vec2(0.7, 0.7));
                    attack4 -> SetPosition(goalPosition);
                    attack4 -> SetVisible(false);
                    attacks.push_back(attack4);
                }
                break;
            }
            case 2:{
                for (int i = 0; i < 8; i++) {
                    double theta = i * PI / 4;  // 八等分圓 (0, 45, 90, 135, 180, 225, 270, 315度)
                    auto attributes = std::make_shared<Attributes>(*GetAttributes());
                    attributes -> SetPower(1);
                    attributes -> SetSpeed(30);
                    // 計算八個方向的坐標點 (作為目標點)
                    double targetX = goalPosition.x + cos(theta) * 100; // 100單位距離，可調整
                    double targetY = goalPosition.y + sin(theta) * 100;
                    glm::vec2 targetPos = glm::vec2(targetX, targetY);
                    std::shared_ptr<Attack> attack3 = std::make_shared<Explosion_slice>(goalPosition, targetPos, attack1, attributes);
                    attack3 -> SetVisible(false);
                    attacks.push_back(attack3);
                }
            }
            case 1:
                // 第一級不需要額外攻擊
                break;
        }
    }else{
        switch (level) {
            case 4:
                
            case 3:
                attack1 -> SetImage(GA_RESOURCE_DIR"/Attack/bombv2.png");
                attack1 -> SetPower(attack1 -> GetPower()*5);
                attack1 -> SetScale(glm::vec2(2, 2));
                attack2 -> SetScale(glm::vec2(2, 2));
                attack1 -> SetTouchScale(glm::vec2(2, 2));
                attack2 -> SetTouchScale(glm::vec2(2, 2));
                break;
                
            case 2:
                attack1 -> SetImage(GA_RESOURCE_DIR"/Attack/bombv1.png");
                attack1 -> SetPower(attack1 -> GetPower()*2);
                attack1 -> SetScale(glm::vec2(2, 2));
                attack2 -> SetScale(glm::vec2(2, 2));
                attack1 -> SetTouchScale(glm::vec2(2, 2));
                attack2 -> SetTouchScale(glm::vec2(2, 2));
                break;
            case 1:
                attack1 -> SetScale(glm::vec2(3, 3));
                attack2 -> SetScale(glm::vec2(3, 3));
                attack1 -> SetTouchScale(glm::vec2(3, 3));
                attack2 -> SetTouchScale(glm::vec2(3, 3));
                break;
        }
    }
    return attacks;
}

void Cannon::UpdateLevel() {
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                ResetCount();
                SetRadius(GetRadius()*1.5);
                UpdateRange();
                break;
            case 4:
                attributes -> AddDebuff({6, 5});
                break;
        }
    }else if (upgradePath == 2) {
        switch (level) {
            case 4:
                break;
            case 3:
                LOG_DEBUG("add property 5============================");
                attributes -> AddProperty(5);//10 mutiple damage
                break;
            case 2:
                SetRadius(GetRadius()*1.5);
                UpdateRange();
                break;
        }
    }
}
//########################################################################

Airport::Airport(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/Airport.png");

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(40);

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<AirportInformationBoard>();

    SetCost(950);
    SetCd(100);
    SetRadius(50);
    UpdateRange();
    SetSize(glm::vec2(50.0f, 50.0f));
}

void Airport::UpdateLevel() {
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                SetCd(GetCd()/2);
                ResetCount();
                break;
            case 2:
                GetAttributes() -> SetPenetration(GetAttributes() -> GetPenetration()*8);
                break;
            case 3:

                break;
            case 4:
                break;
        }
    }else{
        switch (level) {
            case 1:
                break;
            case 2:
                GetAttributes() -> AddProperty(2);
                break;
            case 3:
                break;
        }

    }
}

std::vector<std::shared_ptr<Attack>> Airport::ProduceAttack(glm::vec2 goalPosition) {
    glm::vec2 position = GetPosition();
    std::vector<std::shared_ptr<Attack>> remove_attacks;
    
    std::vector<std::shared_ptr<Attack>> attacks;
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    if (GetCount() == GetCd()) {
        ResetCount();
        if (upgradePath == 1 && level > 2) {
            if (level == 4) {
                attack_type = !attack_type;
            }
            if (attack_type) {
                for (auto& airplanePtr : m_Airplanes) {
                    glm::vec2 position = airplanePtr -> GetPosition();
                    for (int i=0; i < 8; i++) {
                        glm::vec2 rotated_position = ProduceCoordinateByAngle(position, 45*i);
                        std::shared_ptr<Attack> attack = std::make_shared<Chasenormal>(position, rotated_position,goalPosition, GetAttributes());
                        attacks.push_back(attack);
                    }
                }
            }else{
                for (auto& airplanePtr : m_Airplanes) {
                    glm::vec2 position = airplanePtr -> GetPosition();
                    std::shared_ptr<Attack> attack1 = std::make_shared<Bomb>(position, goalPosition, GetAttributes());
                    attack1 -> SetImage(GA_RESOURCE_DIR"/Attack/Airplanebomb.png");
                    attack1 -> SetPenetration(1);
                    attacks.push_back(attack1);
                    std::shared_ptr<Attack> attack2 = std::make_shared<Explosion>(position, goalPosition, attack1, GetAttributes());
                    attack2 -> SetScale(glm::vec2(10, 10));
                    attack2 -> SetTouchScale(glm::vec2(10, 10));
                    attack2 -> SetVisible(false);
                    attacks.push_back(attack2);
                }
            }
        }else if (upgradePath == 2 && level >2){
            for (auto& airplanePtr : m_Airplanes) {
                glm::vec2 position = airplanePtr -> GetPosition();
                for (int i=0; i < 16; i++) {
                    glm::vec2 rotated_position = ProduceCoordinateByAngle(position, 22.5*i);
                    std::shared_ptr<Attack> attack = std::make_shared<Dart>(position, rotated_position, GetAttributes());
                    attacks.push_back(attack);
                }
            }
        } else{
            for (auto& airplanePtr : m_Airplanes) {
                glm::vec2 position = airplanePtr -> GetPosition();
                for (int i=0; i < 8; i++) {
                    glm::vec2 rotated_position = ProduceCoordinateByAngle(position, 45*i);
                    std::shared_ptr<Attack> attack = std::make_shared<Dart>(position, rotated_position, GetAttributes());
                    attacks.push_back(attack);
                }
            }
        }
    }
    if (upgradePath == 2){
        if (level>0 && otheratk_cd[0] == 0){
            otheratk_cd[0] = 180;
            for (auto& airplanePtr : m_Airplanes) {
                glm::vec2 position = airplanePtr -> GetPosition();
                std::shared_ptr<Attack> attack = std::make_shared<BombPiapple>(position);
                attack -> SetScale(glm::vec2(0.05, 0.05));
                std::shared_ptr<Attack> attack2 = std::make_shared<Explosionnew>(attack);
                attack2 -> SetVisible(false);
                attacks.push_back(attack);
                attacks.push_back(attack2);
            }
        }
    }

    if (m_Airplanes.size() < airplane_num) {
        glm::vec2 rotated_position = ProduceCoordinateByAngle(position, 0);
        std::shared_ptr<Attack> attack = std::make_shared<Airplane>(position, rotated_position, GetAttributes());
        m_Airplanes.push_back(attack);
        AddAttackChild(attack);
        attacks.push_back(attack);
    }

    return attacks;
}

bool Airport::otheratk(){
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    if (upgradePath == 2) {
        switch (level) {
            case 4:
            case 3:
            case 2:
            case 1:
                if (otheratk_cd[0] == 0) { 
                    return true;
                }
                otheratk_cd[0] -= 1;
                break;
            case 0:
                return false;
        }
    }
    return false;
}

bool Airport::IsCollision(const std::shared_ptr<Balloon>& other) const{
    return true;
}

glm::vec2 Airport::ProduceCoordinateByAngle(glm::vec2 position, float angle) {
    double radian = angle * PI / 180.0;
    glm::vec2 rotated;
    glm::vec2 rotated_position;
    rotated.x = position.x * cos(radian) - position.y * sin(radian);
    rotated.y = position.x * sin(radian) + position.y * cos(radian);
    rotated_position.x = position.x + rotated.x;
    rotated_position.y = position.y + rotated.y;
    return rotated_position;
}

//########################################################################

BuccaneerMonkey::BuccaneerMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.5f, 1.5f);
    SetImage(GA_RESOURCE_DIR"/Monkey/BuccaneerMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(1);
    attributes -> SetSpeed(40);

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<BuccaneerMonkeyInformationBoard>();

    SetCost(600);
    SetCd(120);
    SetRadius(200);
    UpdateRange();
}

bool BuccaneerMonkey::otheratk(){
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    if (upgradePath == 2) {
        switch (level) {
            case 3:
                if (otheratk_cd[1] == 0) {
                    return true;
                }
                otheratk_cd[1] -= 1;
            case 1:
                if (otheratk_cd[0] == 0) { 
                    return true;
                }
                otheratk_cd[0] -= 1;
        }
    }
    return false;
}


void BuccaneerMonkey::UpdateLevel(){
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                SetCd(GetCd()/2);
                ResetCount();
                break;
            case 2:
                SetRadius(GetRadius()*1.5);
                UpdateRange();
                break;
            case 3:
                SetCd(GetCd()/2);
                ResetCount();
                break;
            case 4:
                break;
        }
    }else if (upgradePath == 2){
        switch (level) {
            case 1:
                break;
            case 2:
                attributes -> AddProperty(2);
                break;
            case 3:
                break;
            case 4:
                break;
        }   
    }
}

bool BuccaneerMonkey::Placeable(std::vector<std::vector<std::vector<glm::vec2>>> Level_Placeable){
    // 检查猴子是否在有效区域内
    glm::vec2 size = GetSize();
    float monkeyLeft = m_Transform.translation.x - size.x / 2.0f;
    float monkeyRight = m_Transform.translation.x + size.x / 2.0f;

    std::vector<std::vector<glm::vec2>> groundplacelist = Level_Placeable[1];
    for (auto& groundplacestr : groundplacelist) {
        glm::vec2 topLeft = groundplacestr[0];    // 左上角坐标
        glm::vec2 bottomRight = groundplacestr[1]; // 右下角坐标
        // 驗證 topleft 和 bottomright 的順序
        if (topLeft.x > bottomRight.x || topLeft.y < bottomRight.y) {
            std::swap(topLeft, bottomRight);
        }
        
        if (IsMonkeyInRectangle(topLeft, bottomRight)) {
            return true;
        }
    }
    // 如果猴子不在任何一个矩形区域内，返回true
    return false;
}

glm::vec2 BuccaneerMonkey::ProduceCoordinateByAngle(glm::vec2 position, float angle) {
    double radian = angle * PI / 180.0;
    glm::vec2 rotated;
    glm::vec2 rotated_position;
    rotated.x = position.x * cos(radian) - position.y * sin(radian);
    rotated.y = position.x * sin(radian) + position.y * cos(radian);
    rotated_position.x = position.x + rotated.x;
    rotated_position.y = position.y + rotated.y;
    return rotated_position;
}

std::vector<std::shared_ptr<Attack>> BuccaneerMonkey::ProduceAttack(glm::vec2 goalPosition) {
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    if (GetCount() == GetCd()) {
        ResetCount();

        std::shared_ptr<Attack> attack1 = std::make_shared<Bomb>(GetPosition(), goalPosition, GetAttributes());
        attacks.push_back(attack1);
        std::shared_ptr<Attack> attack2 = std::make_shared<Explosion>(GetPosition(), goalPosition, attack1, GetAttributes());
        attack2 -> SetVisible(false);
        attacks.push_back(attack2);
    }
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    if (upgradePath == 1) {
        switch (level) {
            // make plane
            case 4: {
                glm::vec2 position = GetPosition();
                std::vector<std::shared_ptr<Attack>> remove_attacks;
                for (auto& airplanePtr : m_Airplanes) {
                    glm::vec2 airplanePosition = airplanePtr->GetPosition();
                    for (int i = 0; i < 8; i++) {
                        glm::vec2 rotated_position = ProduceCoordinateByAngle(airplanePosition, 45*i);
                        std::shared_ptr<Attack> attack = std::make_shared<Dart>(airplanePosition, rotated_position, GetAttributes());
                        attacks.push_back(attack);
                    }
                }

                if (m_Airplanes.size() < airplane_num) {
                    glm::vec2 rotated_position = ProduceCoordinateByAngle(position, 0);
                    std::shared_ptr<Attack> attack = std::make_shared<Airplane>(position, rotated_position, GetAttributes());
                    m_Airplanes.push_back(attack);
                    AddAttackChild(attack);
                    attacks.push_back(attack);
                }
                break;
            }
        }
    } else if (upgradePath == 2) {
        switch (level) {
            case 3:{
                if (otheratk_cd[1] == 0) {
                    otheratk_cd[1] = 50;
                    std::shared_ptr<Attack> attack1 = std::make_shared<Bomb>(GetPosition(), goalPosition, GetAttributes());
                    attacks.push_back(attack1);
                    std::shared_ptr<Attack> attack2 = std::make_shared<Explosion>(GetPosition(), goalPosition, attack1, GetAttributes());
                    attack2 -> SetVisible(false);
                    attacks.push_back(attack2); 
                }
            }
            case 1:
            {
                if (otheratk_cd[0] == 0) {
                    otheratk_cd[0] = 100;
                    auto grap = std::make_shared<Dart>(GetPosition(), goalPosition, GetAttributes());
                    grap -> SetImage(GA_RESOURCE_DIR"/Attack/grap.png");
                    grap -> SetHeight(64);
                    grap -> SetWidth(42);
                    grap -> SetPenetration(4);
                    attacks.push_back(grap);
                }
                break;
            }
        }
    }
    return attacks;
}

//########################################################################

SuperMonkey::SuperMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/SuperMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(3);
    attributes -> SetSpeed(60);
    SetSkillTime(100);
    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<SuperMonkeyInformationBoard>();

    SetCost(4000);
    SetCd(20);
    SetRadius(300);
    UpdateRange();
}

void SuperMonkey::UseSkill() {
    skillEffect = true;

    SetSkillCountdown();
}
void SuperMonkey::EndSkill() {
    skillEffect = false;
}
std::vector<std::shared_ptr<Attack>> SuperMonkey::ProduceAttack(glm::vec2 goalPosition) {
    ResetCount();
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    int level = GetLevel();
    int upgradePath = GetUpgradePath();

    glm::vec2 direction = goalPosition - m_Transform.translation;
    glm::vec2 unit_direction = glm::normalize(direction);
    double perp_x = -unit_direction.y;
    double perp_y = unit_direction.x;
    int distance = 15;

    glm::vec2 movePosition = glm::vec2(distance * perp_x, distance * perp_y);

    if (upgradePath == 1 && level == 3) {
        // 第一條射線（中間）
        std::shared_ptr<Attack> attack = std::make_shared<Superlight>(GetPosition(), goalPosition, GetAttributes());
   
        attacks.push_back(attack);
        
        // 第二條射線（左側）
        attack = std::make_shared<Superlight>(GetPosition()+movePosition, goalPosition+movePosition, GetAttributes());
 
        attacks.push_back(attack);
        
        // 第三條射線（右側）
        attack = std::make_shared<Superlight>(GetPosition()-movePosition, goalPosition-movePosition, GetAttributes());

        attacks.push_back(attack);
    } else if (upgradePath == 1 && level == 4) {
        // 第一條射線（中間）
        std::shared_ptr<Attack> attack = std::make_shared<Superlight>(GetPosition(), goalPosition, GetAttributes());
        attack -> SetScale(glm::vec2(1.5, 1.5));
        attack -> SetTouchScale(glm::vec2(1.5, 1.5));
        attacks.push_back(attack);
    } else {
        // 原有的兩條射線攻擊模式
        std::shared_ptr<Attack> attack = std::make_shared<Ray>(GetPosition()+movePosition, goalPosition+movePosition, GetAttributes());
        attacks.push_back(attack);
        attack = std::make_shared<Ray>(GetPosition()-movePosition, goalPosition-movePosition, GetAttributes());
        attacks.push_back(attack);
    }
    
    return attacks;
}

void SuperMonkey::UpdateLevel(){
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    if (upgradePath == 1){
        switch (level) {
            case 1:
                attributes -> SetPenetration(attributes -> GetPenetration()*2);
                attributes -> AddProperty(4);
                break;
            case 2:
                SetCd(GetCd()/2);
                ResetCount();
                break;
            case 3:
                attributes -> SetPenetration(attributes -> GetPenetration()*3);
                break;
            case 4:
                SetCd(1);
                attributes -> SetSpeed(20);
                ResetCount();
                break;
        }
    }else{
        switch (level) {
            case 1:
                SetRadius(GetRadius()*1.5);
                UpdateRange();
                break;
            case 2:
                SetRadius(GetRadius()*1.5);
                UpdateRange();
                break;
            case 3:
                attributes -> SetPenetration(attributes -> GetPenetration()*2);
                break;
        }
    }

}

//########################################################################

IceMonkey::IceMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/IceMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(0);
    attributes -> SetPower(0);
    attributes -> SetSpeed(0);
    attributes -> AddDebuff({0, 5});

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<IceMonkeyInformationBoard>();

    SetCost(380);
    SetCd(4);
    SetRadius(120);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> IceMonkey::ProduceAttack(glm::vec2 goalPosition) {
    SetRotation(goalPosition);
    ResetCount();
    std::vector<std::shared_ptr<Attack>> attacks;

    std::shared_ptr<Attack> attack = std::make_shared<Blizzard>(GetPosition(), goalPosition, GetAttributes(), GetRadius());
    attacks.push_back(attack);
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    if (upgradePath == 1) { 
        switch (level) {
            case 4:
            case 3:
                attack -> SetScale(glm::vec2(1.3, 1.3));
                attack -> SetTouchScale(glm::vec2(1.3, 1.3));
            case 2:{
                int random = rand() % 100;
                if (random < 20) {
                    attack -> SetPower(1);
                    attack -> SetImage(GA_RESOURCE_DIR"/Attack/Blizzarddamage.png");
                }
            }
            case 1:
                attack -> SetScale(glm::vec2(1.5, 1.5));
                attack -> SetTouchScale(glm::vec2(1.5, 1.5));
                break;
        }
    }
    return attacks;
}
void IceMonkey::UpdateLevel(){
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                SetRadius(GetRadius()*1.5);
                UpdateRange();
                attributes -> AddDebuff({0, 2});
                break;
            case 3:
                SetRadius(GetRadius()*1.3);
                UpdateRange();
                attributes -> AddDebuff({0, 3});
            case 4:
                attributes -> AddDebuff({0, 3});
                break;
        }
    }
    else if (upgradePath == 2) {
        switch (level) {
            case 1:
                attributes -> AddDebuff({9, 1});
                break;
            case 2:
                attributes -> AddDebuff({10, 1000});
                break;
            case 3:
                attributes -> AddDebuff({7, 5});
                break;
            case 4:
                break;
        }
    }
}

//########################################################################

RubberMonkey::RubberMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/RubberMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(1);
    attributes -> SetPower(0);
    attributes -> SetSpeed(50);
    attributes -> AddDebuff({2, 100});

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<RubberMonkeyInformationBoard>();

    SetCost(300);
    SetCd(60);
    SetRadius(120);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> RubberMonkey::ProduceAttack(glm::vec2 goalPosition) {
    SetRotation(goalPosition);
    ResetCount();
    std::vector<std::shared_ptr<Attack>> attacks;

    std::shared_ptr<Attack> attack = std::make_shared<Rubber>(GetPosition(), goalPosition, GetAttributes());
    attacks.push_back(attack);
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    if (upgradePath == 1) {
        switch (level) {
            case 4:
                // 創建爆炸效果並關聯到Rubber攻擊
                std::shared_ptr<Attack> attack2 = std::make_shared<Explosion>(GetPosition(), goalPosition, attack, GetAttributes());
                attack2 -> SetVisible(false); // 初始不可見，等Rubber命中時才顯示
                // attack2 -> SetImage(GA_RESOURCE_DIR"/Attack/Rubber.png");
                attacks.push_back(attack2);
                break;
        }
    }else if (upgradePath == 2){
        switch (level) {
            case 2:{
                // 創建扇形攻擊（左右各15度的兩發，加上中間一發，共三發）
                double angles[] = {-30, -15, 15, 30}; // 扇形角度，可調整
                
                for (double angle : angles) {
                    double radians = angle * PI / 180.0;  // 角度轉為弧度
                    double cosA = cos(radians);
                    double sinA = sin(radians);
                    glm::vec2 center = m_Transform.translation;
                    
                    // 旋轉目標點以獲得新的方向
                    double x = center.x + (goalPosition.x - center.x) * cosA - (goalPosition.y - center.y) * sinA;
                    double y = center.y + (goalPosition.x - center.x) * sinA + (goalPosition.y - center.y) * cosA;
                    
                    // 創建額外的Rubber攻擊
                    std::shared_ptr<Attack> extraAttack = std::make_shared<Rubber>(
                        GetPosition(), 
                        glm::vec2(x, y), 
                        GetAttributes()
                    );
                    attacks.push_back(extraAttack);
                }
            }
        }
    }
    return attacks;
}

void RubberMonkey::UpdateLevel(){
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                attributes -> AddDebuff({11, 300});
                break;
            case 2:
                attributes -> AddDebuff({12, 300});
                break;
            case 3:
                attributes -> AddDebuff({13, 300});
                break;
            case 4:
                attributes -> AddDebuff({12, 300});
                break;
        }
    }else if (upgradePath == 2){
        switch (level) {
            case 1:
                attributes -> AddDebuff({2, 100});
                break;
            case 3:
                SetCd(30);
                break;
                
        }
    }
}

//########################################################################

MagicMonkey::MagicMonkey(glm::vec2 position) : Monkey(position){
    m_Transform.scale = glm::vec2(1.0f, 1.0f);
    SetImage(GA_RESOURCE_DIR"/Monkey/MagicMonkey.png");
    SetSize(glm::vec2(50.0f, 50.0f));

    auto attributes = GetAttributes();
    attributes -> SetPenetration(2);
    attributes -> SetPower(2);
    attributes -> SetSpeed(80);

    auto &informationBoard = GetInfortionBoard();
    informationBoard = std::make_shared<MagicMonkeyInformationBoard>();

    SetCost(550);
    SetCd(60);
    SetRadius(120);
    UpdateRange();
}

std::vector<std::shared_ptr<Attack>> MagicMonkey::ProduceAttack(glm::vec2 goalPosition) {
    SetRotation(goalPosition);
    std::vector<std::shared_ptr<Attack>> attacks;
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    if (GetCount() == GetCd()) { // basic attack
        ResetCount();
        if (upgradePath == 1 && level > 0) {
            std::shared_ptr<Attack> attack = std::make_shared<MagicBall>(GetPosition(), goalPosition, GetAttributes());
            attack -> SetScale(glm::vec2(10, 10));
            attack -> SetTouchScale(glm::vec2(10, 10));
            attacks.push_back(attack);
        }
        else{
            std::shared_ptr<Attack> attack = std::make_shared<MagicBall>(GetPosition(), goalPosition, GetAttributes());
            attacks.push_back(attack);
        }
    }
    if (upgradePath == 1){
        if (level > 1){
            if (otheratk_cd[0] == 0) { // 0 for light
                otheratk_cd[0] = 150;
                std::shared_ptr<Attack> attack = std::make_shared<MagicBall>(GetPosition(), goalPosition, GetAttributes());
                attack -> SetImage(GA_RESOURCE_DIR"/Attack/light.png");
                attack -> GetAttributes() -> SetPenetration(1);
                attacks.push_back(attack);
                std::shared_ptr<Attack> attack2 = std::make_shared<lightExplosion>(attack);
                attacks.push_back(attack2); 
            }
        }
        if (level > 2){
            if (otheratk_cd[1] == 0) { // 1 for wind
                otheratk_cd[1] = 150;
                std::shared_ptr<Attack> attack = std::make_shared<WindAttack>(GetPosition(), goalPosition, GetAttributes());
                attack -> SetPower(0);
                attack -> SetSpeed(20);
                attack -> SetPenetration(30);
                if (level > 3){
                    attack -> SetSpeed(40);
                    attack -> SetPower(1);
                    attack -> SetPenetration(100);
                }
                attacks.push_back(attack);
            }
        }
    }
    if (upgradePath == 2){
        if (level > 0){
            if (otheratk_cd[0] == 0) { // 0 for fireball
                otheratk_cd[0] = 120;
                std::shared_ptr<Attack> attack = std::make_shared<Dart>(GetPosition(), goalPosition, GetAttributes());
                attack -> SetPenetration(30);
                attack -> SetImage(GA_RESOURCE_DIR"/Attack/fireball.png");
                attack -> SetHeight(50);
                attack -> SetWidth(100);
                attack -> SetSpeed(20);
                attacks.push_back(attack);
            }
        }
        if (level > 2){
            if (otheratk_cd[1] == 0) { // 1 for fireball
                otheratk_cd[1] = 20;
                // 創建扇形攻擊（左右各15度的兩發，加上中間一發，共三發）
                double angles[] = {-40, -30, -20, -10, 0, 10, 20, 30, 40}; // 扇形角度，每10度間隔，射出九發
                    
                for (double angle : angles) {
                    double radians = angle * PI / 180.0;  // 角度轉為弧度
                    double cosA = cos(radians);
                    double sinA = sin(radians);
                    glm::vec2 center = m_Transform.translation;
                    
                    // 旋轉目標點以獲得新的方向
                    double x = center.x + (goalPosition.x - center.x) * cosA - (goalPosition.y - center.y) * sinA;
                    double y = center.y + (goalPosition.x - center.x) * sinA + (goalPosition.y - center.y) * cosA;
                    
                    // 創建額外的Rubber攻擊
                    std::shared_ptr<Attack> extraAttack = std::make_shared<Dart>(
                        GetPosition(), 
                        glm::vec2(x, y), 
                        GetAttributes()
                    );
                    extraAttack -> SetHeight(80);
                    extraAttack -> SetWidth(80);
                    extraAttack -> SetSpeed(40);
                    extraAttack -> SetPenetration(1);
                    extraAttack -> SetImage(GA_RESOURCE_DIR"/Attack/dragonfire.png");
                    attacks.push_back(extraAttack);
                }
            }
        }
    }
    //left area is for extra attack
    
    return attacks;
}

bool MagicMonkey::otheratk(){
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    if (upgradePath == 1 ){
        if (level > 1){
            if (otheratk_cd[0] == 0) { // 0 for light
                return true;
            }
            otheratk_cd[0]--;
        }
        if (level > 2){
            if (otheratk_cd[1] == 0) { // 1 for wind
                return true;
            }
            otheratk_cd[1]--;
        }
    }
    if (upgradePath == 2){
        if (level > 0){
            if (otheratk_cd[0] == 0) { // 0 for fireball
                return true;
            }
            otheratk_cd[0]--;
        }
        if (level > 2){
            if (otheratk_cd[1] == 0) { // 1 for fireball
                return true;
            }
            otheratk_cd[1]--;
        }
    }
    return false;
}


void MagicMonkey::UpdateLevel(){
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                attributes -> SetPenetration(7);
                attributes -> SetPower(attributes -> GetPower()*1.5);
                break;
            case 2:
                // attributes -> SetPenetration(2);
                break;
        }
    }else{
        switch (level) {
            case 2:
                attributes -> AddProperty(2);
                break;
        }
    }
}
