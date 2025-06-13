#include <vector>
#include <string>

#ifndef ATTRIBUTES_HPP
#define ATTRIBUTES_HPP

class Attributes {
public:
  void SetPenetration(int penetration){ m_Penetration = penetration;}
  void SetPower(int power){ m_Power = power;}
  void SetSpeed(int speed){ m_Speed = speed;}
  void AddProperty(int property){ m_Properties.push_back(property);}
  void AddDebuff(std::vector<int> debuff){ m_Debuff.push_back(debuff);}
  

  [[nodiscard]] int GetPenetration() const { return m_Penetration; }
  [[nodiscard]] int GetPower() const { return m_Power; }
  [[nodiscard]] float GetSpeed() const { return m_Speed; }
  [[nodiscard]] std::vector<std::vector<int>> GetDebuff() const { return m_Debuff; }
  [[nodiscard]] std::vector<int> GetProperties() const { return m_Properties; }
  //

private:
    int m_Penetration = 0;
    int m_Power = 0;
    float m_Speed = 0;
    std::vector<int> m_Properties = {};
    std::vector<std::vector<int>> m_Debuff = {};
};

#endif //ATTRIBUTES_HPP
