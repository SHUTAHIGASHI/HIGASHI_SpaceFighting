#pragma once
#include "Game.h"
#include <memory>

// �J����
class DemoCamera
{
public:
	// �R���X�g���N�^
	DemoCamera();
	// �f�X�g���N�^
	virtual ~DemoCamera();

	// �v���C���[�̃|�C���^�Z�b�g
	void SetPlayer(class DemoPlayer* player) { m_pDemoPlayer = player; }

	// �J����������
	void Init();
	// �J�����X�V
	void Update();

private:
	// �J�����̈ʒu�X�V
	void UpdateCameraPos();

private:
	// �J�����^�[�Q�b�g���W
	VECTOR m_targetPos;

	// �v���C���[�|�C���^
	class DemoPlayer* m_pDemoPlayer;
	// �J����
	std::shared_ptr<class Camera> m_pCamera;
};
