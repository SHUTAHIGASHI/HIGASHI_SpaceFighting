#include "Player.h"
#include "Model.h"
#include "Shot.h"
#include "SceneMain.h"
#include "CameraController.h"
#include "UiManager.h"
#include "GameDataManager.h"
#include "EffekseerManager.h"
#include "SoundManager.h"
#include "UiManager.h"
#include "Load.h"

using namespace PlayerInfo;

namespace
{
	// �g�嗦
	constexpr float kScale = 2.0f;
	// �X�^�[�g�ʒu���W
	constexpr float kStartPosX = 0.0f;
	constexpr float kStartPosY = -600.0f;
	constexpr float kStartPosZ = 0.0f;
	// �v���C���[���a
	constexpr float kPlayerRadius = 128.0f;
	// �ړ��̐��l
	constexpr float kBaseSpeed = 32.0f;
	constexpr float kMoveSpeed = 24.0f;
	// �v���C���[�X���̐��l
	constexpr float kAngleChangeAddNum = 0.03f;
	constexpr float kAngleChangeSubNum = 0.02f;
	constexpr float kAngleChangeMax = 0.5f;
	
	// HP�񕜗�
	constexpr int kRepairNum = 2;

	// ���G���Ԓ萔
	constexpr int kInvTimeMax = 60;
	// �U���x������
	constexpr int kShotDelay = 6;

	// �`���[�W�V���b�g�̍ő吔
	constexpr int kChargeShotMax = 60;
	// �`���[�W�V���b�g���ˎ���
	constexpr int kChargeShotTime = 10;
	// �`���[�W�V���b�g�̃{�^����������
	constexpr int kChargeShotButtonDelay = 30;
	// �`���[�W�V���b�g�Q�[�W�`��ʒu
	constexpr float kChargeShotDrawPosX = Game::kScreenWidthHalf;
	constexpr float kChargeShotDrawPosY = Game::kScreenHeightHalf + 300.0f;

	// �������
	constexpr int kAvoidTime = 20;
	// ������̈ړ����x
	constexpr float kAvoidSpeed = 2.0f;
	// ������̉�]���x
	constexpr float kAvoidRotaSpeed = 0.3f;
}

Player::Player() :
	m_updateFunc(&Player::NormalUpdate),
	m_hCrosshairImg(-1),
	m_hCrosshairImg2(-1),
	m_avoidDir(Game::kVecZero),
	m_shotDelay(0),
	m_chargeRate(0),
	m_chargeCount(0),
	m_chargeShotTime(0),
	m_chargeShotButtonDelay(0),
	m_avoidCount(0),
	m_avoidRotaSpeed(0.0f),
	m_playerRotaAxisZ(0.0f),
	m_isAvoid(false),
	m_isDrawCrosshair(false),
	m_pModel(nullptr),
	m_pCamera(nullptr),
	m_pScore(nullptr),
	m_pUi(nullptr)
{
	// �摜�f�[�^�Ǎ�
	m_hCrosshairImg = LoadGraph("Data/ImageData/crosshair.png");
	m_hCrosshairImg2 = LoadGraph("Data/ImageData/crosshair_.png");
	// ���f���ǂݍ���
	m_pModel = std::make_shared<Model>(MV1DuplicateModel(Load::GetInstance().GetHandle("ship")));
}

Player::~Player()
{
	// ���f���n���h���폜
	MV1DeleteModel(m_status.hModel);
	// �N���X�w�A�摜�n���h���폜
	DeleteGraph(m_hCrosshairImg);
	DeleteGraph(m_hCrosshairImg2);
}

void Player::Init()
{
	// ���W������
	m_status.pos = VGet(kStartPosX, kStartPosY, kStartPosZ);
	// HP�����l���
	m_status.hp = kHpMax;
	// �v���C���[�ړ����x������
	m_status.moveSpeed = kBaseSpeed;
	// �v���C���[����������
	m_playerRotaAxisZ = 0.0f;
	// �v���C���[�̓����蔻�蔼�a
	m_status.radius = kPlayerRadius;
	m_status.scale = kScale;

	// ���W�ݒ�
	m_pModel->SetPosition(m_status.pos);
	// �g�嗦�ݒ�
	m_pModel->SetScale(m_status.scale);

	// �G�t�F�N�g����
	EffekseerManager::GetInstance().CreateEffect(EffectType::EngineFire, true, this);
}

