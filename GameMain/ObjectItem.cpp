#include "ObjectItem.h"
#include "Game.h"
#include "Model.h"

namespace
{
	// �g�嗦
	constexpr float kScale = 0.5f;
}

ObjectItem::ObjectItem() :
	StageObjectBase(StageObjType::Item, "item")
{
	// �g�嗦�ݒ�
	m_status.scale = kScale;
	// �X�e�[�W�w�i�I�u�W�F�N�g�̊g�嗦�ݒ�
	m_pModel->SetScale(m_status.scale);
}

ObjectItem::~ObjectItem()
{
}

void ObjectItem::OnCollision()
{
	// �I�u�W�F�N�g�𖳌��ɂ���
	m_status.isEnabled = false;
}
