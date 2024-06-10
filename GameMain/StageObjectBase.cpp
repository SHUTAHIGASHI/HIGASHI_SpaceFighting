#include "StageObjectBase.h"
#include "Model.h"
#include "EffekseerManager.h"
#include "SoundManager.h"
#include "Load.h"

namespace
{
	// �����蔻�蒲�����l
	constexpr float kCollRadius = 128.0f;
	// ���f���̃A�E�g���C���T�C�Y
	constexpr float kOutlineSize = 0.5f;
	// ���f����]���x
	constexpr float kRotaSpeed = 0.01f;
}

StageObjectBase::StageObjectBase(StageObjType type, std::string name) :
	m_type(type),
	m_pModel(nullptr)
{
	// �����蔻��ݒ�
	m_status.radius = kCollRadius;
	// �X�e�[�^�X������
	m_status.hModel = MV1DuplicateModel(Load::GetInstance().GetHandle(name));
	// ���f������
	m_pModel = std::make_shared<Model>(m_status.hModel);
}

StageObjectBase::~StageObjectBase()
{
	// ���f���̍폜
	MV1DeleteModel(m_status.hModel);
}

void StageObjectBase::Update()
{
	// ��]�p�x�̍X�V
	m_status.rota.y += kRotaSpeed;
	if(m_status.rota.y > DX_PI_F) m_status.rota.y = -DX_PI_F;

	// ���f���̍X�V
	m_pModel->SetPosition(m_status.pos);
	m_pModel->SetRotation(m_status.rota);
	m_pModel->Update();
}

void StageObjectBase::Draw()
{
	// ���f���̕`��
	m_pModel->Draw();
#ifdef _DEBUG
	DrawSphere3D(m_status.pos, m_status.radius, 16.0f, 0xffffff, 0xffffff, false);
#endif
}

void StageObjectBase::SetPos(VECTOR pos)
{
	// ���W�̐ݒ�
	m_status.pos = pos;
	// ���f���̍��W�ݒ�
	m_pModel->SetPosition(m_status.pos);
}

void StageObjectBase::SetOutline()
{
	// �A�E�g���C���̐ݒ�
	m_pModel->SetOutline(1, kOutlineSize, 255, 0, 0, 255);
}

void StageObjectBase::OnCollision()
{
	// �I�u�W�F�N�g�̖�����
	m_status.isEnabled = false;
	// �_���[�W���Đ�
	SoundManager::GetInstance().PlaySE(SoundType::damage);
	// �_���[�W�G�t�F�N�g����
	EffekseerManager::GetInstance().CreateEffect(EffectType::Damage, false, m_status.pos);
}