void Player::Update(const InputState& input)
{
	// Update�����̃����o�֐��|�C���^
	(this->*m_updateFunc)(input);
	// �e�̍X�V
	for (auto& shot : m_pShot)
	{
		shot->Update();
	}
	// �e�̍폜
	DeleteDisableShot();

	// �v���C���[���f���`��ʒu�ݒ�
	m_pModel->SetPosition(m_status.pos);
	m_pModel->SetRotation(m_status.rota);
	// �G�t�F�N�g�X�V
	EffekseerManager::GetInstance().SetEffectRota(EffectType::EngineFire, VGet(0.0f, 0.0f, m_playerRotaAxisZ));

	// UI�X�V
	m_pUi->SetPlayerHp(m_status.hp);
}

void Player::Draw()
{
	// �v���C���[���f���`��
	if (m_status.invTime % 2 == 0)
	{
		m_pModel->Draw();
	}

	// �e�̕`��
	for (auto& shot : m_pShot)
	{
		shot->Draw();
	}

	// �N���X�w�A�`��
	if (m_isDrawCrosshair)
	{
		DrawCrosshair();
	}
}

void Player::ControllMove(const InputState& input)
{
#ifdef _DEBUG
	if (CheckHitKey(KEY_INPUT_H)) OnDamage();
	if (CheckHitKey(KEY_INPUT_M)) m_status.invTime = 10000;
#endif
	// ��ɑO�ɐi��
	VECTOR frontDir = VGet(0.0f, 0.0f, 1.0f);
	if (VSize(frontDir) > 0) frontDir = VNorm(frontDir);
	frontDir = VScale(frontDir, m_status.moveSpeed);
	m_status.pos = VAdd(m_status.pos, frontDir);

	// �ړ��L�[�̓��͂�����Ă��邩
	bool isSideMove = false, isRight = false;
	// �㉺���E�̈ړ�����
	m_status.dir = VGet(0.0f, 0.0f, 0.0f);

	if (!m_isAvoid)
	{
		if (input.IsPressed(InputType::up))		// �����
		{
			m_status.dir.y += 1.0f;
		}
		if (input.IsPressed(InputType::down))	// ������
		{
			m_status.dir.y -= 1.0f;
		}
		if (input.IsPressed(InputType::right))	// �E����
		{
			m_status.dir.x += 1.0f;
			m_playerRotaAxisZ += -kAngleChangeAddNum;
			if (m_playerRotaAxisZ < -kAngleChangeMax)
			{
				m_playerRotaAxisZ = -kAngleChangeMax;
			}
			isSideMove = true;
			isRight = true;
		}
		if (input.IsPressed(InputType::left))	// ������
		{
			m_status.dir.x -= 1.0f;
			m_playerRotaAxisZ += kAngleChangeAddNum;
			if (m_playerRotaAxisZ > kAngleChangeMax)
			{
				m_playerRotaAxisZ = kAngleChangeMax;
			}
			isSideMove = true;
			isRight = false;
		}
	}

	// �ړ������̐��K��
	if (VSize(m_status.dir) > 0) m_status.dir = VNorm(m_status.dir);
	// �ړ������ɑ��x���|����
	m_status.dir = VScale(m_status.dir, kMoveSpeed);

	// �������
	if (isSideMove)
	{
		if (input.IsTriggered(InputType::avoid) && !m_isAvoid)
		{
			// ��𒆂̃t���O�𗧂Ă�
			m_isAvoid = true;
			// �������̐ݒ�
			m_avoidDir = m_status.dir;
			// ����J�E���g��ݒ�
			m_avoidCount = kAvoidTime;
			// ���͂���Ă�������ɉ�]
			if (isRight)
			{
				m_avoidRotaSpeed = -kAvoidRotaSpeed;
			}
			else
			{
				m_avoidRotaSpeed = kAvoidRotaSpeed;
			}
			// ����G�t�F�N�g
			EffekseerManager::GetInstance().CreateEffect(EffectType::Avoid, false, this);
		}
	}

	// ��𒆂̏ꍇ
	if (m_isAvoid)
	{
		// ����J�E���g�����炷
		m_avoidCount--;
		// �������ɑ��x���|����
		m_status.dir = VScale(m_avoidDir, kAvoidSpeed);
		// ������̉�]��ݒ�
		m_playerRotaAxisZ += m_avoidRotaSpeed;
		if(m_playerRotaAxisZ > DX_PI_F) m_playerRotaAxisZ = -DX_PI_F;
		if(m_playerRotaAxisZ < -DX_PI_F) m_playerRotaAxisZ = DX_PI_F;
		if (m_avoidCount == 1)
		{
			m_playerRotaAxisZ = 0.0f;
		}
		// ����J�E���g��0�ȉ��ɂȂ����ꍇ
		if (m_avoidCount < 0)
		{
			// ��𒆂̃t���O��������
			m_isAvoid = false;
		}
	}
	else
	{
		// ��𒆂łȂ��ꍇ�͉�������������
		m_avoidDir = Game::kVecZero;
		m_avoidRotaSpeed = 0.0f;
	}
	// �v���C���[�̍��W���X�V
	m_status.pos = VAdd(m_status.pos, m_status.dir);

	// �ړ��L�[�̓��͂�����Ă��Ȃ��ꍇ
	if (!isSideMove && !m_isAvoid)
	{
		if (m_playerRotaAxisZ < -kAngleChangeSubNum)
		{
			m_playerRotaAxisZ += kAngleChangeSubNum;
		}
		else if (m_playerRotaAxisZ > kAngleChangeSubNum)
		{
			m_playerRotaAxisZ += -kAngleChangeSubNum;
		}
		else
		{
			m_playerRotaAxisZ = 0.0f;
		}
	}
	// �v���C���[�̉�]��ݒ�
	m_status.rota = VGet(0.0f, 0.0f, m_playerRotaAxisZ);

	// �v���C���[�̈ړ��͈͂𐧌�����
	CheckMoveLimit();
}

