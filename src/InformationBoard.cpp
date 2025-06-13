#include "InformationBoard.hpp"
#include "Util/Input.hpp"
#include "Util/Logger.hpp"

InformationBoard::InformationBoard(){
    SetImage(GA_RESOURCE_DIR"/MonkeyInformation/InformationBoard_test.png");
    SetPosition(glm::vec2(-450, 0));
    SetVisible(false);
    SetZIndex(5);
 };

void InformationBoard::SetPosition(const glm::vec2& Position) {
    m_Transform.translation = Position;
}

void InformationBoard::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

std::vector<std::shared_ptr<Util::GameObject>> InformationBoard::GetAllChildren(){
    std::vector<std::shared_ptr<Util::GameObject>> children =
        {monkey_image, upgrate_image1, upgrate_image2, upgrate_text1, upgrate_text2, upgrate_button1, upgrate_button2, sale_button, skill_button};
    std::vector<std::shared_ptr<Util::GameObject>> tmp;
    tmp = upgrate_button1->GetAllChildren();
    children.insert(children.end(), tmp.begin(), tmp.end());
    tmp = upgrate_button2->GetAllChildren();
    children.insert(children.end(), tmp.begin(), tmp.end());
    tmp = sale_button->GetAllChildren();
    children.insert(children.end(), tmp.begin(), tmp.end());
    tmp = skill_button->GetAllChildren();
    children.insert(children.end(), tmp.begin(), tmp.end());
    return children;
};

std::vector<int> InformationBoard::IsClick(glm::vec2 mousePosition, int money) {
    if (skillEffect) {
        return {0};
    }
    if (mousePosition.x > -625 && mousePosition.x < -275 && mousePosition.y > -250 && mousePosition.y < 250) {
        //關閉選取
        if (mousePosition.x > -307 && mousePosition.x < -279 && mousePosition.y > 221 && mousePosition.y < 247) {
            return {2};
        }
        if (mousePosition.x > -442 && mousePosition.x < -292 && mousePosition.y > -238 && mousePosition.y < -197) {
            return {3};
        }
        if (level < 4 && money >= m_upgradeCost[0][level] && upgradePath != 2 && mousePosition.x > -443 && mousePosition.x < -294 && mousePosition.y > -55 && mousePosition.y < -14) {
            level += 1;
            upgradePath = 1;
            UpdateUpgrade();
            return {5, m_upgradeCost[0][level-1]};
        }
        if (level < 4 && money >= m_upgradeCost[1][level] && upgradePath != 1 && mousePosition.x > -444 && mousePosition.x < -297 && mousePosition.y > -184 && mousePosition.y < -141) {
            level += 1;
            upgradePath = 2;
            UpdateUpgrade();
            return {6, m_upgradeCost[1][level-1]};
        }
        if (upgradePath == 2 && level == 4 && skillCount == 0 && mousePosition.x > -605 && mousePosition.x < -456 && mousePosition.y > -239 && mousePosition.y < -203) {
            skillCount = skillCd;
            return {4};
        }
        return {1};
    }
    return {0};
}

void InformationBoard::SetUpgradeCost(std::vector<std::vector<int>> upgradeCost) {
    m_upgradeCost = upgradeCost;
}

void InformationBoard::SetUpgradeText(std::vector<std::vector<std::string>> upgradeText) {
    m_upgradeText = upgradeText;
}

void InformationBoard::SetUpgrateName(std::string name) {
    upgrateName = name;
}

void InformationBoard::UpdateAllObjectVisible(bool isClicked) {
    SetVisible(isClicked && !skillEffect);
    monkey_image -> SetVisible(isClicked && !skillEffect);
    upgrate_image1 -> SetVisible(isClicked && !skillEffect);
    upgrate_image2 -> SetVisible(isClicked && !skillEffect);
    upgrate_text1 -> SetVisible(isClicked && !skillEffect);
    upgrate_text2 -> SetVisible(isClicked && !skillEffect);
    upgrate_button1 -> UpdateVisible(isClicked && !skillEffect);
    upgrate_button2 -> UpdateVisible(isClicked && !skillEffect);
    sale_button -> UpdateVisible(isClicked && !skillEffect);
    if (upgradePath == 2 and level == 4) {
        skill_button -> UpdateVisible(isClicked && !skillEffect);
    }
}

