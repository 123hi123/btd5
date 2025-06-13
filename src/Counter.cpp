#include "Counter.hpp"
#include <cmath>

#include "Util/Logger.hpp"


Counter::Counter(int current, int maxValue)
{
  m_Transform.scale = glm::vec2(1.5,1.5);
  m_MaxValue = maxValue;
  m_Current = current;
  m_CounterText -> Update(current, maxValue);
}

void Counter::SetPosition(const glm::vec2& Position){
  m_Transform.translation = Position;
}

void Counter::SetImage(const std::string& ImagePath) {
  m_ImagePath = ImagePath;
  m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

void Counter::AddValue(int value) {
  m_Current = fmin(m_Current + value, m_MaxValue);
  UpdateText();
}

void Counter::MinusValue(int value) {
  m_Current = fmax(m_Current - value, 0);
  UpdateText();
}

void Counter::UpdateText() {
  m_CounterText ->Update(m_Current, m_MaxValue);
}

void Counter::SetTextPosition(glm::vec2 position) {
  m_CounterText -> SetPosition(position);
}


// #########################################################

Heart::Heart(int current, int maxValue) : Counter(current, maxValue){
  SetImage(GA_RESOURCE_DIR"/Icon/Heart.png");
  SetPosition(glm::vec2(-600, 330));
  SetTextPosition(glm::vec2(-500, 330));
}

// #########################################################

Money::Money(int current, int maxValue) : Counter(current, maxValue){
  SetImage(GA_RESOURCE_DIR"/Icon/Money.png");
  SetPosition(glm::vec2(-600, 290));
  SetTextPosition(glm::vec2(-460, 290));
}

//##########################################################

Round::Round(int current, int maxValue) : Counter(current, maxValue){
  SetImage(GA_RESOURCE_DIR"/Icon/Round.png");
  SetPosition(glm::vec2(-530, -330));
  SetTextPosition(glm::vec2(-400, -330));
}