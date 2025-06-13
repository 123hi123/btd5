
#pragma once

#include "Util/Input.hpp"
#include <memory>
#include <string>
#include "Util/GameObject.hpp"
#include "Monkey.hpp"

class DragButton : public Util::GameObject {

public:
    DragButton(const glm::vec2& position);
    void Update();
    void SetImage(const std::string& ImagePath);
    // 设置原始图片路径和灰色图片路径
    void SetOriginImage(const std::string& ImagePath);
    void SetPosition(const glm::vec2& position);
    // 设置和获取按钮的尺寸
    void SetSize(const glm::vec2& size) { m_Size = size; }
    const glm::vec2& GetSize() const { return m_Size; }
    
    // 设置图片旋转角度（0-360度）
    void SetAngle(int degrees);
    // 获取当前角度（0-360度）
    int GetAngle() const;
    
    // 检查点是否在按钮范围内
    bool IsPointInside(const glm::vec2& point) const;
    [[nodiscard]] virtual std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition);
    
    // 更新按钮状态，根据金钱是否足够购买
    virtual void UpdateButtonState(int money);
    // 获取按钮是否可购买
    bool IsBuyable() const { return m_Buyable; }
    
protected:
    std::string m_ImagePath; // 存储原始图片路径
    std::string m_grayImagePath; // 存储灰色图片路径
    bool m_Buyable{true}; // 按钮是否可购买
    
private:
    glm::vec2 m_Size{100.0f, 100.0f}; // 默认尺寸
    float m_HoverScale{1.1f};  // 新增：悬停时的放大倍数
    float m_HoverAnimTime{0.0f};  // 新增：悬停动画时间
    glm::vec2 m_OriginalSize{100.0f, 100.0f};  // 新增：原始尺寸
}; 


class DartMonkeyButton : public DragButton {
public:
      explicit DartMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};

class NailMonkeyButton : public DragButton {
public:
      explicit NailMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};

class SniperMonkeyButton : public DragButton {
public:
      explicit SniperMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};

class BoomerangMonkeyButton : public DragButton {
public:
      explicit BoomerangMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};

class NinjaMonkeyButton : public DragButton {
public:
      explicit NinjaMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};
class CannonButton : public DragButton {
public:
      explicit CannonButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};
class AirportButton : public DragButton {
public:
      explicit AirportButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};
class BuccaneerMonkeyButton : public DragButton {
public:
      explicit BuccaneerMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};

class SuperMonkeyButton : public DragButton {
public:
      explicit SuperMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};

class IceMonkeyButton : public DragButton {
public:
      explicit IceMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};

class RubberMonkeyButton : public DragButton {
public:
      explicit RubberMonkeyButton(glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};

class MagicMonkeyButton : public DragButton {
public:
      explicit MagicMonkeyButton (glm::vec2 position);
      [[nodiscard]] std::shared_ptr<Monkey> ProduceMonkey(glm::vec2 goalPosition) override;
      void UpdateButtonState(int money) override;
};