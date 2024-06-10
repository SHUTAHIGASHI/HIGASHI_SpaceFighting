#include "SelectMenuBase.h"
#include "SceneManager.h"

namespace
{
	// 文字間隔
	constexpr int kTextDistance = static_cast<int>(Game::kFontSize * 1.5);
	// シーン文字列拡大率
	constexpr float kSceneTextScale = 1.5f;
}

SelectMenuBase::SelectMenuBase() :
	m_updateFunc(&SelectMenuBase::NormalUpdate),
	m_selectedPos(0),
	m_selectItems(),
	m_sceneText(nullptr),
	m_mouseState(),
	m_isCursorRanged(false),
	m_drawPos(Game::kVecZero),
	m_titleDrawPos(Game::kVecZero),
	m_itemColor(Game::kColorGray),
	m_selectedItemColor(Game::kColorWhite),
	m_countFrame(0)
{
}

SelectMenuBase::~SelectMenuBase()
{
}

void SelectMenuBase::Init(bool isMouse)
{
	if (isMouse) m_updateFunc = &SelectMenuBase::CursorUpdate;
}

void SelectMenuBase::Update(const InputState& input)
{
	// フレームカウント
	m_countFrame++;
	if (m_countFrame > 60) m_countFrame = 0;
	// マウスカーソル座標取得
	m_mouseState.x = input.GetMousePosX();
	m_mouseState.y = input.GetMousePosY();
	// 更新
	(this->*m_updateFunc)(input);
}

void SelectMenuBase::Draw()
{
	// シーン文字列描画
	DrawSceneText();
	// メニュー文字列描画
	DrawMenuText();
}

void SelectMenuBase::AddSelectItem(std::string itemName)
{
	m_selectItems.push_back(SelectItemState());
	m_selectItems.back().itemName = itemName;
}

void SelectMenuBase::DrawMenuText()
{
	int drawX = 0, drawY = 0;

	int itemCount = 0;
	for (auto& items : m_selectItems)
	{
		int textLength = GetDrawFormatStringWidth(items.itemName.c_str());
		drawX = static_cast<int>(m_drawPos.x - (textLength / 2));
		drawY = static_cast<int>(m_drawPos.y + (kTextDistance * itemCount));
		DrawFormatString(drawX, drawY, m_itemColor, "%s", items.itemName.c_str());

		itemCount++;
	}

	std::string drawText = "> " + m_selectItems[m_selectedPos].itemName + " <";
	int textLength = GetDrawFormatStringWidth(drawText.c_str());
	drawX = static_cast<int>(m_drawPos.x - (textLength / 2));
	drawY = static_cast<int>(m_drawPos.y + (kTextDistance * m_selectedPos));

	if ((m_countFrame / 10) % 6 != 0)
	{
		DrawFormatString(drawX, drawY - 2, m_selectedItemColor, "%s", drawText.c_str());
	}
}

void SelectMenuBase::DrawSceneText()
{
	// シーン文字列描画
	if (m_sceneText != nullptr)
	{
		// フォントサイズ拡大
		SetFontSize(static_cast<int>(Game::kFontSize * kSceneTextScale));
		int textLength = GetDrawFormatStringWidth(m_sceneText->c_str());
		int drawX = static_cast<int>(m_titleDrawPos.x - (textLength / 2));
		int drawY = static_cast<int>(m_titleDrawPos.y);
		// 文字描画
		DrawFormatString(drawX, drawY, Game::kColorWhite, "%s", m_sceneText->c_str());
		// フォントサイズ戻す
		SetFontSize(Game::kFontSize);
	}
}

void SelectMenuBase::CursorUpdate(const InputState& input)
{
	m_isCursorRanged = false;

	int buttonPosX = 0, buttonPosY = 0;
	int itemCount = 0;
	for (auto& items : m_selectItems)
	{
		int textLength = GetDrawFormatStringWidth(items.itemName.c_str());
		buttonPosX = static_cast<int>(m_drawPos.x - (textLength / 2));
		buttonPosY = static_cast<int>(m_drawPos.y + (kTextDistance * itemCount));

		if (m_mouseState.x > buttonPosX && m_mouseState.x < buttonPosX + textLength
			&& m_mouseState.y > buttonPosY && m_mouseState.y < buttonPosY + Game::kFontSize)
		{
			m_isCursorRanged = true;
			m_selectedPos = itemCount;
			m_countFrame = 6;
		}
		itemCount++;
	}
}

void SelectMenuBase::NormalUpdate(const InputState& input)
{
	int selectItemMax = static_cast<int>(m_selectItems.size() - 1);
	if (input.IsTriggered(InputType::up))
	{
		m_selectedPos--;
		if (m_selectedPos < 0) m_selectedPos = selectItemMax;
		m_countFrame = 6;
	}
	else if (input.IsTriggered(InputType::down))
	{
		m_selectedPos++;
		if (m_selectedPos > selectItemMax) m_selectedPos = 0;
		m_countFrame = 6;
	}
}
