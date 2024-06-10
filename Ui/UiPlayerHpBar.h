#pragma once

class UiPlayerHpBar
{
public:
    UiPlayerHpBar(int handle);
    virtual ~UiPlayerHpBar();

    void Init(float maxHp);
    void Update();
    void UpdateUIData(int hp);
    void Draw();

private:
    // ”wŒi‰æ‘œ
    int m_hBarImg;
    // •`‰æÀ•W
    int m_drawX, m_drawY;
    int m_drawW, m_drawH;

private:
    // Å‘åHP
    float m_maxHp;
    // Œ»İ‚ÌHP—Ê
    float m_currentHpRate;
    // HPƒo[‚ÌF
    int m_color;
};

