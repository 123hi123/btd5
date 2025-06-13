# Debuff系統詳解

## 概述

在這個塔防遊戲中，Debuff系統是用來對敵人（氣球）施加負面效果的機制。這些效果能夠減緩敵人移動速度、改變敵人屬性或影響其行為。Debuff系統主要由`DebuffView`類和`Balloon`類中的相關方法實現。

## Debuff類型

系統中實現了以下幾種Debuff類型：

1. **雪（Snow）** - 索引0
   - 視覺效果：雪花圖案
   - 效果：減速50%（debuff_slow[0] = 0.5）
   - 特性：累積到350時會轉換為冰凍效果

2. **冰凍（Ice）** - 索引1
   - 視覺效果：冰塊圖案
   - 效果：完全停止移動（debuff_slow[1] = 0）
   - 特性：會給氣球添加冰屬性（property 1）

3. **粘液（Rubber/Mucus）** - 索引2
   - 視覺效果：粘液圖案
   - 效果：減速20%（debuff_slow[2] = 0.2）

4. **岩忍者效果（RockNinja）** - 索引4
   - 視覺效果：岩石圖案
   - 效果：減速50%（debuff_slow[4] = 0.5）

5. **其他效果（索引3）** - 僅對非飛船類型無效
   - 無視覺效果
   - 特殊處理：只能對飛船類型敵人生效

6. **隨機效果（索引5）**
   - 特性：有50%機率不生效

## Debuff實現機制

### 數據結構

1. **視覺元素**：
   ```cpp
   std::shared_ptr<DebuffView> snow = std::make_shared<Snow>();
   std::shared_ptr<DebuffView> ice = std::make_shared<Ice>();
   std::shared_ptr<DebuffView> rubber = std::make_shared<Mucus>();
   std::shared_ptr<DebuffView> rock_ninja = std::make_shared<RockNinjaDebuff>();
   ```

2. **效果計時器**：
   ```cpp
   std::vector<int> m_Debuff = {0, 0, 0, 0, 0};  // 各種Debuff的持續時間
   ```

3. **減速效果值**：
   ```cpp
   std::vector<float> debuff_slow = {0.5, 0, 0.2, 0, 0.5};  // 減速比例
   ```

### 應用Debuff的過程

1. **獲取Debuff**：
   ```cpp
   void Balloon::GetDebuff(std::vector<std::vector<int>> debuff)
   ```
   - 這個方法接收一個二維向量，每個內部向量包含兩個值：[debuff類型, 持續時間]
   - 根據debuff類型更新對應的計時器
   - 處理特殊情況（如冰凍優先級、隨機效果、雪轉冰等）
   - 如果是冰凍效果，還會修改氣球的屬性（添加屬性1）

2. **更新Debuff狀態**：
   ```cpp
   float Balloon::UpdateDebuff()
   ```
   - 每幀調用，更新所有debuff計時器
   - 如果某種debuff處於活動狀態，顯示對應視覺效果並應用減速
   - 減速效果是累乘的，多種debuff同時存在會產生疊加效果
   - 計時器歸零時自動移除debuff效果和視覺顯示
   - 對於冰凍效果，結束時會從氣球的屬性中移除冰屬性

## Debuff觸發和結束時的機制

### 觸發機制

Debuff主要通過攻擊碰撞來觸發。不同攻擊類型可以施加不同的debuff：

1. **冰凍效果**：由`Blizzard`（暴風雪）、`IceMonkey`（冰猴）等攻擊觸發
2. **粘液效果**：由`Rubber`（橡膠）、`RubberMonkey`（橡膠猴）等攻擊觸發
3. **岩忍者效果**：由`RockNinja`攻擊觸發

### 監控Debuff結束

要在Debuff結束時觸發額外效果，可以擴展`UpdateDebuff`方法。目前代碼中已經有一個示例：

```cpp
if (i == 1 && m_Debuff[i] == 0) {
    m_Properties.pop_back();  // 冰凍效果結束時移除冰屬性
}
```

要添加更多的結束時效果，可以在這個方法中增加邏輯：

```cpp
// 示例：在這個位置添加自定義邏輯，檢測debuff結束
if (m_Debuff[i] == 1) {  // 將在下一幀結束
    // 即將結束的處理邏輯
}
else if (m_Debuff[i] == 0 && previousDebuff[i] > 0) {  // 剛剛結束
    // 結束後的處理邏輯
}
```

## 實現自訂結束效果的方法

1. **繼承和擴展**：創建`Balloon`的子類，重寫`UpdateDebuff`方法來添加自定義邏輯

2. **事件回調**：修改`Balloon`類，添加回調機制（如`OnDebuffEnd`）

3. **觀察者模式**：實現觀察者模式，當debuff狀態改變時通知訂閱者

4. **直接修改**：在`UpdateDebuff`方法中直接添加條件檢查和對應處理邏輯

## 具體示例：Debuff結束時的爆炸效果

如要實現debuff結束時產生爆炸效果，可以這樣修改`UpdateDebuff`方法：

```cpp
float Balloon::UpdateDebuff() {
    float slow = 1;
    for (int i = 0; i < m_Debuff.size(); i++) {
        if (m_Debuff[i] > 0) {
            // 處理活動中的debuff...
            
            // 檢查是否即將結束（例如只剩1幀）
            if (m_Debuff[i] == 1) {
                // 為特定類型的debuff準備結束效果
                if (i == 1) {  // 冰凍效果
                    // 準備冰爆效果，如設置標記或準備參數
                }
            }
            
            m_Debuff[i] -= 1;
            
            // 檢查是否剛剛結束
            if (m_Debuff[i] == 0) {
                if (i == 1) {
                    m_Properties.pop_back();  // 移除冰屬性
                    
                    // 觸發冰爆效果
                    std::vector<std::vector<int>> damageNearby = {{0, 50}};  // 雪花debuff 50幀
                    // 對周圍物體施加效果或產生爆炸攻擊物
                    // 可以創建一個新的Attack對象在當前位置
                }
            }
        }
        else {
            // 處理非活動狀態...
        }
    }
    return slow;
};
```

## 總結

Debuff系統是遊戲中重要的機制，可以給玩家策略提供更多可能性。通過拓展現有的`UpdateDebuff`方法，可以實現debuff結束時的各種連鎖反應，例如：

1. 冰凍效果結束時產生範圍傷害（冰爆）
2. 雪效果完全累積後轉換為冰凍效果（已實現）
3. 粘液效果結束時使敵人短暫加速（反彈效果）
4. 特定debuff組合產生化學反應，如火+冰產生蒸氣雲霧

這些機制可以豐富遊戲玩法並增加策略深度。 