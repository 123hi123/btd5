#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <vector>
#include <string>
#include "CounterText.hpp"
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class Counter : public Util::GameObject {
public:
  explicit Counter(int current, int maxValue);

  void SetPosition(const glm::vec2& position);
  void SetImage(const std::string& ImagePath);
  void AddValue(int value);
  void MinusValue(int value);
  void UpdateText();
  void SetTextPosition(glm::vec2 position);
  [[nodiscard]] std::shared_ptr<CounterText> GetCounterText(){ return m_CounterText; }
  [[nodiscard]] int GetCurrent(){ return m_Current; }
  [[nodiscard]] int GetMaxValue(){ return m_MaxValue; }

private:
  int m_MaxValue = 0;
  int m_Current = 0;
  std::shared_ptr<CounterText> m_CounterText = std::make_shared<CounterText>();
  std::string m_ImagePath;

};

#endif //COUNTER_HPP

class Heart : public Counter {
public:
  explicit Heart(int current, int maxValue);
};

class Money : public Counter {
public:
  explicit Money(int current, int maxValue);
};

class Round : public Counter {
public:
  explicit Round(int current, int maxValue);
};