void InformationBoard::SetSkillEffect(bool b) {
    skillEffect = b;
    if (b) {
        UpdateAllObjectVisible(!b);
    }
}

void InformationBoard::UpdateUpgrade() {
    if (level == 4) {
        if (upgradePath == 1){upgrate_text1 -> Update("Level Max");}
        else {
            skill_button -> UpdateVisible(true);
            upgrate_text2 -> Update("Level Max");
        }
        upgrate_image1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Lock.png");
        upgrate_image2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Lock.png");
        upgrate_button1 -> Update("None");
        upgrate_button2 -> Update("None");
    }
    else if (upgradePath == 1) {
        upgrate_image1 -> SetImage(upgrateName + "_1_" + std::to_string(level+1) + ".png");
        upgrate_image2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Lock.png");
        upgrate_text1 -> Update(m_upgradeText[0][level]);
        upgrate_text2 -> Update("next path");
        upgrate_button1 -> Update(std::to_string(m_upgradeCost[0][level]));
        upgrate_button2 -> Update("None");
    }
    else {
        upgrate_image2 -> SetImage(upgrateName + "_2_" + std::to_string(level+1) + ".png");
        upgrate_image1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Lock.png");
        upgrate_text2 -> Update(m_upgradeText[1][level]);
        upgrate_text1 -> Update("next path");
        upgrate_button2 -> Update(std::to_string(m_upgradeCost[1][level]));
        upgrate_button1 -> Update("None");
    }
}

void InformationBoard::IsButtonTouch(glm::vec2 mousePosition) {
    upgrate_button1 -> IsTouch(mousePosition);
    upgrate_button2 -> IsTouch(mousePosition);
    sale_button -> IsTouch(mousePosition);
    skill_button -> IsTouch(mousePosition);
}

void InformationBoard::SkillCountDown() {
    if (skillCount != 0) {
        skillCount -= 1;
        skill_button -> Update(std::to_string(int(skillCount/60)));
    }
    else {
        skill_button -> Update("skill");
    }
}

DartMonkeyInformationBoard::DartMonkeyInformationBoard() {
    SetUpgradeCost({{90, 120, 500, 1500},
                       {140, 170, 330, 8000}});
    SetUpgradeText({{"Long Range Darts", "Enhanced Eyesight", "Spike-o-pult", "Juggernaut"},
                          {"Sharp Shots", "Razor Sharp Shots", "Triple Darts", "Super Monkey Fan Club"}});
    auto monkeyImage  = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/DartMonkeyImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/DartMonkey");
    auto upgradeImage1  = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/DartMonkey_1_1.png");
    auto upgradeImage2  = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/DartMonkey_2_1.png");
    auto upgradeText1  = GetUpgrateText1();
    upgradeText1 -> Update("Long Range Darts");
    auto upgradeText2  = GetUpgrateText2();
    upgradeText2 -> Update("Sharp Shots");
    auto upgradeButton1  = GetUpgrateButton1();
    upgradeButton1 -> Update("90");
    auto upgradeButton2  = GetUpgrateButton2();
    upgradeButton2 -> Update("140");
}

NailMonkeyInformationBoard::NailMonkeyInformationBoard() {
    // 使用360列的价格数据
    SetUpgradeCost({{210, 300, 500, 2500},
                    {100, 230, 680, 2700}});
    // 设置升级路径名称
    SetUpgradeText({{"Fast Shooting", "Even Faster Shooting", "Tack Shooter", "Ring of Fire"},
                    {"Extra Range Tacks", "Super Range Tacks", "Blade Shooter", "Blade Mealstrom"}});
    auto monkeyImage  = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/NailMonkeyImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/NailMonkey");
    auto upgradeImage1  = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/NailMonkey_1_1.png");
    auto upgradeImage2  = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/NailMonkey_2_1.png");
    auto upgradeText1  = GetUpgrateText1();
    upgradeText1 -> Update("Fast Shooting");
    auto upgradeText2  = GetUpgrateText2();
    upgradeText2 -> Update("Extra Range Tacks");
    auto upgradeButton1  = GetUpgrateButton1();
    upgradeButton1 -> Update("210");
    auto upgradeButton2  = GetUpgrateButton2();
    upgradeButton2 -> Update("100");
}

