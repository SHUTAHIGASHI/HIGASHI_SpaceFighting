#pragma once

class UiBar
{
public:
    UiBar();
    virtual ~UiBar();
    
    void Init(int maxHp);
    void Update();
    void UpdateUIData(int hp);
    void Draw();

    void SetDrawPos(int x, int y);

private:
    // 描画座標
    int m_drawX, m_drawY;

private:
    // HP量
    int m_num;
    // HPバーの最大値
    int m_maxNum;
    // 現在のHP率
    float m_currentNumRate;
    float m_maxNumRate;
	// HPバーの色
	int m_color;
};

