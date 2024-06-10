#include "SceneTitleMenu.h"
#include "SoundManager.h"
#include "SceneManager.h"
#include "SceneMain.h"
#include "SceneHowTo.h"
#include "SceneOption.h"
#include "SelectMenuBase.h"
#include "TitleDemo.h"
#include "Load.h"

namespace
{
	// ロケットのサイズ
	constexpr double kRocketSize = 0.1;
	// ロケットの回転速度
	constexpr float kRocketRotaSpeed = 0.0175f;
	// ロケットの中心からの距離
	constexpr float kRocketCenterDistance = 320.0f;
	// タイトルロゴ描画位置
	constexpr float kLogoDrawPosX = Game::kScreenWidthHalf;
	constexpr float kLogoDrawPosY = 340.0f;
	// テキスト描画位置
	constexpr float kTextDrawPosY = Game::kScreenHeightHalf + 200.0f;
	constexpr float kTextDrawSpace = Game::kFontSize;
	// テキスト
	const char* const kMenuTexts[] = { "ゲームスタート", "あそびかた", "設定", "終了" };
}

SceneTitleMenu::SceneTitleMenu(SceneManager& manager) : Scene(manager),
m_updateFunc(&SceneTitleMenu::NormalUpdate),
m_countFrame(0),
m_hTitleLogoImg(-1),
m_hTitleRocketImg(-1),
m_rocketPos(),
m_rocketDir(),
m_rocketRotation(DX_PI_F),
m_rocketAngle(90.0),
m_pSelectMenu(std::make_shared<SelectMenuBase>()),
m_pDemo(std::make_shared<TitleDemo>())
{
	// データ読み込み
	LoadData();
}

SceneTitleMenu::~SceneTitleMenu()
{
	// BGM停止
	SoundManager::GetInstance().StopBGM();
	// 画像ハンドル解放
	DeleteGraph(m_hTitleLogoImg);
	DeleteGraph(m_hTitleRocketImg);
}

void SceneTitleMenu::Init()
{
	// デモ初期化
	m_pDemo->Init();
	// 選択メニューの初期化
	m_pSelectMenu->Init(false);
	// 選択項目追加
	for (const auto& item : kMenuTexts)
	{
		m_pSelectMenu->AddSelectItem(item);
	}
	// 選択項目描画位置設定
	m_pSelectMenu->SetDrawPos(Game::kScreenWidthHalf, kTextDrawPosY);
	// ロケット座標初期化
	m_rocketPos = VGet(kLogoDrawPosX, kLogoDrawPosY + kRocketCenterDistance, 0.0f);
	m_rocketDir = VGet(0.0f, 0.0f, 0.0f);
}

void SceneTitleMenu::Update(const InputState& input)
{
	// デモ再生
	m_pDemo->Update();

	// 音楽再生開始
	if (SoundManager::GetInstance().IsPlayingMusic() != 1)
	{
		SoundManager::GetInstance().PlayMusic(MusicType::title);
	}

	// 毎フレームカウント
	m_countFrame++;
	// 更新処理のメンバ関数ポインタ
	(this->*m_updateFunc)(input);
}

void SceneTitleMenu::Draw()
{
	// デモ描画
	m_pDemo->Draw();
	// ロケット描画
	DrawRotaGraphF(m_rocketPos.x, m_rocketPos.y, kRocketSize, m_rocketRotation, m_hTitleRocketImg, true);
	// タイトルロゴ描画
	DrawRotaGraphF(kLogoDrawPosX, kLogoDrawPosY, 1.0, 0, m_hTitleLogoImg, true);
	// 選択項目描画
	m_pSelectMenu->Draw();
}

void SceneTitleMenu::End()
{
	// デモ終了
	m_pDemo->End();
}

void SceneTitleMenu::LoadData()
{
	// 画像読み込み
	m_hTitleLogoImg = LoadGraph("Data/ImageData/GAME_TITLE.png");
	m_hTitleRocketImg = LoadGraph("Data/ImageData/TitleRocket.png");
}

void SceneTitleMenu::UpdateRocket()
{
	// 回転数を毎フレーム変更
	m_rocketRotation += kRocketRotaSpeed;
	if (m_rocketRotation > DX_PI_F)
	{
		m_rocketRotation = -DX_PI_F;
	}
	// 角度の値を増やす
	m_rocketAngle += 1.0f;

	// 角度の最大値(  360°)を超えた場合、0にする
	if (m_rocketAngle >= 360.0f)
	{
		m_rocketAngle = 0.0f;
	}
	// 角度をラジアンに変換
	float rad = m_rocketAngle * (DX_PI_F / 180.0f);

	// 中心位置から半径をもとに軌道を計算
	m_rocketDir.x = cos(rad) * kRocketCenterDistance;
	m_rocketDir.y = sin(rad) * kRocketCenterDistance;

	// 中心位置の代入
	m_rocketPos = VGet(kLogoDrawPosX, kLogoDrawPosY, 0.0f);

	// ベクトルを位置に加算
	m_rocketPos = VAdd(m_rocketPos, m_rocketDir);
}

void SceneTitleMenu::OnSceneEnd()
{
	// 選択項目によってシーン遷移
	if (m_pSelectMenu->GetSelectedNum() == 0)
	{
		m_Manager.ChangeScene(new SceneMain(m_Manager));
	}
	else if (m_pSelectMenu->GetSelectedNum() == 1)
	{
		m_Manager.PushScene(new SceneHowTo(m_Manager));
	}
	else if (m_pSelectMenu->GetSelectedNum() == 2)
	{
		m_Manager.PushScene(new SceneOption(m_Manager));
	}
	else if (m_pSelectMenu->GetSelectedNum() == 3)
	{
		m_Manager.GameEnd();
	}
	return;
}

void SceneTitleMenu::NormalUpdate(const InputState& input)
{
	// 選択項目描画
	m_pSelectMenu->Update(input);
	// 演出ロケット更新
	UpdateRocket();

	if (input.IsTriggered(InputType::select))
	{
		SoundManager::GetInstance().PlaySE(SoundType::select);
		OnSceneEnd();
		return;
	}
}
