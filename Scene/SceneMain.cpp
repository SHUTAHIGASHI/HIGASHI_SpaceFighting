#include "SceneMain.h"
// シーン関係
#include "SceneManager.h"
#include "SceneTitleMenu.h"
#include "ScenePauseMenu.h"
#include "SceneGameOver.h"
#include "SceneClear.h"
#include "SceneAllClear.h"
#include "SceneHowTo.h"
#include "SceneFade.h"
// ゲーム関係
#include "Game.h"
#include "CameraController.h"
#include "Player.h"
#include "StageManager.h"
#include "UiManager.h"
#include "EffekseerManager.h"
#include "SoundManager.h"
#include "GameDataManager.h"

namespace
{
	// ゲーム終了処理の実行フレーム数
	constexpr int kGameEndTimeCountMax = 120;
}

SceneMain::SceneMain(SceneManager& manager) :
	Scene(manager),
	m_updateFunc(&SceneMain::StartUpdate),
	m_drawFunc(&SceneMain::StartDraw),
	m_pCamera(std::make_shared<CameraController>()),
	m_pPlayer(std::make_shared<Player>()),
	m_pStage(std::make_shared<StageManager>()),
	m_pUi(std::make_shared<UiManager>()),
	m_isEnd(false),
	m_isDrawOperation(false),
	m_endTimeCount(0)
{
}

SceneMain::~SceneMain()
{
	// 再生中の音楽を停止
	SoundManager::GetInstance().StopBGM();
}

void SceneMain::Init()
{
	// ゲームポイントリセット
	GameDataManager::GetInstance().ResetPoint();

	// プレイヤーの初期化処理
	m_pPlayer->SetCamera(m_pCamera.get());
	m_pPlayer->SetUi(m_pUi.get());
	m_pPlayer->Init();

	// ステージの初期化処理
	m_pStage->SetPlayer(m_pPlayer.get());
	m_pStage->SetUi(m_pUi.get());
	m_pStage->Init();

	// カメラ初期化
	m_pCamera->SetPlayer(m_pPlayer.get());
	m_pCamera->Init();

	// 操作説明描画フラグ
	if (GameDataManager::GetInstance().IsFirstPlay())
	{
		m_isDrawOperation = true;
		GameDataManager::GetInstance().SetFirstPlay(false);
	}
}

void SceneMain::Update(const InputState& input)
{
	// Update処理のメンバ関数ポインタ
	(this->*m_updateFunc)(input);

	// UI更新処理
	m_pUi->Update();

	// エフェクトの更新処理
	EffekseerManager::GetInstance().Update();
}

void SceneMain::Draw()
{
	// ステージ描画
	m_pStage->Draw();
	// プレイヤー描画
	m_pPlayer->Draw();

	// Draw処理のメンバ関数ポインタ
	(this->*m_drawFunc)();

	// エフェクト描画
	EffekseerManager::GetInstance().Draw();
}

void SceneMain::End()
{
	// 再生中のエフェクト停止
	EffekseerManager::GetInstance().StopAllEffect();
}

void SceneMain::CheckStageEnd()
{
	// ステージ終了判定
	if (m_pStage->IsClear() || m_pPlayer->IsDead())
	{
		// ステージ終了時の処理
		m_pStage->OnStageEnd();
		// ゲーム終了時の処理
		OnGameEnd();
		return;
	}
}

void SceneMain::ChangeNextScene()
{
	// ステージクリア判定だったら
	if (m_pStage->IsClear())
	{
		// 全ステージクリアしていたら
		if (GameDataManager::GetInstance().IsAllStageClear())
		{
			// オールクリアシーンへ移行
			m_Manager.PushSceneAllUpdate(new SceneAllClear(m_Manager));
		}
		else
		{
			// 通常クリアシーンへ移行
			m_Manager.PushSceneAllUpdate(new SceneClear(m_Manager));
		}
	}
	else 
	{
		// ゲームオーバーシーンへ移行
		m_Manager.PushSceneAllUpdate(new SceneGameOver(m_Manager)); 
	}
}

void SceneMain::OnGameStart()
{
	// カメラが定位置についたら通常の処理へと移行
	m_updateFunc = &SceneMain::NormalUpdate;
	m_drawFunc = &SceneMain::NormalDraw;
	// クロスヘア表示
	m_pPlayer->SetCrosshair(true);
}

void SceneMain::OnGameEnd()
{
	// ゲーム終了処理の実行フレーム数を設定
	m_endTimeCount = kGameEndTimeCountMax;
	// ゲーム終了処理へ移行
	m_updateFunc = &SceneMain::EndUpdate;
	m_drawFunc = &SceneMain::EndDraw;
	// クロスヘア非表示
	m_pPlayer->SetCrosshair(false);
}

void SceneMain::NormalUpdate(const InputState& input)
{
	// ポーズメニューへ移行
	if (input.IsTriggered(InputType::pause))
	{
		// BGM停止
		SoundManager::GetInstance().StopBGM();
		// ポーズメニューシーンへ移行
		m_Manager.PushScene(new ScenePauseMenu(m_Manager));
		return;
	}

	// 音楽再生開始
	if (SoundManager::GetInstance().IsPlayingMusic() != 1)
	{
		SoundManager::GetInstance().PlayMusic(MusicType::main);
	}

	// プレイヤー更新処理
	m_pPlayer->Update(input);
	// ステージ更新処理
	m_pStage->Update(input);
	// カメラ更新処理
	m_pCamera->Update();

	// ステージ終了判定
	CheckStageEnd();
}

void SceneMain::StartUpdate(const InputState& input)
{
	// カメラ更新処理
	m_pCamera->StartUpdate();

	// カメラの開始時の処理を行う
	if (m_pCamera->IsCameraSet())
	{
		// ゲーム開始時の処理
		OnGameStart();
	}

	// 操作説明描画フラグが立っていたら
	if (m_isDrawOperation)
	{
		// 操作説明シーンを実行
		m_Manager.PushScene(new SceneHowTo(m_Manager));
		// 操作説明描画フラグを下げる
		m_isDrawOperation = false;
		return;
	}
}

void SceneMain::EndUpdate(const InputState& input)
{
	// ゲーム終了処理の実行フレーム数をカウント
	m_endTimeCount--;
	if (m_endTimeCount < 0)
	{
		// シーン終了
		ChangeNextScene();
		// リザルト表示時の更新処理へ移行
		m_updateFunc = &SceneMain::ResultUpdate;
		// カメラを初期位置へ
		m_pCamera->Init();
		return;
	}

	// プレイヤー更新処理
	m_pPlayer->Update(input);
	// ステージ更新処理
	m_pStage->Update(input);
	// カメラ更新処理
	m_pCamera->Update();
}

void SceneMain::ResultUpdate(const InputState& input)
{
	// プレイヤー更新処理
	m_pPlayer->Update(input);
	// ステージ更新処理
	m_pStage->Update(input);
	// カメラ更新処理
	m_pCamera->ResultUpdate();
}

void SceneMain::NormalDraw()
{
	// UI描画
	m_pUi->Draw();
}

void SceneMain::StartDraw()
{
	// 処理なし
}

void SceneMain::EndDraw()
{
	// 処理なし
}
