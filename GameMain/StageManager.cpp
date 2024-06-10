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
	// �v���C���[�擾
	m_pStage = std::make_shared<Stage>(GameDataManager::GetInstance().GetStageData());
}

StageManager::~StageManager()
{
}

void StageManager::Init()
{
	// �}�b�v������
	m_pMap->Init();
	// �X�e�[�W������
	m_pStage->SetPlayer(m_pPlayer);
	m_pStage->SetUi(m_pUi);
	m_pStage->Init();
}

void StageManager::Update(const InputState& input)
{
	// �}�b�v�X�V
	m_pMap->SetBasePos(m_pPlayer->GetPos());
	m_pMap->Update();
	// �X�e�[�W�X�V
	m_pStage->Update();
}

void StageManager::Draw()
{
	// �}�b�v�`��
	m_pMap->Draw();
	// �X�e�[�W�`��
	m_pStage->Draw();
}

void StageManager::OnStageEnd()
{
	// �X�e�[�W�I�����̏���
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
