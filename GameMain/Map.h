#pragma once
#include <vector>
#include <memory>
#include "game.h"

class Map
{
public:
	// �R���X�g���N�^
	Map();
	// �f�X�g���N�^
	~Map();

	// ������
	void Init();
	// �X�V
	void Update();
	// �`��
	void Draw();

	// �X�e�[�W�I�����̏���
	void OnStageEnd();

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
	// �w�i�I�u�W�F�N�g���폜����͈�
	float m_deleteRange;

private:
	// �X�J�C�h�[��
	std::shared_ptr<class SkyDome> m_pSkyDome;
};