void Player::ControllShot(const InputState& input)
{
	// �v���C���[�̍U������
	if (input.IsTriggered(InputType::shot))
	{
		if (m_shotDelay < 0 && !m_isAvoid)
		{
			OnAttack();
			m_shotDelay = kShotDelay;
		}
	}

	// �`���[�W�V���b�g����
	if (input.IsPressed(InputType::shot))
	{
		// �`���[�W�V���b�g�{�^�������J�E���g�𑝂₷
		m_chargeShotButtonDelay++;
		if (m_chargeShotButtonDelay > kChargeShotButtonDelay)
		{
			// �`���[�W�V���b�g�J�E���g�𑝂₷
			m_chargeCount++;
			if (m_chargeCount >= kChargeShotMax)
			{
				m_chargeCount = kChargeShotMax;
			}
		}
	}
	else
	{
		// �`���[�W�V���b�g�J�E���g����萔�ȏ�̏ꍇ
		if (m_chargeCount > kChargeShotMax * 0.3)
		{
			// �`���[�W�V���b�g����
			m_chargeShotTime = kChargeShotTime;
			m_chargeRate = m_chargeCount;
		}
		// �`���[�W�V���b�g�J�E���g��������
		m_chargeCount = 0;
		// �`���[�W�V���b�g�{�^�������J�E���g��������
		m_chargeShotButtonDelay = 0;
	}

	if (m_chargeShotTime > 0)
	{
		m_chargeShotTime--;
		// ��𒆈ȊO
		if (!m_isAvoid)
		{
			// �`���[�W�V���b�g����
			OnChargeAttack();
		}
	}

	// �`���[�W�V���b�gUI
	if (m_chargeCount > 0)
	{
		// �`���[�W�V���b�gUI���Ȃ��ꍇ�͍쐬
		if (m_pUi->GetChargeShotUI() == nullptr)
		{
			m_pUi->CreateShotUI(kChargeShotMax);
		}
		// �`���[�W�V���b�gUI�̍X�V
		m_pUi->UpdateShotUI(m_chargeCount);
		m_pUi->SetShotDrawPos(kChargeShotDrawPosX, kChargeShotDrawPosX);
		// �`���[�W�G�t�F�N�g�Đ�
		if (!EffekseerManager::GetInstance().IsPlayingEffect(EffectType::Charge))
		{
			EffekseerManager::GetInstance().CreateEffect(EffectType::Charge, false, this);
		}
	}
	else
	{
		// �`���[�W�V���b�gUI������ꍇ�͍폜
		m_pUi->DeleteShotUI();
		// �`���[�W�G�t�F�N�g��~
		EffekseerManager::GetInstance().StopEffect(EffectType::Charge);
	}
}

