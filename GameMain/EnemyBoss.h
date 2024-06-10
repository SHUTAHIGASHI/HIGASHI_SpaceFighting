#pragma once
#include "ObjectBase.h"
#include <memory>
#include <vector>
#include "game.h"

class EnemyBoss : public ObjectBase
{
public:
	// �R���X�g���N�^
	EnemyBoss(VECTOR spawnPos, float speed, int maxHp);
	// �f�X�g���N�^
	~EnemyBoss();

	// �X�V
	void Update();
	// �`��
	void Draw();

	// �_���[�W����
	void OnDamage();
	// ���S���̏���
	void OnDead();

	// �{�X�ړ����x�ݒ�
	void SetSpeed(float speed) { m_status.moveSpeed = speed; }
	// �{�X�̍U���^�[�Q�b�g�ʒu�ݒ�
	void SetTargetPos(VECTOR targetPos) { m_targetPos = targetPos; }

	// Getter
	const std::vector<std::shared_ptr<class Shot>> GetShot() const { return m_pShots; }
	const int GetMaxHp() const { return m_maxHp; }

private:
	// �s�v�ȃV���b�g�̍폜
	void DeleteDisableShot();

	// �����o�֐��|�C���^(�X�V)
	using m_tUpdateFunc = void (EnemyBoss::*) ();
	m_tUpdateFunc m_updateFunc = nullptr;
	void SpawnUpdate();		// �ʏ�̍X�V����
	void NormalUpdate();	// �ʏ�̍X�V����
	void DeadUpdate();		// �Q�[���I�����̍X�V����
private:
	// �{�X�̌Œ�ʒu
	VECTOR m_fixedPos;
	// �U���^�[�Q�b�g���W
	VECTOR m_targetPos;
	// �p�x
	float m_angle;
	// ��]���x
	float m_rotaSpeed;
	// �U���Ԋu
	int m_shotDelay;
	// �{�X�̍ő�̗�
	int m_maxHp;
	// ���S����
	bool m_isDead;

	// ���f��
	std::shared_ptr<class Model> m_pModel;
	// �{�X�̍U��
	std::vector<std::shared_ptr<class Shot>> m_pShots;
};