SniperMonkeyInformationBoard::SniperMonkeyInformationBoard() {
    // 使用400列的价格数据（中等难度）
    SetUpgradeCost({{350, 2200, 7500, 12000},
                    {400, 300, 3500, 12500}});
    // 设置升级路径名称
    SetUpgradeText({{"Full Metal Jacket", "Point Five Oh", "Deadly Precision", "Cripple MOAB"},
                    {"Faster Firing", "Night Vision Goggles", "Semi Automatic Rifle", "Supply Drop"}});
    auto monkeyImage = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/SniperMonkeyImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/SniperMonkey");
    auto upgradeImage1 = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/SniperMonkey_1_1.png");
    auto upgradeImage2 = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/SniperMonkey_2_1.png");
    auto upgradeText1 = GetUpgrateText1();
    upgradeText1 -> Update("Full Metal Jacket");
    auto upgradeText2 = GetUpgrateText2();
    upgradeText2 -> Update("Faster Firing");
    auto upgradeButton1 = GetUpgrateButton1();
    upgradeButton1 -> Update("350");
    auto upgradeButton2 = GetUpgrateButton2();
    upgradeButton2 -> Update("400");
}

BoomerangMonkeyInformationBoard::BoomerangMonkeyInformationBoard() {
    // 使用400列的价格数据（中等难度）
    SetUpgradeCost({{250, 280, 1100, 2500},
                    {100, 150, 1000, 3000}});
    // 设置升级路径名称
    SetUpgradeText({{"Multi-Target", "Glaive Thrower", "Glaive Riccochet", "Glaive Lord"},
                    {"Sonic Boom", "Red Hot Rangs", "Bionic Boomer", "Turbo Charge"}});
    auto monkeyImage = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/BoomerangMonkeyImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/BoomerangMonkey");
    auto upgradeImage1 = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/BoomerangMonkey_1_1.png");
    auto upgradeImage2 = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/BoomerangMonkey_2_1.png");
    auto upgradeText1 = GetUpgrateText1();
    upgradeText1 -> Update("Multi-Target");
    auto upgradeText2 = GetUpgrateText2();
    upgradeText2 -> Update("Sonic Boom");
    auto upgradeButton1 = GetUpgrateButton1();
    upgradeButton1 -> Update("250");
    auto upgradeButton2 = GetUpgrateButton2();
    upgradeButton2 -> Update("100");
}

NinjaMonkeyInformationBoard::NinjaMonkeyInformationBoard() {
    // 使用600列的价格数据（中等难度）
    SetUpgradeCost({{300, 350, 850, 2750},
                    {250, 350, 2750, 1800}});
    // 设置升级路径名称
    SetUpgradeText({{"Ninja Discipline", "Sharp Shurikens", "Double Shot", "Bloonjitsu"},
                    {"Seeking Shuriken", "Distraction", "Flash Bomb", "Sabotage Supply Lines"}});
    auto monkeyImage = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/NinjaMonkeyImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/NinjaMonkey");
    auto upgradeImage1 = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/NinjaMonkey_1_1.png");
    auto upgradeImage2 = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/NinjaMonkey_2_1.png");
    auto upgradeText1 = GetUpgrateText1();
    upgradeText1 -> Update("Ninja Discipline");
    auto upgradeText2 = GetUpgrateText2();
    upgradeText2 -> Update("Seeking Shuriken");
    auto upgradeButton1 = GetUpgrateButton1();
    upgradeButton1 -> Update("300");
    auto upgradeButton2 = GetUpgrateButton2();
    upgradeButton2 -> Update("250");
}

