#include "SceneHowTo.h"
#include "SceneManager.h"

namespace
{
	// 描画テキスト
	const char* const kTextBack = "ボタンを押して戻る";
}

SceneHowTo::SceneHowTo(SceneManager& manager) :
	Scene(manager),
	m_countFrame(0),
	m_hImg(-1),
	m_pos()
{
	// 画像読み込み
	m_hImg = LoadGraph("Data/ImageData/HowImage.png");
}

SceneHowTo::~SceneHowTo()
{
	// 画像解放
	DeleteGraph(m_hImg);
}

void SceneHowTo::Update(const InputState& input)
{
	// 毎フレームカウント
	m_countFrame++;
	if (m_countFrame > 10000)
	{
		m_countFrame = 0;
	}

	// どこかのボタンが押されたらタイトルへ
	if (input.IsTriggered(InputType::all))
	{
		m_Manager.PopScene();
	}
}

void SceneHowTo::Draw()
{
	// 画像描画
	DrawExtendGraphF(0.0f, 0.0f, Game::kScreenWidth, Game::kScreenHeight, m_hImg, true);
	float stringLength = static_cast<float>(GetDrawFormatStringWidth(kTextBack));
	// 10フレームごとに点滅
	if ((m_countFrame / 10) % 6 != 0)
	{
		// テキスト描画
		DrawFormatStringF(Game::kScreenWidthHalf - (stringLength / 2), Game::kScreenHeight - 100, Game::kColorWhite, "%s", kTextBack);
	}
}
