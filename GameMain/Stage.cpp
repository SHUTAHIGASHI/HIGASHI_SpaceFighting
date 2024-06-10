#include "Stage.h"
#include "StageObjectBase.h"
#include "StageObjectManager.h"
#include "EnemyBoss.h"
#include "Player.h"
#include "UiManager.h"
#include "GameDataManager.h"
#include "Shot.h"

namespace
{
	// 背景オブジェクトの削除距離
	constexpr float kDeleteRange = 1000.0f;
	constexpr float kDeleteRangeAfterEnd = 12000.0f;
	// オブジェクトの生成距離
	constexpr float kObjectCreateDistance = 10000.0f;
	// オブジェクトの生成範囲
	constexpr float kObjectCreateRangeX = Game::kStageWidth * 2;
	constexpr float kObjectCreateRangeY = Game::kStageHeight * 2;
	constexpr float kObjectCreateRangeZ = 10000.0f;
	// 敵の同時沸き最大数
	constexpr int kEnemyMaxNum = 5;
}

Stage::Stage(StageData stageData) :
	m_stage(stageData),
	m_countFrame(0),
	m_killEnemyNum(0),
	m_deleteRange(kDeleteRange),
	m_stageLengthMax(stageData.length),
	m_isClear(false),
	m_pObjectManager(std::make_shared<StageObjectManager>()),
	m_pBoss(nullptr),
	m_pPlayer(nullptr),
	m_pUi(nullptr)
{
}

Stage::~Stage()
{
	// ボス削除
	if (m_pBoss != nullptr)
	{
		delete m_pBoss;
		m_pBoss = nullptr;
	}
}

void Stage::Init()
{
	// オブジェクト管理初期化
	m_pObjectManager->SetStage(this);
	m_pObjectManager->SetUi(m_pUi);
	m_pObjectManager->Init();
	// 岩生成
	for (int i = 0; i < m_stage.rockNum; i++)
	{
		// 岩の生成
		m_pObjectManager->CreateObject("rock", GetRandomPos());
	}
	// UIに敵撃破数を設定
	m_pUi->SetEnemyKillCount(m_stage.enemyNum);
	m_pUi->SetIsBossRate(false);
}

void Stage::Update()
{
	// プレイヤーの位置を設定
	m_pObjectManager->SetPlayerPos(m_pPlayer->GetPos());
	// オブジェクト管理更新
	m_pObjectManager->Update();
	// プレイヤーとオブジェクトの当たり判定
	m_pObjectManager->CheckPlayerCollision(m_pPlayer);

	// オブジェクトの座標再設定
	for (auto& obj : m_pObjectManager->GetStageObject())
	{
		if (obj->GetPos().z < m_pPlayer->GetPos().z - m_deleteRange)
		{
			obj->SetPos(GetRandomPos());
		}
	}

	// オブジェクト数管理
	CheckObjectNum();

	// ボス更新
	if (m_pBoss != nullptr)
	{
		UpdateBoss();
		m_pUi->SetBossHp(m_pBoss->GetHP());
		m_pUi->SetBossHpDrawPos(m_pBoss->GetPos());
	}
	else
	{
		m_stage.length -= m_pPlayer->GetSpeed();
		if (m_stage.length <= 0.0f && m_killEnemyNum >= m_stage.enemyNum)
		{
			// ステージ進行度リセット
			m_stage.length = 0.0f;
			// フェーズ変更
			if (!m_isClear)
			{
				OnChangeProgress();
			}
		}
		m_pUi->SetStageRate(static_cast<int>(100 - (m_stage.length / m_stageLengthMax * 100)));
	}

	// 雑魚敵数管理
	EnemyNumManager();

	// ステージクリア判定チェック
	CheckStageClear();
}

void Stage::Draw()
{
	// オブジェクト管理描画
	m_pObjectManager->Draw();
	// ボス描画
	if (m_pBoss != nullptr)
	{
		m_pBoss->Draw();
	}
}

void Stage::CountKillEnemy()
{
	m_killEnemyNum++;
	m_pUi->CountEnemyKill();
}

void Stage::OnStageClear()
{
	// ステージクリア時の処理
	if (!m_isClear)
	{
		GameDataManager::GetInstance().OnStageClear();
	}

	// クリア判定
	m_isClear = true;
}

