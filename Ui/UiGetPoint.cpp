#include "UiGetPoint.h"

UiGetPoint::UiGetPoint()
{
	m_posX = Game::kScreenWidthHalf;
	m_posY = Game::kScreenHeightHalf + 150.0f;

	m_textData = "+";
	m_numData = 0;
	m_textColor = 0xffff00;

	m_countDelay = kDrawDelay;
}

UiGetPoint::UiGetPoint(int point)
{
	m_posX = Game::kScreenWidthHalf;
	m_posY = Game::kScreenHeightHalf + 150.0f;

	m_textData = "+";
	m_numData = point;
	m_textColor = 0xffff00;

	m_countDelay = kDrawDelay;
}

UiGetPoint::~UiGetPoint()
{
}

void UiGetPoint::Update()
{
	m_countDelay--;

	if (m_countDelay < 0)
	{
		m_isDelete = true;
	}
	else
	{
		m_posY -= kTextMoveSpeed;
	}
}
