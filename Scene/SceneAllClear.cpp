#include "SceneAllClear.h"
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
	const char* const kTextScene = "ゲームクリア！";
	const char* const kScoreTexts[] = {
		"スコア : ",
		"ランク : "
	};
	// 選択項目
	const char* const kMenuTexts[] = {
		"タイトルへ戻る",
		"最初から"
	};

	// テキスト描画位置
	constexpr float kTextDrawPosX = Game::kScreenWidthHalf - 200.0f;
	constexpr float kTextDrawPosY = Game::kScreenHeightHalf;
	// スコア描画位置
	constexpr float kHighScoreDrawPosX = Game::kScreenWidthHalf + 200.0f;
	constexpr float kHighScoreDrawPosY = Game::kScreenHeightHalf;
	// タイトルテキスト描画位置
	constexpr float kTitleDrawPosX = Game::kScreenWidthHalf;
	constexpr float kTitleDrawPosY = Game::kScreenHeightHalf - 180.0f;
	// スコア描画初期位置
	constexpr float kScorePosY = 1000.0f;
	// スクロール速度
	constexpr float kScrollSpeed = 12.0f;
	// 文字間隔
	constexpr int kTextDistance = static_cast<int>(Game::kFontSize * 1.5);
}

SceneAllClear::SceneAllClear(SceneManager& manager) :
	Scene(manager),
	m_updateFunc(&SceneAllClear::StartUpdate),
	m_hBgImg(-1),
	m_highScore(0),
	m_rank(""),
	m_scorePosY(-kScorePosY),
	m_pSelectMenu(std::make_shared<SelectMenuBase>())
{
	// ハイスコア取得
	m_highScore = GameDataManager::GetInstance().GetPoint();
	m_rank = GameDataManager::GetInstance().GetRank();
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

SceneAllClear::~SceneAllClear()
{
	// 音楽停止
	SoundManager::GetInstance().StopBGM();
	// 背景画像削除
	DeleteGraph(m_hBgImg);
}

void SceneAllClear::Update(const InputState& input)
{
	// 音楽再生開始
	if (SoundManager::GetInstance().IsPlayingMusic() != 1)
	{
		SoundManager::GetInstance().PlayMusic(MusicType::over);
	}

	// Update処理のメンバ関数ポインタ
	(this->*m_updateFunc)(input);
}

void SceneAllClear::Draw()
{
	// 背景描画
	DrawRotaGraphF(Game::kScreenWidthHalf, Game::kScreenHeightHalf + m_scorePosY, 1.0f, 0.0f, m_hBgImg, true);

	// スコア描画
	int i = 0;
	for (auto& text : kScoreTexts)
	{
		std::string scoreText;
		if (i == 0) scoreText = text + std::to_string(m_highScore);
		else scoreText = text + m_rank;
		int textLength = GetDrawFormatStringWidth(scoreText.c_str());
		// 描画位置設定
		int drawX = static_cast<int>(kHighScoreDrawPosX - (textLength / 2));
		int drawY = static_cast<int>(kHighScoreDrawPosY + (i * kTextDistance));
		DrawFormatString(drawX, drawY + static_cast<int>(m_scorePosY), Game::kColorWhite, "%s", scoreText.c_str());
		i++;
	}
	// メニュー描画
	m_pSelectMenu->Draw();
}

void SceneAllClear::StartUpdate(const InputState& input)
{
	if (m_scorePosY < 0.0f)
	{
		m_scorePosY += kScrollSpeed;
	}
	else
	{
		m_scorePosY = 0.0f;
		m_updateFunc = &SceneAllClear::NormalUpdate;
	}

	// 描画位置設定
	m_pSelectMenu->SetDrawPos(kTextDrawPosX, kTextDrawPosY + m_scorePosY);
	m_pSelectMenu->SetTitleDrawPos(kTitleDrawPosX, kTitleDrawPosY + m_scorePosY);
}

void SceneAllClear::NormalUpdate(const InputState& input)
{
	// 選択メニュー更新
	m_pSelectMenu->Update(input);
	// エンターキーが押されたら
	if (input.IsTriggered(InputType::select))
	{
		// SE再生
		SoundManager::GetInstance().PlaySE(SoundType::select);
		// 選択項目によって処理を分岐
		m_updateFunc = &SceneAllClear::EndUpdate;
		return;
	}
}

void SceneAllClear::EndUpdate(const InputState& input)
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
			// ステージリセット
			GameDataManager::GetInstance().ResetStage();
			// タイトルへ戻る
			m_Manager.ChangeScene(new SceneTitleMenu(m_Manager));
		}
		else if (m_pSelectMenu->GetSelectedNum() == 1)
		{
			// ステージリセット
			GameDataManager::GetInstance().ResetStage();
			// 次のステージへ
			m_Manager.ChangeScene(new SceneMain(m_Manager));
		}
		return;
	}

	// 描画位置設定
	m_pSelectMenu->SetDrawPos(kTextDrawPosX, kTextDrawPosY + m_scorePosY);
	m_pSelectMenu->SetTitleDrawPos(kTitleDrawPosX, kTitleDrawPosY + m_scorePosY);
}
