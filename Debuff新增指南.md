# Debuff 新增指南

## 前言

本文件旨在說明在遊戲中新增 Debuff 效果的完整流程。Debuff 系統用於對氣球施加減速或其他負面效果，是遊戲策略的重要部分。

## 新增 Debuff 的步驟

### 1. 在 DebuffView.hpp 中新增 Debuff 視覺效果類

```cpp
class 新Debuff名稱 : public DebuffView {
public:
    explicit 新Debuff名稱();
};
```

### 2. 在 DebuffView.cpp 中實現該類的構造函數

```cpp
新Debuff名稱::新Debuff名稱() {
    SetImage(GA_RESOURCE_DIR"/Debuff/新圖片名稱.png"); // 設置適當的圖片路徑
    SetVisible(false);
    SetZIndex(4); // 設置適當的Z層級，決定顯示優先順序
};
```

### 3. 在 Balloon.hpp 中新增相關內容

在 Balloon 類的私有成員中添加：

```cpp
// 新增 DebuffView 實例
std::shared_ptr<DebuffView> 新debuff名稱 = std::make_shared<新Debuff名稱>();

// 確保 m_Debuff 向量有足夠的索引位置（如需要）
std::vector<int> m_Debuff = {0, 0, 0, 0, 0, 0, 0, 0, 新索引}; 

// 在 debuff_slow 中添加新 Debuff 的減速效果值
std::vector<float> debuff_slow = {0.5, 0, 0.2, 0, 0.5, -1, 0, 0, 新減速值};
```

### 4. 修改 Balloon.cpp 中的 GetDebuffViews() 函數

```cpp
std::vector<std::shared_ptr<Util::GameObject>> Balloon::GetDebuffViews() {
    return {snow, ice, rubber, rock_ninja, dizzylight, iceburst, 新debuff名稱};
}
```

### 5. 在 Balloon.cpp 的 UpdateDebuff() 方法中新增處理邏輯

```cpp
float Balloon::UpdateDebuff() {
    float slow = 1;
    for (int i = 0; i < m_Debuff.size(); i++) {
        if (m_Debuff[i] != 0) {
            // 顯示視覺效果（已有的條件分支...）
            else if (i == 新索引) { 新debuff名稱 -> Update(GetPosition(), true); }
            
            // 計算減速效果
            slow *= debuff_slow[i];
            m_Debuff[i] -= 1;
            
            // 若需要在 Debuff 結束時執行特殊動作
            if (i == 新索引 && m_Debuff[i] == 0) {
                // 特殊處理邏輯，例如：
                // - 觸發爆炸效果
                // - 添加其他 Debuff
                // - 移除特定屬性
            }
        }
        else {
            // 隱藏視覺效果（已有的條件分支...）
            else if (i == 新索引) { 新debuff名稱 -> Update(GetPosition(), false); }
        }
    }
    return slow;
}
```

### 6. 在攻擊類或猴子類中設置 Debuff 效果觸發

#### 在攻擊類構造函數中添加

```cpp
// 在攻擊類的構造函數中
GetAttributes() -> AddDebuff({新索引, 持續時間});
```

#### 在猴子的升級函數中添加

```cpp
void 猴子類名::UpdateLevel() {
    int level = GetLevel();
    int upgradePath = GetUpgradePath();
    auto attributes = GetAttributes();
    
    if (upgradePath == 1) {
        switch (level) {
            case 1:
                // 其他升級效果...
                attributes -> AddDebuff({新索引, 持續時間});
                break;
            // 其他等級...
        }
    }
    // 其他升級路徑...
}
```

## 現有的 Debuff 索引及其含義

| 索引 | 名稱 | 效果 | 減速值 |
|------|------|------|--------|
| 0 | 霜 | 減速 50%，累積到 350 會變成冰凍 | 0.5 |
| 1 | 凍 | 完全停止移動，添加冰屬性 | 0 |
| 2 | 膠 | 減速 20% | 0.2 |
| 3 | 破壞飛船 | 僅對飛船類型有效 | 0 |
| 4 | 忍者技能減速 | 減速 50% | 0.5 |
| 5 | 忍者擊退 | 有 50% 機率失效 | -1 |
| 6 | 閃光蛋 | 視覺效果 | 0 |
| 7 | 爆裂冰塊標記 | 視覺效果 | 0 |

