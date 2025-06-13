#include "DragButton.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"

#include <cmath>

DragButton::DragButton(const glm::vec2& position)
{
    m_Transform.translation = position;
    m_OriginalSize = m_Size; // 保存原始尺寸
}

bool DragButton::IsPointInside(const glm::vec2& point) const {
    // LOG_DEBUG("Point position: " + std::to_string(point.x) + ", " + std::to_string(point.y));
    
    float leftBound = m_Transform.translation.x - m_Size.x / 2.0f;
    float rightBound = m_Transform.translation.x + m_Size.x / 2.0f;
    float topBound = m_Transform.translation.y - m_Size.y / 2.0f;
    float bottomBound = m_Transform.translation.y + m_Size.y / 2.0f;
    
    // LOG_DEBUG("Button bounds: Left(" + std::to_string(leftBound) +
    //           "), Right(" + std::to_string(rightBound) +
    //           "), Top(" + std::to_string(topBound) +
    //           "), Bottom(" + std::to_string(bottomBound) + ")");
    //
    // LOG_DEBUG("Button position: " + std::to_string(m_Position.x) + ", " + std::to_string(m_Position.y));
    // LOG_DEBUG("Button size: " + std::to_string(m_Size.x) + ", " + std::to_string(m_Size.y));
    
    return (point.x >= leftBound &&
            point.x <= rightBound &&
            point.y >= topBound &&
            point.y <= bottomBound);
}
void DragButton::SetPosition(const glm::vec2 &position) {
    m_Transform.translation = position;
}

void DragButton::SetAngle(int degrees) {
    // 确保角度在0-360范围内
    degrees = degrees % 360;
    if (degrees < 0) degrees += 360;
    
    // 将角度转换为弧度（GameObject的rotation使用弧度）
    float radians = degrees * (M_PI / 180.0f);
    m_Transform.rotation = radians;
}

int DragButton::GetAngle() const {
    // 将弧度转换为角度
    float degrees = m_Transform.rotation * (180.0f / M_PI);
    
    // 确保角度在0-360范围内
    int intDegrees = static_cast<int>(round(degrees)) % 360;
    if (intDegrees < 0) intDegrees += 360;
    
    return intDegrees;
}

void DragButton::Update() {
    // 获取鼠标位置
    glm::vec2 mousePosition = Util::Input::GetCursorPosition();
    
    // 检查鼠标是否悬停在按钮上
    bool isInside = IsPointInside(mousePosition);
    
    // 更新悬停动画时间
    if (isInside) {
        m_HoverAnimTime += 0.016f; // 假设每帧16ms
    } else {
        m_HoverAnimTime = 0.0f;
    }
    
    // 应用悬停效果 - 大小变化
    if (isInside) {
        // 使用正弦函数实现轻微的上下浮动效果
        float scale = 1.0f + (m_HoverScale - 1.0f) * 0.5f * (1.0f + sinf(m_HoverAnimTime * 5.0f));
        m_Transform.scale = glm::vec3(scale, scale, 1.0f);
    } else {
        m_Transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    }
}


void DragButton::SetImage(const std::string& ImagePath) {
    m_Drawable = std::make_shared<Util::Image>(ImagePath);
}
void DragButton::SetOriginImage(const std::string& ImagePath) {
    // 设置原始图片路径
    m_ImagePath = ImagePath;
    
    // 构建灰色图片路径 - 将文件名和扩展名分开，在文件名后添加_gray
    size_t lastDot = ImagePath.find_last_of('.');
    if (lastDot != std::string::npos) {
        std::string baseName = ImagePath.substr(0, lastDot);
        std::string extension = ImagePath.substr(lastDot);
        m_grayImagePath = baseName + "_gray" + extension;
    } else {
        m_grayImagePath = ImagePath + "_gray";
    }
    
    // 使用原始图片初始化按钮
    SetImage(m_ImagePath);
}
void DragButton::UpdateButtonState(int money) {
    // 基类默认实现，子类会根据各自价格覆盖此方法
}

