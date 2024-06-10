#pragma once
#include "Game.h"
#include <vector>

class UiManager
{
public:
	// �R���X�g���N�^
	UiManager();
	// �f�X�g���N�^
	~UiManager();

	// �X�V
	void Update();
	// �`��
	void Draw();

	// �s�v��UI�폜����
	void DeleteUI();

	/* �`���[�W�V���b�g��UI */
	// �V���b�gUI�̐���
	void CreateShotUI(int maxShot);
	// �V���b�gUI�̍폜
	void DeleteShotUI();
	// �V���b�gUI�̍X�V
	void UpdateShotUI(int chargeNum);
	// �V���b�gUI�̕`��ʒu�ݒ�
	void SetShotDrawPos(float x, float y);
	// �V���b�gUI�擾
	class UiBar* GetChargeShotUI();

	/* �{�X��HP�o�[ */
	// �{�XUI�̐���
	void CreateBossUI(int maxHp);
	// �{�XUI�̍폜
	void DeleteBossUI();

	// �G���j���̍X�V
	void CountEnemyKill();

	// �|�C���g�l�����̏���
	void OnGetPoint(int point);
	// �|�C���g�r�����̏���
	void OnLosePoint(int point);

	// �X�e�[�W�i�s���̍X�V
	void SetStageRate(int rate);
	// �{�X��HP�`��ʒu�ݒ�
	void SetBossHpDrawPos(VECTOR pos);
	// �v���C���[��HP��ݒ�
	void SetPlayerHp(int hp) { m_playerHp = hp; }
	// �{�X��HP��ݒ�
	void SetBossHp(int hp) { m_bossHp = hp; }
	// �ڕW�G���j���̐ݒ�
	void SetEnemyKillCount(int count) { m_enemyKillCountMax = count; }
	// �`��e�L�X�g�ؑ֔���
	void SetIsBossRate(bool isBossRate) { m_isBossRate = isBossRate; }
private:
	// �X�e�[�W�ڕW�̕`��
	void DrawStageRate();
	// �X�e�[�W�ԍ��`��
	void DrawStageNum();

private:	
	// �`��e�L�X�g�ؑ֔���
	bool m_isBossRate;

	// �X�e�[�W�ԍ�
	int m_stageNum;
	// HP�o�[�̉摜
	int m_hHpBarImg;
	// �w�i�摜
	int m_hBgImg;
	// ���݂̊e��HP
	int m_playerHp;
	int m_bossHp;
	// �G���j��
	int m_enemyKillCount;
	int m_enemyKillCountMax;
	// �X�e�[�W�i�s��
	int m_stageRate;

	// �l�������|�C���g���ꎞ�I�ɕ`�悷��
	class UiPointBase* m_pPointUI = nullptr;
	// �v���C���[��HP�o�[
	class UiPlayerHpBar* m_pPlayerHp = nullptr;
	// �v���C���[�̃`���[�W�V���b�g
	class UiBar* m_pChargeShot = nullptr;
	// HP�o�[
	class UiBar* m_pBossHp = nullptr;
};