CannonInformationBoard::CannonInformationBoard() {
    // 使用400列的价格数据（中等难度）
    SetUpgradeCost({{200, 300, 800, 3200},
                    {400, 400, 900, 3200}});
    // 设置升级路径名称
    SetUpgradeText({{"Extra Range", "Frag Bombs", "Cluster Bombs", "Bloon Impact"},
                    {"Bigger Bombs", "Missile Launcher", "MOAB Mauler", "MOAB Assassin"}});
    auto monkeyImage = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/CannonImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/Cannon");
    auto upgradeImage1 = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Cannon_1_1.png");
    auto upgradeImage2 = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Cannon_2_1.png");
    auto upgradeText1 = GetUpgrateText1();
    upgradeText1 -> Update("Extra Range");
    auto upgradeText2 = GetUpgrateText2();
    upgradeText2 -> Update("Bigger Bombs");
    auto upgradeButton1 = GetUpgrateButton1();
    upgradeButton1 -> Update("200");
    auto upgradeButton2 = GetUpgrateButton2();
    upgradeButton2 -> Update("400");
}

IceMonkeyInformationBoard::IceMonkeyInformationBoard() {
    // 使用380列的价格数据（中等难度）
    SetUpgradeCost({{230, 400, 6500, 8000},
                    {100, 350, 2000, 2000}});
    // 设置升级路径名称
    SetUpgradeText({{"Enhanced Freeze", "Snap Freeze", "Arctic Wind", "Viral Frost"},
                    {"Permafrost", "Deep Freeze", "Ice Shards", "Absolute Zero"}});
    auto monkeyImage = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/IceMonkeyImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/IceMonkey");
    auto upgradeImage1 = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/IceMonkey_1_1.png");
    auto upgradeImage2 = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/IceMonkey_2_1.png");
    auto upgradeText1 = GetUpgrateText1();
    upgradeText1 -> Update("Enhanced Freeze");
    auto upgradeText2 = GetUpgrateText2();
    upgradeText2 -> Update("Permafrost");
    auto upgradeButton1 = GetUpgrateButton1();
    upgradeButton1 -> Update("230");
    auto upgradeButton2 = GetUpgrateButton2();
    upgradeButton2 -> Update("100");
}

RubberMonkeyInformationBoard::RubberMonkeyInformationBoard() {
    // 使用300列的价格数据（中等难度）
    SetUpgradeCost({{200, 300, 2300, 12500},
                    {120, 1800, 3250, 5500}});
    // 设置升级路径名称
    SetUpgradeText({{"Glue Soak", "Corrosive Glue", "Bloon Dissolver", "Bloon Liquefier"},
                    {"Sticker Glue", "Glue Splatter", "Glue Hose", "Glue Striker"}});
    auto monkeyImage = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/RubberMonkeyImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/RubberMonkey");
    auto upgradeImage1 = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/RubberMonkey_1_1.png");
    auto upgradeImage2 = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/RubberMonkey_2_1.png");
    auto upgradeText1 = GetUpgrateText1();
    upgradeText1 -> Update("Glue Soak");
    auto upgradeText2 = GetUpgrateText2();
    upgradeText2 -> Update("Sticker Glue");
    auto upgradeButton1 = GetUpgrateButton1();
    upgradeButton1 -> Update("200");
    auto upgradeButton2 = GetUpgrateButton2();
    upgradeButton2 -> Update("120");
}

BuccaneerMonkeyInformationBoard::BuccaneerMonkeyInformationBoard() {
    // 使用600列的价格数据（中等难度）
    SetUpgradeCost({{400, 180, 2200, 17500},
                    {300, 250, 1200, 4500}});
    // 设置升级路径名称
    SetUpgradeText({{"Fast Shooting", "Longer Cannon", "Destoryer", "Aircraft Carrier"},
                    {"Grape Shot", "Crow's Nest", "Cannon Ship", "Monkey Pirates"}});
    auto monkeyImage = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/BuccaneerMonkeyImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/BuccaneerMonkey");
    auto upgradeImage1 = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/BuccaneerMonkey_1_1.png");
    auto upgradeImage2 = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/BuccaneerMonkey_2_1.png");
    auto upgradeText1 = GetUpgrateText1();
    upgradeText1 -> Update("Fast Shooting");
    auto upgradeText2 = GetUpgrateText2();
    upgradeText2 -> Update("Grape Shot");
    auto upgradeButton1 = GetUpgrateButton1();
    upgradeButton1 -> Update("400");
    auto upgradeButton2 = GetUpgrateButton2();
    upgradeButton2 -> Update("300");
}

