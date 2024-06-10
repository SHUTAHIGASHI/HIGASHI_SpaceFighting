#include "UiPlayerHpBar.h"
#include "Game.h"
#include "Player.h"

namespace
{
	// ÉQÅ[ÉWÇÃç¿ïW
	constexpr int kGaugeX = Game::kScreenWidthHalf;
	constexpr int kGaugeY = 150;
	constexpr int kGaugeW = 100;
	constexpr int kGaugeH = 50;
	constexpr int kDrawPosDiff = 500;
}

UiPlayerHpBar::UiPlayerHpBar(int handle) :
	m_hBarImg(handle),
	m_drawX(0),
	m_drawY(0),
	m_drawW(0),
	m_drawH(0),
	m_maxHp(0.0f),
	m_currentHpRate(0.0f),
	m_color(0)
{
}

UiPlayerHpBar::~UiPlayerHpBar()
{
}

void UiPlayerHpBar::Init(float maxHp)
{
	m_maxHp = maxHp;
	m_color = Game::kColorOrange;
	m_drawX = kGaugeX - kDrawPosDiff;
	m_drawY = kGaugeY;
}

void UiPlayerHpBar::Update()
{
	m_drawW = m_drawX + static_cast<int>(m_currentHpRate * 10);
	m_drawH = m_drawY + kGaugeH;

	if (m_currentHpRate < 30)
	{
		m_color = Game::kColorRed;
	}
	else if (m_currentHpRate < 60)
	{
		m_color = Game::kColorOrange;
	}
	else
	{
		m_color = Game::kColorGreen;
	}
}

void UiPlayerHpBar::UpdateUIData(int hp)
{
	m_currentHpRate = (static_cast<float>(hp) / m_maxHp) * 100;
}

void UiPlayerHpBar::Draw()
{
	DrawBox(m_drawX, m_drawY, m_drawW, m_drawH, m_color, true);
	DrawRotaGraph(m_drawX + kDrawPosDiff, m_drawY, 1, 0.0, m_hBarImg, true);
}
