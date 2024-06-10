#include "StageManager.h"
#include "Stage.h"
#include "Map.h"
#include "Player.h"
#include "GameDataManager.h"

StageManager::StageManager() :
	m_pMap(std::make_shared<Map>()),
	m_pStage(nullptr),
	m_pPlayer(nullptr),
	m_pUi(nullptr)
{
	// プレイヤー取得
	m_pStage = std::make_shared<Stage>(GameDataManager::GetInstance().GetStageData());
}

StageManager::~StageManager()
{
}

void StageManager::Init()
{
	// マップ初期化
	m_pMap->Init();
	// ステージ初期化
	m_pStage->SetPlayer(m_pPlayer);
	m_pStage->SetUi(m_pUi);
	m_pStage->Init();
}

void StageManager::Update(const InputState& input)
{
	// マップ更新
	m_pMap->SetBasePos(m_pPlayer->GetPos());
	m_pMap->Update();
	// ステージ更新
	m_pStage->Update();
}

void StageManager::Draw()
{
	// マップ描画
	m_pMap->Draw();
	// ステージ描画
	m_pStage->Draw();
}

void StageManager::OnStageEnd()
{
	// ステージ終了時の処理
	m_pMap->OnStageEnd();
	m_pStage->OnStageEnd();
}

const EnemyBoss* StageManager::GetBoss() const
{
	return m_pStage->GetBoss();
}

const bool StageManager::IsClear() const
{
	return m_pStage->IsClear();
}
