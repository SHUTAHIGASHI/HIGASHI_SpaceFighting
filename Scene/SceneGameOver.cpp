#include "SceneGameOver.h"
#include "SceneManager.h"
#include "GameDataManager.h"
#include "SelectMenuBase.h"
#include "Game.h"
#include "SceneMain.h"
#include "SceneTitleMenu.h"
#include "SoundManager.h"

namespace
{
	// テキスト
	const char* const kTextScene = "ゲームオーバー";
	const char* const kTextScore = "スコア : ";
	// 選択項目
	const char* const kMenuTexts[] = {
		"コンティニュー",
		"タイトルへ戻る"
	};

	// テキスト描画位置
	constexpr float kTextDrawPosX = Game::kScreenWidthHalf;
	constexpr float kTextDrawPosY = Game::kScreenHeightHalf;
	// タイトルテキスト描画位置
	constexpr float kTitleDrawPosX = Game::kScreenWidthHalf;
	constexpr float kTitleDrawPosY = Game::kScreenHeightHalf - 180.0f;
	// スコア描画初期位置
	constexpr float kScorePosY = 1000.0f;
	// スクロール速度
	constexpr float kScrollSpeed = 12.0f;
}

SceneGameOver::SceneGameOver(SceneManager& manager) :
	Scene(manager),
	m_updateFunc(&SceneGameOver::StartUpdate),
	m_hBgImg(-1),
	m_scorePosY(-kScorePosY),
	m_pSelectMenu(std::make_shared<SelectMenuBase>())
{
	// 選択メニュー初期化
	m_pSelectMenu->Init(false);
	// シーン文字列設定
	m_pSelectMenu->SetTitleTextAndPos(kTextScene, kTitleDrawPosX, kTitleDrawPosY + m_scorePosY);
	// 描画位置設定
	m_pSelectMenu->SetDrawPos(kTextDrawPosX, kTextDrawPosY + m_scorePosY);
	// 選択メニューに項目を追加
	for (auto& item : kMenuTexts)
	{
		m_pSelectMenu->AddSelectItem(item);
	}

	// 背景画像読み込み
	m_hBgImg = LoadGraph("Data/ImageData/OptionBg.png");
}

SceneGameOver::~SceneGameOver()
{
	// 音楽停止
	SoundManager::GetInstance().StopBGM();
	// 背景画像削除
	DeleteGraph(m_hBgImg);
}

void SceneGameOver::Update(const InputState& input)
{
	// 音楽再生開始
	if (SoundManager::GetInstance().IsPlayingMusic() != 1)
	{
		SoundManager::GetInstance().PlayMusic(MusicType::over);
	}

	// Update処理のメンバ関数ポインタ
	(this->*m_updateFunc)(input);
}

void SceneGameOver::Draw()
{
	// 背景描画
	DrawRotaGraphF(Game::kScreenWidthHalf, Game::kScreenHeightHalf + m_scorePosY, 1.0f, 0.0f, m_hBgImg, true);

	// メニュー描画
	m_pSelectMenu->Draw();
}

void SceneGameOver::StartUpdate(const InputState& input)
{
	if (m_scorePosY < 0.0f)
	{
		m_scorePosY += kScrollSpeed;
	}
	else
	{
		m_scorePosY = 0.0f;
		m_updateFunc = &SceneGameOver::NormalUpdate;
	}

	// 描画位置設定
	m_pSelectMenu->SetDrawPos(kTextDrawPosX, kTextDrawPosY + m_scorePosY);
	m_pSelectMenu->SetTitleDrawPos(kTitleDrawPosX, kTitleDrawPosY + m_scorePosY);
}

void SceneGameOver::NormalUpdate(const InputState& input)
{
	// 選択メニュー更新
	m_pSelectMenu->Update(input);
	// エンターキーが押されたら
	if (input.IsTriggered(InputType::select))
	{
		// SE再生
		SoundManager::GetInstance().PlaySE(SoundType::select);
		// 選択項目によって処理を分岐
		m_updateFunc = &SceneGameOver::EndUpdate;
		return;
	}
}

void SceneGameOver::EndUpdate(const InputState& input)
{
	if (m_scorePosY < kScorePosY)
	{
		m_scorePosY += kScrollSpeed;
	}
	else
	{
		m_scorePosY = kScorePosY;
		if (m_pSelectMenu->GetSelectedNum() == 0)
		{
			// 次のステージへ
			m_Manager.ChangeScene(new SceneMain(m_Manager));
		}
		else if (m_pSelectMenu->GetSelectedNum() == 1)
		{
			// ステージリセット
			GameDataManager::GetInstance().ResetStage();
			// タイトルへ戻る
			m_Manager.ChangeScene(new SceneTitleMenu(m_Manager));
		}
		return;
	}

	// 描画位置設定
	m_pSelectMenu->SetDrawPos(kTextDrawPosX, kTextDrawPosY + m_scorePosY);
	m_pSelectMenu->SetTitleDrawPos(kTitleDrawPosX, kTitleDrawPosY + m_scorePosY);
}