void Player::OnDamage()
{
	if(m_avoidCount > 0) return;

	// ���G���Ԃ̂Ƃ��͏����͍s��Ȃ�
	if (m_status.invTime > 0) return;
	// ���G���ԃZ�b�g
	m_status.invTime = kInvTimeMax;

	// HP������������
	m_status.hp--;
	if (m_status.hp < 0) m_status.hp = 0;

	// HP��0�ȏ�̏ꍇ
	if (m_status.hp >= 0)
	{
		// �_���[�W�G�t�F�N�g�Đ�
		EffekseerManager::GetInstance().CreateEffect(EffectType::Damage, false, this);
		// �_���[�W���Đ�
		SoundManager::GetInstance().PlaySE(SoundType::damage);
		// ��_���[�W���̃J��������
		m_pCamera->OnScreenQuake();
		// ��_���[�W���̃X�R�A����
		GameDataManager::GetInstance().SubPoint(ScoreInfo::MISS_SCORE);
		// ��_���[�W����UI����
		m_pUi->OnLosePoint(ScoreInfo::MISS_SCORE);
	}
}

void Player::OnAttack()
{
	VECTOR target = VGet(m_status.pos.x, m_status.pos.y, m_status.pos.z + 100.0f);
	m_pShot.push_back(std::make_shared<Shot>(m_status.pos, target));
	// �U�����Đ�
	SoundManager::GetInstance().PlaySE(SoundType::shot);
	// �U���G�t�F�N�g�Đ�
	EffekseerManager::GetInstance().CreateEffect(EffectType::Shot, false, m_pShot.back().get());
}

void Player::OnChargeAttack()
{
	// �`���[�W�V���b�g�g�嗦
	float scale = static_cast<float>(m_chargeRate / 10);
	// �`���[�W�V���b�g�̃^�[�Q�b�g���W
	VECTOR target = VGet(m_status.pos.x, m_status.pos.y, m_status.pos.z + 100.0f);
	// �`���[�W�V���b�g����
	m_pShot.push_back(std::make_shared<Shot>(m_status.pos, target));
	m_pShot.back()->SetScale(scale);
	// �U�����Đ�
	SoundManager::GetInstance().PlaySE(SoundType::shot);
	// �U���G�t�F�N�g�Đ�
	EffekseerManager::GetInstance().CreateEffectAndSetScale(EffectType::ChargeShot, false, m_pShot.back().get(), scale);
}

void Player::OnDead()
{
	// ���S���Đ�
	SoundManager::GetInstance().PlaySE(SoundType::dead);
	// ���S�����true��
	m_status.isDead = true;
	// ���S���̃G�t�F�N�g����
	EffekseerManager::GetInstance().CreateEffect(EffectType::Dead, false, this);
	// �����o�֐��|�C���^���Q�[���I�[�o�[��̏����ɕύX
	m_updateFunc = &Player::GameEndUpdate;
}

