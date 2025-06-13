#ifndef INFORMATIONBOARD_HPP
#define INFORMATIONBOARD_HPP

#include <vector>
#include <string>
#include <memory>
#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "InformationImage.hpp"
#include "InformationText.hpp"
#include "InformationButton.hpp"

class InformationBoard : public Util::GameObject {
public:

    explicit InformationBoard();

    void SetPosition(const glm::vec2& Position);
    void SetImage(const std::string& ImagePath);
    void SetUpgrateName(std::string name);
    void UpdateAllObjectVisible(bool isClicked);
    void UpdateUpgrade();
    void SetUpgradeCost(std::vector<std::vector<int>> upgradeCost);
    void SetUpgradeText(std::vector<std::vector<std::string>> upgradeText);
    void IsButtonTouch(glm::vec2 mousePosition);
    void SkillCountDown();
    void SetSkillEffect(bool b);

    [[nodiscard]] std::vector<int> IsClick(glm::vec2 mousePosition, int money);
    [[nodiscard]] int GetLevel() { return level;}
    [[nodiscard]] int GetUpgradePath() { return upgradePath;}

    [[nodiscard]] std::vector<std::shared_ptr<GameObject>> GetAllChildren();
    [[nodiscard]] std::shared_ptr<InformationImage> GetMonkeyImage(){ return monkey_image;}
    [[nodiscard]] std::shared_ptr<InformationImage> GetUpgrateImage1(){ return upgrate_image1;}
    [[nodiscard]] std::shared_ptr<InformationImage> GetUpgrateImage2(){ return upgrate_image2;}
    [[nodiscard]] std::shared_ptr<InformationText> GetUpgrateText1(){ return upgrate_text1;}
    [[nodiscard]] std::shared_ptr<InformationText> GetUpgrateText2(){ return upgrate_text2;}
    [[nodiscard]] std::shared_ptr<InformationButton> GetUpgrateButton1(){ return upgrate_button1;}
    [[nodiscard]] std::shared_ptr<InformationButton> GetUpgrateButton2(){ return upgrate_button2;}

private:
    int level = 0;
    int upgradePath = 0;
    int skillCd = 1800; // TODO: 記得換回 1800
    int skillCount = 0;
    bool skillEffect = false;
    std::vector<std::vector<int>> m_upgradeCost = {};
    std::vector<std::vector<std::string>> m_upgradeText = {};
    std::string upgrateName = GA_RESOURCE_DIR"//MonkeyInformation/DartMonkey";
    std::string m_ImagePath;
    std::shared_ptr<InformationImage> monkey_image = std::make_shared<InformationImage>(GA_RESOURCE_DIR"/MonkeyInformation/WithoutImage.png", glm::vec2(-500, 155));
    std::shared_ptr<InformationImage> upgrate_image1 = std::make_shared<InformationImage>(GA_RESOURCE_DIR"/MonkeyInformation/Lock.png", glm::vec2(-530, 0));
    std::shared_ptr<InformationImage> upgrate_image2 = std::make_shared<InformationImage>(GA_RESOURCE_DIR"/MonkeyInformation/Lock.png", glm::vec2(-530, -125));
    std::shared_ptr<InformationText> upgrate_text1 = std::make_shared<InformationText>(glm::vec2(-360, 25 ), "00000000", 20);
    std::shared_ptr<InformationText> upgrate_text2 = std::make_shared<InformationText>(glm::vec2(-360, -100 ), "00000000", 20);
    std::shared_ptr<InformationButton> upgrate_button1 = std::make_shared<InformationButton>(glm::vec2(-370, -35 ), "0");
    std::shared_ptr<InformationButton> upgrate_button2 = std::make_shared<InformationButton>(glm::vec2(-370, -160 ), "0");
    std::shared_ptr<InformationButton> sale_button = std::make_shared<InformationButton>(glm::vec2(-365, -220 ), "Sale");
    std::shared_ptr<InformationButton> skill_button = std::make_shared<InformationButton>(glm::vec2(-530, -220 ), "Skill");
};


#endif //INFORMATIONBOARD_HPP

class DartMonkeyInformationBoard : public InformationBoard {
public:
    explicit DartMonkeyInformationBoard();
};

class NailMonkeyInformationBoard : public InformationBoard {
public:
    explicit NailMonkeyInformationBoard();
};

class SniperMonkeyInformationBoard : public InformationBoard {
public:
    explicit SniperMonkeyInformationBoard();
};

class BoomerangMonkeyInformationBoard : public InformationBoard {
public:
    explicit BoomerangMonkeyInformationBoard();
};

class NinjaMonkeyInformationBoard : public InformationBoard {
public:
    explicit NinjaMonkeyInformationBoard();
};

class CannonInformationBoard : public InformationBoard {
public:
    explicit CannonInformationBoard();
};

class IceMonkeyInformationBoard : public InformationBoard {
public:
    explicit IceMonkeyInformationBoard();
};

class RubberMonkeyInformationBoard : public InformationBoard {
public:
    explicit RubberMonkeyInformationBoard();
};

class BuccaneerMonkeyInformationBoard : public InformationBoard {
public:
    explicit BuccaneerMonkeyInformationBoard();
};

class AirportInformationBoard : public InformationBoard {
public:
    explicit AirportInformationBoard();
};

class SuperMonkeyInformationBoard : public InformationBoard {
public:
    explicit SuperMonkeyInformationBoard();
};

class MagicMonkeyInformationBoard : public InformationBoard {
public:
    explicit MagicMonkeyInformationBoard();
};



