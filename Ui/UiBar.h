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
    // �`����W
    int m_drawX, m_drawY;

private:
    // HP��
    int m_num;
    // HP�o�[�̍ő�l
    int m_maxNum;
    // ���݂�HP��
    float m_currentNumRate;
    float m_maxNumRate;
	// HP�o�[�̐F
	int m_color;
};

