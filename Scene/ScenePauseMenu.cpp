#include "ScenePauseMenu.h"
#include "Game.h"
#include "SceneManager.h"
#include "SceneOption.h"
#include "SceneTitleMenu.h"
#include "SelectMenuBase.h"
#include "EffekseerManager.h"
#include "SoundManager.h"

namespace
{
	// テキスト
	const char* const kTextPause = "～ポーズメニュー～";
	// 選択項目
	const char* const kMenuTexts[] = {
		"戻る",
		"設定",
		"タイトルへ"
	};
}

ScenePauseMenu::ScenePauseMenu(SceneManager& manager) :
	Scene(manager),
	m_pSelectMenu(std::make_shared<SelectMenuBase>())
{
	// 選択メニューの初期化
	m_pSelectMenu->Init(false);
	// シーン文字列設定
	for (auto& item : kMenuTexts)
	{
		m_pSelectMenu->AddSelectItem(item);
	}
	// 選択項目描画位置設定
	m_pSelectMenu->SetDrawPos(Game::kScreenWidthHalf, Game::kScreenHeightHalf - 50.0f);
}

ScenePauseMenu::~ScenePauseMenu()
{

}

void ScenePauseMenu::Update(const InputState& input)
{
	if (input.IsTriggered(InputType::pause))
	{
		SoundManager::GetInstance().RestartCurrentBGM();
		m_Manager.PopScene();
		return;
	}

	m_pSelectMenu->Update(input);

	if (input.IsTriggered(InputType::select))
	{
		SoundManager::GetInstance().PlaySE(SoundType::select);

		if (m_pSelectMenu->GetSelectedNum() == 0)
		{
			SoundManager::GetInstance().RestartCurrentBGM();
			m_Manager.PopScene();
		}
		else if (m_pSelectMenu->GetSelectedNum() == 1)
		{
			m_Manager.PushScene(new SceneOption(m_Manager));
		}
		else if (m_pSelectMenu->GetSelectedNum() == 2)
		{
			m_Manager.ChangeScene(new SceneTitleMenu(m_Manager));
		}
		return;
	}
}

void ScenePauseMenu::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	float drawX = 0.0f, drawY = 0.0f;
	drawX = static_cast<float>(Game::kScreenWidthHalf - (GetDrawFormatStringWidth(kTextPause) / 2));
	drawY = Game::kScreenHeightHalf - 150.0f;
	DrawStringF(drawX, drawY, kTextPause, 0xffffff);

	m_pSelectMenu->Draw();
}
