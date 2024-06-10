#pragma once
#include "Game.h"
#include <vector>
#include <memory>

class StageManager
{
public:
	// �R���X�g���N�^
	StageManager();
	// �f�X�g���N�^
	virtual ~StageManager();

	// �v���C���[�|�C���^�Z�b�g
	void SetPlayer(class Player* player) { m_pPlayer = player; }
	// UI�|�C���^�Z�b�g
	void SetUi(class UiManager* ui) { m_pUi = ui; }

	// ������
	void Init();
	// �X�V
	void Update(const InputState& input);
	// �`��
	void Draw();

	// �X�e�[�W�I�����̏���
	void OnStageEnd();

	// �{�X�擾
	const class EnemyBoss* GetBoss() const;

	// �X�e�[�W�N���A����
	const bool IsClear() const;

private:
	// �}�b�v
	std::shared_ptr<class Map> m_pMap;
	// �X�e�[�W�Ǘ�
	std::shared_ptr<class Stage> m_pStage;
	// �v���C���[
	class Player* m_pPlayer;
	// UI
	class UiManager* m_pUi;
};

