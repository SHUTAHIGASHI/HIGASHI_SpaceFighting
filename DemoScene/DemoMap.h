#pragma once
#include <vector>
#include <memory>
#include "game.h"

class DemoMap
{
public:
	// �R���X�g���N�^
	DemoMap();
	// �f�X�g���N�^
	~DemoMap();

	// ������
	void Init();
	// �X�V
	void Update();
	// �`��
	void Draw();

	// ����W�̐ݒ�
	void SetBasePos(const VECTOR pos) { m_basePos = pos; }

private:
	// �X�e�[�W�w�i�I�u�W�F�N�g�̐���
	void CreateStageBackModel();
	// �X�e�[�W�w�i�I�u�W�F�N�g�̍��W����
	VECTOR GetRandStageBackPos();

private:
	// �X�e�[�W�w�i�I�u�W�F�N�g�n���h��
	int m_hStageBackModelHandleBase;
	std::vector<class Model*> m_stageBackModel;

	// �w�i�I�u�W�F�N�g��z�u�������W
	VECTOR m_basePos;

private:
	// �X�J�C�h�[��
	std::shared_ptr<class SkyDome> m_pSkyDome;
};

