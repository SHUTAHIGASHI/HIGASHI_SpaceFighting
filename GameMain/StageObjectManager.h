#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "Game.h"

class StageObjectManager
{
public:
	// �R���X�g���N�^
	StageObjectManager();
	// �f�X�g���N�^
	virtual ~StageObjectManager();

	// �X�e�[�W�Ǘ��N���X�Z�b�g
	void SetStage(class Stage* stage) { m_pStage = stage; }
	// UI�|�C���^�Z�b�g
	void SetUi(class UiManager* ui) { m_pUi = ui; }

	// ������
	void Init();
	// �X�V
	void Update();
	// �`��
	void Draw();
	// �v���C���[�Ƃ̓����蔻��
	void CheckPlayerCollision(class Player* player);
	// �w��I�u�W�F�N�g����
	void CreateObject(std::string objName, VECTOR pos);

	// �v���C���[���W�ݒ�
	void SetPlayerPos(VECTOR pos) { m_playerPos = pos; }

	// �I�u�W�F�N�g���擾
	std::vector<std::shared_ptr<class StageObjectBase>> GetStageObject() const { return m_pObjects; }

private:
	// �I�u�W�F�N�g�폜
	void DeleteDisableObject();

private:
	// �v���C���[���W
	VECTOR m_playerPos;
	// �I�u�W�F�N�g�z��
	std::vector<std::shared_ptr<class StageObjectBase>> m_pObjects;
	// �A�C�e���������W
	std::vector<VECTOR> m_itemCreatePos;

private:
	// �X�e�[�W�Ǘ�
	class Stage* m_pStage;
	// UI
	class UiManager* m_pUi;
};