void Stage::OnStageEnd()
{
	// 背景オブジェクトの削除範囲を変更
	m_deleteRange = kDeleteRangeAfterEnd;
}

void Stage::CheckObjectNum()
{
	int objCount = 0;
	// オブジェクト数が設定数より少ない場合、生成
	for (auto& obj : m_pObjectManager->GetStageObject())
	{
		if (obj->GetType() == StageObjType::Rock)
		{
			objCount++;
		}
	}

	if (objCount < m_stage.rockNum)
	{
		for (int i = 0; i < m_stage.rockNum - objCount; i++)
		{
			// 敵の生成
			m_pObjectManager->CreateObject("rock", GetRandomPos());
		}
	}
}

void Stage::CheckStageClear()
{
	// ボスが存在しないステージの場合、ステージの長さが0になったらクリア
	if (m_stage.length <= 0.0f && m_killEnemyNum >= m_stage.enemyNum)
	{
		// ボスが存在するステージの場合
		if (m_stage.isBoss)
		{
			if (m_pBoss != nullptr)
			{
				// ボスが無効化された場合、ボスを削除
				if (!m_pBoss->IsEnabled())
				{
					// ステージクリア処理
					OnStageClear();
					// ボス削除
					delete m_pBoss;
					m_pBoss = nullptr;
					// ボスUI削除
					m_pUi->DeleteBossUI();
					// スコア加算
					GameDataManager::GetInstance().AddPoint(ScoreInfo::BOSS_SCORE);
				}
			}
		}
		else
		{
			m_stage.length = 0.0f;
			// ステージクリア処理
			OnStageClear();
		}
	}
}

void Stage::EnemyNumManager()
{
	// 敵の同時沸き数をカウント
	int countEnemy = 0;
	for (auto& obj : m_pObjectManager->GetStageObject())
	{
		if (obj->GetType() == StageObjType::Enemy)
		{
			countEnemy++;
		}
	}

	// 敵の生成
	if (countEnemy < kEnemyMaxNum)
	{
		m_pObjectManager->CreateObject("enemy", GetRandomPos());
	}
}

void Stage::CreateBoss()
{
	// ボスの生成座標
	VECTOR bossPos = VGet(0.0f, 0.0f, m_pPlayer->GetDistance() + 4000.0f);
	// フェーズ変更
	m_pBoss = new EnemyBoss(bossPos, m_pPlayer->GetSpeed(), m_stage.bossHp);
	// ボスUI設定
	m_pUi->CreateBossUI(m_pBoss->GetMaxHp());
	m_pUi->SetBossHpDrawPos(m_pBoss->GetPos());
	m_pUi->SetIsBossRate(true);
}

void Stage::UpdateBoss()
{
	m_pBoss->SetSpeed(m_pPlayer->GetSpeed());
	m_pBoss->SetTargetPos(m_pPlayer->GetPos());
	m_pBoss->Update();

	// プレイヤーのショットとボスの当たり判定チェック
	for (auto& shot : m_pPlayer->GetShot())
	{
		if (m_pBoss->CheckCollision(shot.get()))
		{
			// 被ダメージ時の処理
			m_pBoss->OnDamage();
			// ショット削除
			shot->OnHit();
		}
	}

	// ボスのショットとプレイヤーの当たり判定チェック
	for (auto& shot : m_pBoss->GetShot())
	{
		if (m_pPlayer->CheckCollision(shot.get()))
		{
			// 被ダメージ時の処理
			m_pPlayer->OnDamage();
			// ショット削除
			shot->OnHit();
		}
	}
}

void Stage::OnChangeProgress()
{
	if (m_stage.isBoss)
	{
		CreateBoss();
	}
}

VECTOR Stage::GetRandomPos()
{
	// オブジェクトの位置を設定
	VECTOR resultPos = Game::kVecZero;
	resultPos.x = static_cast<float>(GetRand(static_cast<int>(kObjectCreateRangeX)) - kObjectCreateRangeX / 2);
	resultPos.y = static_cast<float>(GetRand(static_cast<int>(kObjectCreateRangeY)) - kObjectCreateRangeY / 2);
	resultPos.z = m_pPlayer->GetPos().z + kObjectCreateRangeZ + static_cast<float>(GetRand(static_cast<int>(kObjectCreateRangeZ)));

	return resultPos;
}
