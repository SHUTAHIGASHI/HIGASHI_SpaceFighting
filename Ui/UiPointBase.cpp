#include "UiPointBase.h"

UiPointBase::UiPointBase():
	m_posX(0.0f),
	m_posY(0.0f),
	m_isDelete(false),
	m_countDelay(0),
	m_numData(0),
	m_textData("EMPTY"),
	m_textColor(Game::kColorWhite)
{
}

UiPointBase::~UiPointBase()
{
}

void UiPointBase::Draw()
{
	DrawFormatStringF(m_posX, m_posY, m_textColor, "%s%d", m_textData.c_str(), m_numData);
}

void UiPointBase::SetUIInfo(int tData)
{
	m_numData = tData;
}