## 特殊效果處理範例

### Debuff 結束時的特殊效果

要在 Debuff 結束時觸發特殊效果，可以在 `UpdateDebuff()` 方法中添加條件檢查：

```cpp
// 檢測 Debuff 是否剛剛結束
if (i == 特定索引 && m_Debuff[i] == 0) {
    // 例如：冰凍效果結束時移除冰屬性
    if (i == 1) {
        m_Properties.pop_back();
    }
    
    // 例如：冰爆效果（在冰凍結束時產生範圍傷害）
    if (i == 7) {
        // 創建爆炸效果或對周圍氣球施加新的 Debuff
        // 可以創建一個新的 Attack 對象
    }
}
```

### Debuff 累積轉換效果

現有代碼中已實現雪花累積成冰的效果：

```cpp
void Balloon::GetDebuff(std::vector<std::vector<int>> debuff) {
    // ...
    if (m_Debuff[0] > 350) {  // 雪花累積到 350
        m_Debuff[0] = 0;      // 清除雪花效果
        m_Debuff[1] = 100;    // 添加 100 幀的冰凍效果
    }
    // ...
}
```

## 注意事項

1. **圖片資源**：確保新的 Debuff 圖片放在正確的路徑下（通常是 `GA_RESOURCE_DIR"/Debuff/"`）
2. **索引一致性**：新 Debuff 的索引在各處必須保持一致
3. **減速值選擇**：
   - 0 表示完全停止
   - 0~1 之間表示減速比例（越小減速越嚴重）
   - -1 等特殊值需要在代碼中處理特殊邏輯
4. **特殊效果處理**：複雜的特殊效果可能需要修改其他部分的代碼或添加新的類

## 範例：新增「燃燒」Debuff

以下是新增一個「燃燒」Debuff 的完整範例：

1. **在 DebuffView.hpp 添加：**
```cpp
class Fire : public DebuffView {
public:
    explicit Fire();
};
```

2. **在 DebuffView.cpp 實現：**
```cpp
Fire::Fire() {
    SetImage(GA_RESOURCE_DIR"/Debuff/Fire.png");
    SetVisible(false);
    SetZIndex(3);
};
```

3. **在 Balloon.hpp 添加：**
```cpp
std::shared_ptr<DebuffView> fire = std::make_shared<Fire>();
// 假設使用索引 8
std::vector<int> m_Debuff = {0, 0, 0, 0, 0, 0, 0, 0, 0};
std::vector<float> debuff_slow = {0.5, 0, 0.2, 0, 0.5, -1, 0, 0, 0.7}; // 30% 減速
```

4. **修改 GetDebuffViews()：**
```cpp
std::vector<std::shared_ptr<Util::GameObject>> Balloon::GetDebuffViews() {
    return {snow, ice, rubber, rock_ninja, dizzylight, iceburst, fire};
}
```

5. **在 UpdateDebuff() 添加：**
```cpp
float Balloon::UpdateDebuff() {
    float slow = 1;
    for (int i = 0; i < m_Debuff.size(); i++) {
        if (m_Debuff[i] != 0) {
            // 已有代碼...
            else if (i == 8) { fire -> Update(GetPosition(), true); }
            
            slow *= debuff_slow[i];
            
            // 燃燒效果每幀造成傷害
            if (i == 8) {
                LoseHealth(1); // 每幀扣除 1 點生命值
            }
            
            m_Debuff[i] -= 1;
        }
        else {
            // 已有代碼...
            else if (i == 8) { fire -> Update(GetPosition(), false); }
        }
    }
    return slow;
}
```

6. **在火系攻擊中添加：**
```cpp
// 在火系攻擊的構造函數中
GetAttributes() -> AddDebuff({8, 60}); // 60 幀的燃燒效果
``` 