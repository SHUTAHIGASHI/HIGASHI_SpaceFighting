#pragma once
#include "Game.h"

struct StageData
{
	// �{�X�̏o������
	bool isBoss = false;
	// �{�X�̗�
	int bossHp = 0;
	// ��̐�
	int rockNum = 0;
	// �G���G�̐�
	int enemyNum = 0;
	// �X�e�[�W�̒���
	float length = 0.0f;
};

class Stage
{
public:
	// �R���X�g���N�^
	Stage(StageData stageData);
	// �f�X�g���N�^
	virtual ~Stage();

	// �v���C���[�|�C���^�Z�b�g
	void SetPlayer(class Player* player) { m_pPlayer = player; }
	// UI�|�C���^�Z�b�g
	void SetUi(class UiManager* ui) { m_pUi = ui; }

	// ������
	void Init();
	// �X�V
	void Update();
	// �`��
	void Draw();

	// ���j���J�E���g
	void CountKillEnemy();

	// �X�e�[�W�N���A���̏���
	void OnStageClear();
	// �X�e�[�W�I�����̏���
	void OnStageEnd();

	// �{�X�擾
	const class EnemyBoss* GetBoss() const
	{
		if (m_pBoss != nullptr) return m_pBoss;
		else return nullptr;
	}

	// �N���A����
	const bool IsClear() const { return m_isClear; }

private:
	// �I�u�W�F�N�g���̊Ǘ�
	void CheckObjectNum();
	// �X�e�[�W�N���A����`�F�b�N
	void CheckStageClear();
	// �G���G���Ǘ�
	void EnemyNumManager();
	// �{�X����
	void CreateBoss();
	// �{�X�̍X�V����
	void UpdateBoss();
	// �X�e�[�W�i�s����
	void OnChangeProgress();
	// �����_���Ɉʒu�w��
	VECTOR GetRandomPos();

private:
	// �X�e�[�W�f�[�^
	StageData m_stage;
	// �b���J�E���g
	int m_countFrame;
	// ���j���J�E���g
	int m_killEnemyNum;
	// �I�u�W�F�N�g�폜�͈�
	float m_deleteRange;
	// �X�e�[�W�̒���
	float m_stageLengthMax;
	// �N���A����
	bool m_isClear;

private:
	// �I�u�W�F�N�g�Ǘ�
	std::shared_ptr<class StageObjectManager> m_pObjectManager;

	// �{�X
	class EnemyBoss* m_pBoss;

	// �v���C���[
	class Player* m_pPlayer;

	// UI
	class UiManager* m_pUi;
};

