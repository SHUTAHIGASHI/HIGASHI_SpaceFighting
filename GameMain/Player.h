#pragma once
#include "ObjectBase.h"
#include "Game.h"
#include "StageObjectBase.h"
#include <vector>
#include <deque>

class Model;
class GameScore;
class CameraController;
class UiManager;

namespace PlayerInfo
{
	// �ő�HP
#ifdef _DEBUG
	constexpr int kHpMax = 30;
#else
	constexpr int kHpMax = 20;
#endif
}

class Player: public ObjectBase
{
public:
	// ���ꂼ��̃|�C���^�Z�b�g
	void SetScore(GameScore* score) { m_pScore = score; }
	void SetCamera(CameraController* camera) { m_pCamera = camera; }
	void SetUi(UiManager* ui) { m_pUi = ui; }

	// �R���X�g���N�^
	Player();
	// �f�X�g���N�^
	virtual ~Player();

	// ������
	void Init();
	// �X�V
	void Update(const InputState& input);
	// �`��
	void Draw();

	// ��_���[�W��
	void OnDamage();
	// �U�����ˎ�
	void OnAttack();
	// �`���[�W�V���b�g���ˎ�
	void OnChargeAttack();
	// ���S��
	void OnDead();
	// �Փˎ�
	void OnCollision(StageObjType type);
	// ��
	void OnHeal();

	// �N���X�w�A�`�攻��
	void SetCrosshair(bool i) { m_isDrawCrosshair = i; }

	// �e�擾�֐�
	const float GetDistance() const { return m_status.pos.z; }
	const int GetCharge() const { return 0; }
	std::vector<std::shared_ptr<class Shot>> GetShot() const { return m_pShot; }

private:// �v���C�x�[�g�֐�
	// ���͐���
	void ControllMove(const InputState& input);
	// �V���b�g�Ǘ�
	void ControllShot(const InputState& input);
	// �v���C���[�̈ړ��\�͈̓`�F�b�N
	void CheckMoveLimit();
	// ���S����`�F�b�N
	void CheckDead();
	// �N���X�w�A�`��
	void DrawCrosshair();
	// �v���C���[�̃V���b�g�폜
	void DeleteDisableShot();

	// �����o�֐��|�C���^(�X�V)
	using m_tUpdateFunc = void (Player::*) (const InputState& input);
	m_tUpdateFunc m_updateFunc = nullptr;
	void NormalUpdate(const InputState& input);     // �ʏ�̍X�V����
	void GameEndUpdate(const InputState& input);	// �Q�[���I�����̍X�V����

private:// �v���C�x�[�g�ϐ��F�f�[�^�֌W
	// �N���X�w�A�摜�n���h��
	int m_hCrosshairImg;
	int m_hCrosshairImg2;

private:// �v���C�x�[�g�ϐ��F�X�e�[�^�X�֌W
	// ����x�N�g��
	VECTOR m_avoidDir;
	// �v���C���[�̃V���b�g�x��
	int m_shotDelay;
	// �`���[�W�V���b�g��
	int m_chargeRate;
	// �`���[�W�V���b�g�J�E���g
	int m_chargeCount;
	// �`���[�W�V���b�g���ˎ���
	int m_chargeShotTime;
	// �`���[�W�V���b�g�{�^�������J�E���g
	int m_chargeShotButtonDelay;
	// ����J�E���g
	int m_avoidCount;
	// �������]���x
	float m_avoidRotaSpeed;
	// ��]�ʒu
	float m_playerRotaAxisZ;
	// ���
	bool m_isAvoid;
	// �N���X�w�A����
	bool m_isDrawCrosshair;

private:// �N���X�錾
	// �V���b�g
	std::vector<std::shared_ptr<class Shot>> m_pShot;
	// ���f��
	std::shared_ptr<Model> m_pModel;
	// �X�R�A
	GameScore* m_pScore;
	// �J����
	CameraController* m_pCamera;
	// UI
	UiManager* m_pUi;
};
