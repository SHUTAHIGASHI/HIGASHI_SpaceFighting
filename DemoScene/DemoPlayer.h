#pragma once
#include "ObjectBase.h"
#include <memory>

class DemoPlayer : public ObjectBase
{
public:
	// �R���X�g���N�^
	DemoPlayer();
	// �f�X�g���N�^
	~DemoPlayer();

	// �X�V
	void Update();
	// �`��
	void Draw();

private:
	// ��]����
	void UpdateRot();
	// �ړ�����
	void UpdateMove();

private:
	// ��]���x
	float m_rotSpeed;
	// �ړ����x
	float m_heightSpeed;
	// �㉺�ړ�����
	bool m_isUp;
	// ���E�ړ�����
	bool m_isRight;
	// ���E�ړ��̃t���[���J�E���g
	int m_moveFrameCount;
	// �v���C���[���f���n���h��
	int m_hModel;
	// �v���C���[�̃��f��
	std::shared_ptr<class Model> m_pModel;
};