void Player::OnCollision(StageObjType type)
{
	// �G���ɓ��������ꍇ
	if (type == StageObjType::Rock || type == StageObjType::Enemy)
	{
		// ��_���[�W��
		OnDamage();
	}
	// �A�C�e���ɓ��������ꍇ
	else if (type == StageObjType::Item)
	{
		// ��
		OnHeal();
	}
}

void Player::OnHeal()
{
	// HP��
	m_status.hp += kRepairNum;
	if (m_status.hp > kHpMax) m_status.hp = kHpMax;
	// �񕜃G�t�F�N�g�Đ�
	EffekseerManager::GetInstance().CreateEffect(EffectType::Heal, false, this);
	// �񕜉��Đ�
	SoundManager::GetInstance().PlaySE(SoundType::boost);
	// �񕜎��̃X�R�A���Z
	GameDataManager::GetInstance().AddPoint(ScoreInfo::HEAL_SCORE);
	// �񕜎���UI����
	m_pUi->OnGetPoint(ScoreInfo::HEAL_SCORE);
}

void Player::CheckMoveLimit()
{
	// �v���C���[�̈ړ��͈͂𐧌�����
	if (m_status.pos.x > Game::kStageWidth) m_status.pos.x = Game::kStageWidth;
	if (m_status.pos.x < -Game::kStageWidth) m_status.pos.x = -Game::kStageWidth;
	if (m_status.pos.y > Game::kStageHeight) m_status.pos.y = Game::kStageHeight;
	if (m_status.pos.y < -Game::kStageHeight) m_status.pos.y = -Game::kStageHeight;
}

void Player::CheckDead()
{
	// ���G���Ԃł͂Ȃ�
	if (m_status.invTime <= 0)
	{
		// HP��0�ȉ�
		if (m_status.hp <= 0)
		{
			// ���S���Ă��Ȃ�����
			if (!m_status.isDead)
			{
				// ���S�������s��
				OnDead();
			}
		}
	}
}

void Player::DrawCrosshair()
{
	// ��O�̃N���X�w�A
	VECTOR pos = VGet(m_status.pos.x, m_status.pos.y, m_status.pos.z + 800.0f);
	pos = ConvWorldPosToScreenPos(pos);
	DrawRotaGraphF(pos.x, pos.y, 1.0f, 0.0f, m_hCrosshairImg2, true);
	// ���̃N���X�w�A
	pos = VGet(m_status.pos.x, m_status.pos.y, m_status.pos.z + 6500.0f);
	pos = ConvWorldPosToScreenPos(pos);
	DrawRotaGraphF(pos.x, pos.y, 1.0f, 0.0f, m_hCrosshairImg, true);
}

void Player::DeleteDisableShot()
{
	// ���Ȃ��Ȃ����V���b�g�͔r��
	auto rmIt = std::remove_if(m_pShot.begin(), m_pShot.end(),
		[](const std::shared_ptr<Shot>& shot)
		{
			return !shot->IsEnabled();
		});

	// ���ۂɔ͈͂��w�肵�č폜
	m_pShot.erase(rmIt, m_pShot.end());
}

void Player::NormalUpdate(const InputState& input)
{
	// ���G���Ԃ��c���Ă���ꍇ�͖��t���[���}�C�i�X����
	if (m_status.invTime > 0) m_status.invTime--;

	// �U���x�����Ԃ����炷
	m_shotDelay--;

	// ���S����`�F�b�N
	CheckDead();

	// ��{�̈ړ�����
	ControllMove(input);

	// �U������
	ControllShot(input);
}

void Player::GameEndUpdate(const InputState& input)
{
	// ��ɑO�ɐi��
	m_status.dir = VGet(0.0f, 0.0f, 1.0f);
	if (VSize(m_status.dir) > 0) m_status.dir = VNorm(m_status.dir);
	float playerMoveSpeed = m_status.moveSpeed;
	m_status.dir = VScale(m_status.dir, playerMoveSpeed / 2);
	m_status.pos = VAdd(m_status.pos, m_status.dir);
	// �v���C���[�̍��W���X�V
	EffekseerManager::GetInstance().SetEffectPosition(EffectType::Dead, m_status.pos);
}