std::shared_ptr<Monkey> DragButton::ProduceMonkey(glm::vec2 handPosition){
    return nullptr;
}
DartMonkeyButton::DartMonkeyButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/DartMonkey.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> DartMonkeyButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<DartMonkey>(handPosition);
    return monkey;
}

void DartMonkeyButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 200) { // DartMonkey价格是200
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}

// 新增按钮类实现

NailMonkeyButton::NailMonkeyButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/NailMonkey.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> NailMonkeyButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<NailMonkey>(handPosition);
    return monkey;
}

void NailMonkeyButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 360) { // NailMonkey价格是360
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}

SniperMonkeyButton::SniperMonkeyButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/SniperMonkey.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> SniperMonkeyButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<SniperMonkey>(handPosition);
    return monkey;
}

void SniperMonkeyButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 400) { // SniperMonkey价格是400
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}

BoomerangMonkeyButton::BoomerangMonkeyButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/BoomerangMonkey.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> BoomerangMonkeyButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<BoomerangMonkey>(handPosition);
    return monkey;
}

void BoomerangMonkeyButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 400) { // BoomerangMonkey价格是400
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}

NinjaMonkeyButton::NinjaMonkeyButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/NinjaMonkey.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> NinjaMonkeyButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<NinjaMonkey>(handPosition);
    return monkey;
}

void NinjaMonkeyButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 600) { // NinjaMonkey价格是600
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}

CannonButton::CannonButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/Cannon.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> CannonButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<Cannon>(handPosition);
    return monkey;  
}

void CannonButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 700) { // Cannon价格是700
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}

AirportButton::AirportButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/AirplaneMonkey.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> AirportButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<Airport>(handPosition);
    return monkey;
}

void AirportButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 950) { // Airport价格是950
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}


BuccaneerMonkeyButton::BuccaneerMonkeyButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/BuccaneerMonkey.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> BuccaneerMonkeyButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<BuccaneerMonkey>(handPosition);
    return monkey;
}

void BuccaneerMonkeyButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 600) { // BuccaneerMonkey价格是600
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}

SuperMonkeyButton::SuperMonkeyButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/SuperMonkey.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> SuperMonkeyButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<SuperMonkey>(handPosition);
    return monkey;
}

void SuperMonkeyButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 4000) { // SuperMonkey价格是4000
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}

IceMonkeyButton::IceMonkeyButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/IceMonkey.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> IceMonkeyButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<IceMonkey>(handPosition);
    return monkey;
}

void IceMonkeyButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 380) { // IceMonkey价格是380
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}

RubberMonkeyButton::RubberMonkeyButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/RubberMonkey.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> RubberMonkeyButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<RubberMonkey>(handPosition);
    return monkey;
}

void RubberMonkeyButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 300) { // RubberMonkey价格是300
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}

MagicMonkeyButton::MagicMonkeyButton(glm::vec2 position) : DragButton(position){
    SetOriginImage(GA_RESOURCE_DIR"/Monkey/MagicMonkey.png");
    SetAngle(90);
}
std::shared_ptr<Monkey> MagicMonkeyButton::ProduceMonkey(glm::vec2 handPosition){
    if (!m_Buyable) {
        return nullptr;
    }
    std::shared_ptr<Monkey> monkey = std::make_shared<MagicMonkey>(handPosition);
    return monkey;
}

void MagicMonkeyButton::UpdateButtonState(int money) {
    bool previousBuyable = m_Buyable;
    
    if (money < 550) { // MagicMonkey价格是550
        m_Buyable = false;
    } else {
        m_Buyable = true;
    }
    
    // 只有当状态改变时才更新图像
    if (previousBuyable != m_Buyable) {
        if (m_Buyable) {
            SetImage(m_ImagePath);
        } else {
            SetImage(m_grayImagePath);
        }
    }
}