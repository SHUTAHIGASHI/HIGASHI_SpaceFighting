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
    // �w�i�摜
    int m_hBarImg;
    // �`����W
    int m_drawX, m_drawY;
    int m_drawW, m_drawH;

private:
    // �ő�HP
    float m_maxHp;
    // ���݂�HP��
    float m_currentHpRate;
    // HP�o�[�̐F
    int m_color;
};

