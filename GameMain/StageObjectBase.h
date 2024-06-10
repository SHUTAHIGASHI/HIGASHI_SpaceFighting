#pragma once
#include "ObjectBase.h"
#include "Game.h"
#include <memory>
#include <string>

// �I�u�W�F�N�g�̎��
enum class StageObjType
{
	Item,
	Rock,
	Enemy
};

class StageObjectBase : public ObjectBase
{
public:
	// �R���X�g���N�^
	StageObjectBase(StageObjType type, std::string name);
	// �f�X�g���N�^
	virtual ~StageObjectBase();

	// �X�V����
	virtual void Update();
	// �`�揈��
	void Draw();

	// �Փˎ��̏���
	virtual void OnCollision();

	// ���W�ݒ�
	void SetPos(VECTOR pos);
	// �A�E�g���C���ݒ�
	void SetOutline();
	// ������
	virtual void SetDisable() { m_status.isEnabled = false; }

	// �I�u�W�F�N�g�̎�ގ擾
	StageObjType GetType() const { return m_type; }
protected:
	// �I�u�W�F�N�g�̎��
	StageObjType m_type;
	// ���f��
	std::shared_ptr<class Model> m_pModel;
};

