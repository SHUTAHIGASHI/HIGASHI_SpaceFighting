#include "ObjectRock.h"
#include "Game.h"
#include "Model.h"

namespace
{
	// �g�嗦
	constexpr float kScale = 1.0f;
}

ObjectRock::ObjectRock() :
	StageObjectBase(StageObjType::Rock, "rock")
{
	// �g�嗦�ݒ�
	m_status.scale = kScale;
	// ���f���̃��[�J�����W�𒲐�
	m_pModel->SetModelLocalPos(VGet(0.0f, -144.0f, 0.0f));
	// �X�e�[�W�w�i�I�u�W�F�N�g�̊g�嗦�ݒ�
	m_pModel->SetScale(m_status.scale);
}

ObjectRock::~ObjectRock()
{ 
}