AirportInformationBoard::AirportInformationBoard() {
    // 使用950列的价格数据（中等难度）
    SetUpgradeCost({{700, 500, 2000, 18000},
                    {200, 350, 3000, 14000}});
    // 设置升级路径名称
    SetUpgradeText({{"Rapid Fire", "Sharper Darts", "Neva-Miss Targeting", "Spectre"},
                    {"Pineapple Present", "Spy plane", "Operation: Dart Storm", "Ground Zero"}});
    auto monkeyImage = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/AirportImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/Airport");
    auto upgradeImage1 = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Airport_1_1.png");
    auto upgradeImage2 = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/Airport_2_1.png");
    auto upgradeText1 = GetUpgrateText1();
    upgradeText1 -> Update("Rapid Fire");
    auto upgradeText2 = GetUpgrateText2();
    upgradeText2 -> Update("Pineapple Present");
    auto upgradeButton1 = GetUpgrateButton1();
    upgradeButton1 -> Update("700");
    auto upgradeButton2 = GetUpgrateButton2();
    upgradeButton2 -> Update("200");
}

SuperMonkeyInformationBoard::SuperMonkeyInformationBoard() {
    // 使用4000列的价格数据（中等难度）
    SetUpgradeCost({{3500, 5000, 16500, 75000},
                    {1000, 1500, 9000, 25000}});
    // 设置升级路径名称
    SetUpgradeText({{"Laser Blasts", "Plasma Blasts", "Sun God", "Temple of the Monkey God"},
                    {"Super Range", "Epic Range", "Robo-Monkey", "Technological Terror"}});
    auto monkeyImage = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/SuperMonkeyImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/SuperMonkey");
    auto upgradeImage1 = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/SuperMonkey_1_1.png");
    auto upgradeImage2 = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/SuperMonkey_2_1.png");
    auto upgradeText1 = GetUpgrateText1();
    upgradeText1 -> Update("Laser Blasts");
    auto upgradeText2 = GetUpgrateText2();
    upgradeText2 -> Update("Super Range");
    auto upgradeButton1 = GetUpgrateButton1();
    upgradeButton1 -> Update("3500");
    auto upgradeButton2 = GetUpgrateButton2();
    upgradeButton2 -> Update("1000");
}

MagicMonkeyInformationBoard::MagicMonkeyInformationBoard() {
    // 使用550列的价格数据（中等难度）
    SetUpgradeCost({{300, 800, 2000, 8000},
                    {200, 300, 3500, 8000}});
    // 设置升级路径名称
    SetUpgradeText({{"Intense Magic", "Lighting Bolt", "Summon Whirlwind", "Tempest Tornado"},
                    {"Fireball", "Monkey Sense", "Dragon's Breath", "Summon Phoenix"}});
    auto monkeyImage = GetMonkeyImage();
    monkeyImage -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/MagicMonkeyImage.png");
    SetUpgrateName(GA_RESOURCE_DIR"/MonkeyInformation/MagicMonkey");
    auto upgradeImage1 = GetUpgrateImage1();
    upgradeImage1 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/MagicMonkey_1_1.png");
    auto upgradeImage2 = GetUpgrateImage2();
    upgradeImage2 -> SetImage(GA_RESOURCE_DIR"/MonkeyInformation/MagicMonkey_2_1.png");
    auto upgradeText1 = GetUpgrateText1();
    upgradeText1 -> Update("Intense Magic");
    auto upgradeText2 = GetUpgrateText2();
    upgradeText2 -> Update("Fireball");
    auto upgradeButton1 = GetUpgrateButton1();
    upgradeButton1 -> Update("300");
    auto upgradeButton2 = GetUpgrateButton2();
    upgradeButton2 -> Update("200");
}

